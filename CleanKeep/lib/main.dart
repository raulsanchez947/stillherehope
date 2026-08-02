import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

import 'cleanup_engine.dart';

void main() {
  runApp(const CleanKeepApp());
}

class CleanKeepApp extends StatelessWidget {
  const CleanKeepApp({super.key});

  @override
  Widget build(BuildContext context) {
    final colorScheme = ColorScheme.fromSeed(
      seedColor: const Color(0xFF1C7C54),
      brightness: Brightness.light,
    );

    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'CleanKeep',
      theme: ThemeData(
        colorScheme: colorScheme,
        useMaterial3: true,
        cardTheme: CardThemeData(
          clipBehavior: Clip.antiAlias,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(24),
          ),
        ),
      ),
      home: const CleanerHomePage(),
    );
  }
}

class CleanerHomePage extends StatefulWidget {
  const CleanerHomePage({super.key});

  @override
  State<CleanerHomePage> createState() => _CleanerHomePageState();
}

class _CleanerHomePageState extends State<CleanerHomePage> {
  static const MethodChannel _platform = MethodChannel('cleankeep/files');

  final CleanupEngine _engine = CleanupEngine();
  CleanupSummary? _summary;
  bool _isScanning = false;
  bool _isCleaning = false;
  String? _status;
  List<String> _roots = const [];

  @override
  void initState() {
    super.initState();
    _loadDefaultRoots();
  }

  Future<void> _loadDefaultRoots() async {
    final roots = <String>[];
    if (Platform.isMacOS) {
      try {
        final platformRoots = await _platform.invokeListMethod<String>(
          'defaultScanRoots',
        );
        roots.addAll(platformRoots ?? const []);
      } on PlatformException {
        roots.addAll(_fallbackRoots());
      }
    } else {
      roots.addAll(_fallbackRoots());
    }

    if (mounted) {
      setState(() {
        _roots = roots.toSet().toList();
      });
    }
  }

  List<String> _fallbackRoots() {
    final roots = <String>[];
    roots.add(Directory.systemTemp.path);
    try {
      roots.add(Directory.current.path);
    } on FileSystemException {
      // Directory.current may be unavailable on some sandboxed builds.
    }
    return roots;
  }

  Future<void> _chooseMacFolder() async {
    if (!Platform.isMacOS) {
      setState(() {
        _status = 'iPhone and iPad builds scan only app-accessible storage.';
      });
      return;
    }

    try {
      final path = await _platform.invokeMethod<String>('chooseDirectory');
      if (path == null || path.isEmpty) {
        return;
      }
      setState(() {
        _roots = {..._roots, path}.toList();
      });
    } on PlatformException catch (error) {
      setState(() {
        _status =
            'Could not open folder picker: ${error.message ?? error.code}';
      });
    }
  }

  Future<void> _scan() async {
    if (_roots.isEmpty) {
      setState(() {
        _status = 'Choose at least one folder to scan.';
      });
      return;
    }

    setState(() {
      _isScanning = true;
      _status = 'Scanning locally. Nothing is uploaded.';
      _summary = null;
    });

    try {
      final summary = await _engine.scanRoots(_roots);
      setState(() {
        _summary = summary;
        _status =
            'Found ${summary.findings.length} review items across ${summary.scannedFiles} files.';
      });
    } on FileSystemException catch (error) {
      setState(() {
        _status = 'Scan stopped: ${error.message}';
      });
    } finally {
      if (mounted) {
        setState(() {
          _isScanning = false;
        });
      }
    }
  }

  Future<void> _quarantineSelected() async {
    final summary = _summary;
    if (summary == null) {
      return;
    }

    final selected = summary.findings.where((finding) => finding.selected);
    if (selected.isEmpty) {
      setState(() {
        _status =
            'Nothing selected. Important files stay protected by default.';
      });
      return;
    }

    setState(() {
      _isCleaning = true;
      _status = 'Moving selected files to local quarantine...';
    });

    try {
      final movedBytes = await _engine.moveSelectedToQuarantine(
        summary.findings,
      );
      setState(() {
        _status =
            'Moved ${formatBytes(movedBytes)} into .cleankeep_quarantine folders.';
      });
      await _scan();
    } on FileSystemException catch (error) {
      setState(() {
        _status = 'Clean-up failed safely: ${error.message}';
      });
    } finally {
      if (mounted) {
        setState(() {
          _isCleaning = false;
        });
      }
    }
  }

