import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../models/app_models.dart';
import '../../views/components/calm_components.dart';
import '../notes/data/hope_repository.dart';

class ReportNoteScreen extends ConsumerStatefulWidget {
  const ReportNoteScreen({super.key, required this.note});
  final HopeNote note;

  @override
  ConsumerState<ReportNoteScreen> createState() => _ReportNoteScreenState();
}

class _ReportNoteScreenState extends ConsumerState<ReportNoteScreen> {
  ReportCategory? reason;
  bool blockSource = false;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Report note')),
      body: CalmScreen(
        title: 'Report a note',
        subtitle: 'This hides the note right away and marks it for review.',
        children: [
          CalmCard(child: Text(widget.note.text)),
          const SizedBox(height: 16),
          ...ReportCategory.values.map(
            (category) => ListTile(
              shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16)),
              title: Text(category.title),
              leading: Icon(
                reason == category ? Icons.radio_button_checked : Icons.radio_button_off,
                color: reason == category ? Theme.of(context).colorScheme.primary : null,
              ),
              onTap: () => setState(() => reason = category),
            ),
          ),
          SwitchListTile(
            value: blockSource,
            onChanged: (value) => setState(() => blockSource = value),
            title: const Text('Block posts from this source'),
            subtitle: const Text('No identifier is shown. This only hides future posts locally.'),
          ),
          const SizedBox(height: 12),
          FilledButton(
            onPressed: reason == null ? null : _submit,
            child: const Text('Submit report'),
          ),
        ],
      ),
    );
  }

  Future<void> _submit() async {
    await ref.read(hopeRepositoryProvider).report(
          widget.note.id,
          reason!,
          blockSource: blockSource,
        );
    if (mounted) {
      Navigator.pop(context);
      ScaffoldMessenger.of(context).showSnackBar(const SnackBar(content: Text('Thanks. The note is hidden.')));
    }
  }
}
