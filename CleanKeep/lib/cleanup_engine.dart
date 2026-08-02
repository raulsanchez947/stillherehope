import 'dart:io';

enum CleanupCategory { oldFile, junk, important, securityReview }

enum CleanupRisk { low, medium, high }

class CleanupFinding {
  CleanupFinding({
    required this.path,
    required this.name,
    required this.bytes,
    required this.modified,
    required this.category,
    required this.risk,
    required this.reason,
    this.selected = false,
  });

  final String path;
  final String name;
  final int bytes;
  final DateTime modified;
  final CleanupCategory category;
  final CleanupRisk risk;
  final String reason;
  bool selected;

  bool get isDeletableByDefault =>
      category == CleanupCategory.junk || category == CleanupCategory.oldFile;
}

class CleanupSummary {
  const CleanupSummary({
    required this.scannedFiles,
    required this.scannedBytes,
    required this.findings,
    required this.skippedPaths,
  });

  final int scannedFiles;
  final int scannedBytes;
  final List<CleanupFinding> findings;
  final List<String> skippedPaths;

  int get reclaimableBytes => findings
      .where((finding) => finding.isDeletableByDefault)
      .fold(0, (total, finding) => total + finding.bytes);
}

class CleanupEngine {
  CleanupEngine({DateTime? now, this.oldFileDays = 365, this.maxFiles = 10000})
    : now = now ?? DateTime.now();

  final DateTime now;
  final int oldFileDays;
  final int maxFiles;

  static const Set<String> junkExtensions = {
    '.tmp',
    '.temp',
    '.bak',
    '.old',
    '.log',
    '.crash',
    '.dmp',
    '.download',
    '.part',
  };

  static const Set<String> importantExtensions = {
    '.doc',
    '.docx',
    '.xls',
    '.xlsx',
    '.ppt',
    '.pptx',
    '.pdf',
    '.pages',
    '.numbers',
    '.jpg',
    '.jpeg',
    '.png',
    '.heic',
    '.mov',
    '.mp4',
    '.sqlite',
    '.db',
    '.tax',
    '.qbb',
    '.qbw',
    '.pem',
    '.key',
    '.p12',
  };

  static const Set<String> riskyExtensions = {
    '.app',
    '.command',
    '.dmg',
    '.pkg',
    '.sh',
    '.zsh',
    '.bash',
    '.py',
    '.rb',
    '.js',
    '.jar',
    '.webloc',
    '.mobileconfig',
  };

  static const List<String> suspiciousNameFragments = [
    'install flash',
    'free vpn',
    'cleanmymac crack',
    'adobe crack',
    'keygen',
    'patcher',
    'trojan',
    'miner',
    'stealer',
    'payload',
  ];

  Future<CleanupSummary> scanRoots(List<String> roots) async {
    final findings = <CleanupFinding>[];
    final skippedPaths = <String>[];
    var scannedFiles = 0;
    var scannedBytes = 0;

    for (final root in roots) {
      final directory = Directory(root);
      if (!await directory.exists()) {
        skippedPaths.add(root);
        continue;
      }

      await for (final entity in directory.list(
        recursive: true,
        followLinks: false,
      )) {
        if (scannedFiles >= maxFiles) {
          skippedPaths.add('Scan stopped after $maxFiles files.');
          break;
        }
        if (entity is! File) {
          continue;
        }
        if (_shouldSkip(entity.path)) {
          continue;
        }

        try {
          final stat = await entity.stat();
          scannedFiles += 1;
          scannedBytes += stat.size;
          final finding = classifyFile(entity.path, stat.size, stat.modified);
          if (finding != null) {
            finding.selected =
                finding.isDeletableByDefault &&
                finding.risk != CleanupRisk.high;
            findings.add(finding);
          }
        } on FileSystemException catch (_) {
          skippedPaths.add(entity.path);
        }
      }
    }

    findings.sort((a, b) {
      final riskCompare = b.risk.index.compareTo(a.risk.index);
      if (riskCompare != 0) {
        return riskCompare;
      }
      return b.bytes.compareTo(a.bytes);
    });

    return CleanupSummary(
      scannedFiles: scannedFiles,
      scannedBytes: scannedBytes,
      findings: findings,
      skippedPaths: skippedPaths,
    );
  }

