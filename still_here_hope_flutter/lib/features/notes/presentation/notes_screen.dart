import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';

import '../../../models/app_models.dart';
import '../../../services/content_moderation_service.dart';
import '../../../views/components/calm_components.dart';
import '../../notes/data/hope_repository.dart';

class NotesScreen extends ConsumerStatefulWidget {
  const NotesScreen({super.key});

  @override
  ConsumerState<NotesScreen> createState() => _NotesScreenState();
}

class _NotesScreenState extends ConsumerState<NotesScreen> {
  MoodType? filter;

  @override
  Widget build(BuildContext context) {
    final repo = ref.watch(hopeRepositoryProvider);
    final notes = filter == null
        ? repo.visibleNotes
        : repo.visibleNotes.where((note) => note.tags.contains(filter)).toList();

    return Scaffold(
      floatingActionButton: FloatingActionButton.extended(
        heroTag: 'write-note',
        onPressed: () => showModalBottomSheet<void>(
          context: context,
          isScrollControlled: true,
          builder: (_) => const _WriteNoteSheet(),
        ),
        icon: const Icon(Icons.edit_note),
        label: const Text('Add note'),
      ),
      body: CalmScreen(
        title: 'Notes',
        subtitle: 'Small anonymous words from people still trying.',
        children: [
          const SectionHeader(
            eyebrow: 'Browse gently',
            title: 'Notes feed',
            subtitle: 'Filter by feeling if you want something closer to your day.',
          ),
          const SizedBox(height: 12),
          SingleChildScrollView(
            scrollDirection: Axis.horizontal,
            child: Row(
              children: [
                Padding(
                  padding: const EdgeInsets.only(right: 8),
                  child: ChoiceChip(
                    label: const Text('All'),
                    selected: filter == null,
                    onSelected: (_) => setState(() => filter = null),
                  ),
                ),
                ...MoodType.values.map(
                  (mood) => Padding(
                    padding: const EdgeInsets.only(right: 8),
                    child: ChoiceChip(
                      label: Text(mood.title),
                      selected: filter == mood,
                      onSelected: (_) => setState(() => filter = mood),
                    ),
                  ),
                ),
              ],
            ),
          ),
          const SizedBox(height: 16),
          if (notes.isEmpty)
            const CalmCard(
              child: Text('No notes here yet. Try another filter or add a gentle note.'),
            )
          else
            ...notes.map(
              (note) => Padding(
                padding: const EdgeInsets.only(bottom: 14),
                child: HopeNoteCard(
                  note: note,
                  onHelpful: () => repo.markHelpful(note.id),
                  onSave: () => repo.toggleSaved(note.id),
                  onReport: () => context.push('/report', extra: note),
                  onBlock: () => _confirmBlock(context, repo, note),
                ),
              ),
            ),
        ],
      ),
    );
  }

  Future<void> _confirmBlock(BuildContext context, HopeRepository repo, HopeNote note) async {
    final confirmed = await showDialog<bool>(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('Block this source?'),
        content: const Text('Posts from this source will no longer appear in your feed.'),
        actions: [
          TextButton(onPressed: () => Navigator.pop(context, false), child: const Text('Cancel')),
          FilledButton(onPressed: () => Navigator.pop(context, true), child: const Text('Block')),
        ],
      ),
    );
    if (confirmed == true) {
      await repo.blockSource(note.anonymousUserId);
      if (context.mounted) {
        ScaffoldMessenger.of(context).showSnackBar(const SnackBar(content: Text('Posts from this source are hidden.')));
      }
    }
  }
}

class _WriteNoteSheet extends ConsumerStatefulWidget {
  const _WriteNoteSheet();

  @override
  ConsumerState<_WriteNoteSheet> createState() => _WriteNoteSheetState();
}

class _WriteNoteSheetState extends ConsumerState<_WriteNoteSheet> {
  final textController = TextEditingController();
  final selectedTags = <MoodType>{};
  String? error;

  @override
  Widget build(BuildContext context) {
    final bottom = MediaQuery.of(context).viewInsets.bottom;
    return Padding(
      padding: EdgeInsets.fromLTRB(20, 20, 20, bottom + 20),
      child: SingleChildScrollView(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text('What would you tell someone feeling like you?', style: Theme.of(context).textTheme.titleLarge),
            const SizedBox(height: 12),
            TextField(
              controller: textController,
              minLines: 3,
              maxLines: 5,
              maxLength: 240,
              onChanged: (_) => setState(() => error = null),
              decoration: InputDecoration(
                hintText: 'Keep it gentle, anonymous, and safe.',
                errorText: error,
                border: const OutlineInputBorder(),
              ),
            ),
            Wrap(
              spacing: 8,
              runSpacing: 8,
              children: MoodType.values.map((mood) {
                return FilterChip(
                  label: Text(mood.title),
                  selected: selectedTags.contains(mood),
                  onSelected: (selected) {
                    setState(() {
                      selected ? selectedTags.add(mood) : selectedTags.remove(mood);
                    });
                  },
                );
              }).toList(),
            ),
            const SizedBox(height: 16),
            FilledButton(
              onPressed: _submit,
              child: const Text('Share anonymously'),
            ),
          ],
        ),
      ),
    );
  }

  Future<void> _submit() async {
    final text = textController.text.trim();
    if (text.isEmpty) {
      setState(() => error = 'Add a few words first.');
      return;
    }

    final moderation = ContentModerationService().evaluate(text);
    if (moderation is ModerationBlocked) {
      setState(() => error = moderation.message);
      return;
    }

    final status = moderation is ModerationPendingReview
        ? ModerationStatus.underReview
        : ModerationStatus.visible;
    await ref.read(hopeRepositoryProvider).addNote(
          text,
          selectedTags.isEmpty ? [MoodType.overwhelmed] : selectedTags.toList(),
          status,
        );
    if (mounted) {
      Navigator.pop(context);
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(
          content: Text(
            status == ModerationStatus.visible
                ? 'Your words may help someone keep going.'
                : 'Your note is saved for review.',
          ),
        ),
      );
    }
  }
}
