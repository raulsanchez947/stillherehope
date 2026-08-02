import 'package:cleankeep/cleanup_engine.dart';
import 'package:cleankeep/main.dart';
import 'package:flutter_test/flutter_test.dart';

void main() {
  final now = DateTime(2026, 8, 2);
  final engine = CleanupEngine(now: now);

  test('classifies files older than 365 days', () {
    final finding = engine.classifyFile(
      '/Users/example/Downloads/archive.txt',
      2048,
      now.subtract(const Duration(days: 366)),
    );

    expect(finding, isNotNull);
    expect(finding!.category, CleanupCategory.oldFile);
    expect(finding.risk, CleanupRisk.medium);
  });

  test('protects important document types', () {
    final finding = engine.classifyFile(
      '/Users/example/Documents/taxes.pdf',
      8192,
      now.subtract(const Duration(days: 800)),
    );

    expect(finding, isNotNull);
    expect(finding!.category, CleanupCategory.important);
    expect(finding.selected, isFalse);
  });

  test('identifies obvious junk', () {
    final finding = engine.classifyFile(
      '/Users/example/Downloads/install.tmp',
      512,
      now,
    );

    expect(finding, isNotNull);
    expect(finding!.category, CleanupCategory.junk);
    expect(finding.isDeletableByDefault, isTrue);
  });

  test('flags executable downloads for security review', () {
    final finding = engine.classifyFile(
      '/Users/example/Downloads/free vpn.command',
      4096,
      now,
    );

    expect(finding, isNotNull);
    expect(finding!.category, CleanupCategory.securityReview);
    expect(finding.risk, CleanupRisk.high);
  });

  testWidgets('app opens scanner home', (tester) async {
    await tester.pumpWidget(const CleanKeepApp());

    expect(find.text('CleanKeep'), findsWidgets);
    expect(find.text('Scan now'), findsOneWidget);
  });
}