  CleanupFinding? classifyFile(String path, int bytes, DateTime modified) {
    final name = path.split(Platform.pathSeparator).last;
    final lowerName = name.toLowerCase();
    final extension = _extension(lowerName);
    final age = now.difference(modified);

    if (_looksSecurityRelevant(lowerName, extension)) {
      return CleanupFinding(
        path: path,
        name: name,
        bytes: bytes,
        modified: modified,
        category: CleanupCategory.securityReview,
        risk: CleanupRisk.high,
        reason:
            'Potentially executable or suspiciously named. Review manually; CleanKeep does not claim confirmed malware detection.',
      );
    }

    if (importantExtensions.contains(extension)) {
      return CleanupFinding(
        path: path,
        name: name,
        bytes: bytes,
        modified: modified,
        category: CleanupCategory.important,
        risk: CleanupRisk.high,
        reason: 'Likely personal, financial, work, media, or credential file.',
      );
    }

    if (_isJunkName(lowerName) || junkExtensions.contains(extension)) {
      return CleanupFinding(
        path: path,
        name: name,
        bytes: bytes,
        modified: modified,
        category: CleanupCategory.junk,
        risk: CleanupRisk.low,
        reason: 'Looks like cache, temporary, crash, partial, or backup junk.',
      );
    }

    if (age.inDays > oldFileDays) {
      return CleanupFinding(
        path: path,
        name: name,
        bytes: bytes,
        modified: modified,
        category: CleanupCategory.oldFile,
        risk: CleanupRisk.medium,
        reason: 'Not modified in ${age.inDays} days.',
      );
    }

    return null;
  }

  Future<int> moveSelectedToQuarantine(List<CleanupFinding> findings) async {
    var movedBytes = 0;
    for (final finding in findings.where((finding) => finding.selected)) {
      final source = File(finding.path);
      if (!await source.exists()) {
        continue;
      }
      final quarantine = Directory(
        '${source.parent.path}${Platform.pathSeparator}.cleankeep_quarantine',
      );
      await quarantine.create(recursive: true);
      final safeName = _safeQuarantineName(finding.name);
      final destination = File(
        '${quarantine.path}${Platform.pathSeparator}${DateTime.now().millisecondsSinceEpoch}_$safeName',
      );
      await source.rename(destination.path);
      movedBytes += finding.bytes;
    }
    return movedBytes;
  }

  bool _looksSecurityRelevant(String lowerName, String extension) {
    return riskyExtensions.contains(extension) ||
        suspiciousNameFragments.any(lowerName.contains);
  }

  bool _isJunkName(String lowerName) {
    return lowerName == '.ds_store' ||
        lowerName == 'thumbs.db' ||
        lowerName.endsWith('~') ||
        lowerName.startsWith('~\$') ||
        lowerName.contains('cache') ||
        lowerName.contains('crash') ||
        lowerName.contains('temporary');
  }

  bool _shouldSkip(String path) {
    final lowerPath = path.toLowerCase();
    return lowerPath.contains('${Platform.pathSeparator}.git') ||
        lowerPath.contains(
          '${Platform.pathSeparator}library${Platform.pathSeparator}keychains',
        ) ||
        lowerPath.contains(
          '${Platform.pathSeparator}library${Platform.pathSeparator}mail',
        ) ||
        lowerPath.contains(
          '${Platform.pathSeparator}photos library.photoslibrary',
        ) ||
        lowerPath.contains('${Platform.pathSeparator}.cleankeep_quarantine');
  }

  String _extension(String lowerName) {
    final index = lowerName.lastIndexOf('.');
    if (index <= 0) {
      return '';
    }
    return lowerName.substring(index);
  }

  String _safeQuarantineName(String name) {
    return name.replaceAll(RegExp(r'[^a-zA-Z0-9._ -]'), '_');
  }
}

String formatBytes(int bytes) {
  const units = ['B', 'KB', 'MB', 'GB', 'TB'];
  var value = bytes.toDouble();
  var unitIndex = 0;
  while (value >= 1024 && unitIndex < units.length - 1) {
    value /= 1024;
    unitIndex += 1;
  }
  return '${value.toStringAsFixed(value >= 10 || unitIndex == 0 ? 0 : 1)} ${units[unitIndex]}';
}