  void _toggleFinding(CleanupFinding finding, bool? value) {
    setState(() {
      finding.selected = value ?? false;
    });
  }

  @override
  Widget build(BuildContext context) {
    final summary = _summary;
    final selectedBytes =
        summary?.findings
            .where((finding) => finding.selected)
            .fold<int>(0, (total, finding) => total + finding.bytes) ??
        0;

    return Scaffold(
      body: SafeArea(
        child: CustomScrollView(
          slivers: [
            SliverAppBar.large(
              title: const Text('CleanKeep'),
              actions: [
                IconButton(
                  onPressed: _chooseMacFolder,
                  icon: const Icon(Icons.create_new_folder_outlined),
                  tooltip: 'Choose folder',
                ),
              ],
            ),
            SliverPadding(
              padding: const EdgeInsets.fromLTRB(20, 0, 20, 28),
              sliver: SliverList.list(
                children: [
                  _HeroCard(
                    isScanning: _isScanning,
                    roots: _roots,
                    onScan: _isScanning ? null : _scan,
                    onChooseFolder: _chooseMacFolder,
                  ),
                  const SizedBox(height: 16),
                  if (_status != null) _StatusCard(message: _status!),
                  if (summary != null) ...[
                    const SizedBox(height: 16),
                    _SummaryGrid(summary: summary),
                    const SizedBox(height: 16),
                    FilledButton.icon(
                      onPressed: _isCleaning
                          ? null
                          : () => _confirmQuarantine(selectedBytes),
                      icon: const Icon(Icons.inventory_2_outlined),
                      label: Text(
                        'Quarantine selected (${formatBytes(selectedBytes)})',
                      ),
                    ),
                    const SizedBox(height: 16),
                    _FindingList(
                      findings: summary.findings,
                      onChanged: _toggleFinding,
                    ),
                  ],
                  const SizedBox(height: 24),
                  const _SafetyNotes(),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }

  Future<void> _confirmQuarantine(int selectedBytes) async {
    final confirmed = await showDialog<bool>(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('Move files to quarantine?'),
        content: Text(
          'CleanKeep will move ${formatBytes(selectedBytes)} into local .cleankeep_quarantine folders. It does not permanently delete files.',
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context, false),
            child: const Text('Cancel'),
          ),
          FilledButton(
            onPressed: () => Navigator.pop(context, true),
            child: const Text('Quarantine'),
          ),
        ],
      ),
    );

    if (confirmed == true) {
      await _quarantineSelected();
    }
  }
}

class _HeroCard extends StatelessWidget {
  const _HeroCard({
    required this.isScanning,
    required this.roots,
    required this.onScan,
    required this.onChooseFolder,
  });

  final bool isScanning;
  final List<String> roots;
  final VoidCallback? onScan;
  final VoidCallback onChooseFolder;

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(24),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Icon(Icons.shield_moon_outlined, size: 44),
            const SizedBox(height: 16),
            Text(
              'Find old files, junk, and risky-looking downloads without uploading anything.',
              style: Theme.of(context).textTheme.headlineSmall,
            ),
            const SizedBox(height: 12),
            Text(
              'Files older than 365 days are separated from likely important files. Security review flags are heuristic, not a confirmed malware diagnosis.',
              style: Theme.of(context).textTheme.bodyLarge,
            ),
            const SizedBox(height: 20),
            Wrap(
              spacing: 12,
              runSpacing: 12,
              children: [
                FilledButton.icon(
                  onPressed: onScan,
                  icon: isScanning
                      ? const SizedBox.square(
                          dimension: 18,
                          child: CircularProgressIndicator(strokeWidth: 2),
                        )
                      : const Icon(Icons.search),
                  label: Text(isScanning ? 'Scanning...' : 'Scan now'),
                ),
                OutlinedButton.icon(
                  onPressed: onChooseFolder,
                  icon: const Icon(Icons.folder_open_outlined),
                  label: const Text('Add folder'),
                ),
              ],
            ),
            const SizedBox(height: 18),
            Text('Scan roots', style: Theme.of(context).textTheme.labelLarge),
            const SizedBox(height: 8),
            for (final root in roots)
              Padding(
                padding: const EdgeInsets.only(bottom: 6),
                child: Text(
                  root,
                  maxLines: 1,
                  overflow: TextOverflow.ellipsis,
                  style: Theme.of(context).textTheme.bodySmall,
                ),
              ),
          ],
        ),
      ),
    );
  }
}

class _StatusCard extends StatelessWidget {
  const _StatusCard({required this.message});

  final String message;

  @override
  Widget build(BuildContext context) {
    return Card(
      color: Theme.of(context).colorScheme.secondaryContainer,
      child: Padding(padding: const EdgeInsets.all(16), child: Text(message)),
    );
  }
}

class _SummaryGrid extends StatelessWidget {
  const _SummaryGrid({required this.summary});

  final CleanupSummary summary;

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(
      builder: (context, constraints) {
        final columns = constraints.maxWidth > 720 ? 4 : 2;
        return GridView.count(
          crossAxisCount: columns,
          shrinkWrap: true,
          physics: const NeverScrollableScrollPhysics(),
          mainAxisSpacing: 12,
          crossAxisSpacing: 12,
          childAspectRatio: 1.45,
          children: [
            _MetricCard(
              label: 'Scanned',
              value: '${summary.scannedFiles} files',
            ),
            _MetricCard(
              label: 'Data read',
              value: formatBytes(summary.scannedBytes),
            ),
            _MetricCard(
              label: 'Review items',
              value: '${summary.findings.length}',
            ),
            _MetricCard(
              label: 'Likely reclaimable',
              value: formatBytes(summary.reclaimableBytes),
            ),
          ],
        );
      },
    );
  }
}

class _MetricCard extends StatelessWidget {
  const _MetricCard({required this.label, required this.value});

  final String label;
  final String value;

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(label, style: Theme.of(context).textTheme.labelMedium),
            const SizedBox(height: 8),
            Text(value, style: Theme.of(context).textTheme.titleLarge),
          ],
        ),
      ),
    );
  }
}

class _FindingList extends StatelessWidget {
  const _FindingList({required this.findings, required this.onChanged});

  final List<CleanupFinding> findings;
  final void Function(CleanupFinding finding, bool? value) onChanged;

  @override
  Widget build(BuildContext context) {
    if (findings.isEmpty) {
      return const Card(
        child: Padding(
          padding: EdgeInsets.all(24),
          child: Text('Nothing risky or stale found in the selected folders.'),
        ),
      );
    }

    return Card(
      child: Column(
        children: [
          for (final finding in findings)
            CheckboxListTile(
              value: finding.selected,
              onChanged: finding.risk == CleanupRisk.high
                  ? null
                  : (value) => onChanged(finding, value),
              title: Text(
                finding.name,
                maxLines: 1,
                overflow: TextOverflow.ellipsis,
              ),
              subtitle: Text(
                '${_categoryLabel(finding.category)} • ${formatBytes(finding.bytes)} • ${finding.reason}\n${finding.path}',
                maxLines: 3,
                overflow: TextOverflow.ellipsis,
              ),
              secondary: Icon(_categoryIcon(finding.category)),
              isThreeLine: true,
            ),
        ],
      ),
    );
  }

  String _categoryLabel(CleanupCategory category) {
    return switch (category) {
      CleanupCategory.oldFile => 'Older than 365 days',
      CleanupCategory.junk => 'Likely junk',
      CleanupCategory.important => 'Protected important file',
      CleanupCategory.securityReview => 'Security review',
    };
  }

  IconData _categoryIcon(CleanupCategory category) {
    return switch (category) {
      CleanupCategory.oldFile => Icons.history,
      CleanupCategory.junk => Icons.delete_sweep_outlined,
      CleanupCategory.important => Icons.lock_outline,
      CleanupCategory.securityReview => Icons.warning_amber_outlined,
    };
  }
}

class _SafetyNotes extends StatelessWidget {
  const _SafetyNotes();

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(20),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              'Safety model',
              style: Theme.of(context).textTheme.titleMedium,
            ),
            const SizedBox(height: 8),
            const Text(
              '• Review-first workflow; selected files are quarantined, not permanently deleted.',
            ),
            const Text('• Important file types are locked by default.'),
            const Text(
              '• iPhone storage access is intentionally limited by iOS sandboxing.',
            ),
            const Text(
              '• Malware flags are heuristic indicators for review, not antivirus certification.',
            ),
          ],
        ),
      ),
    );
  }
}
