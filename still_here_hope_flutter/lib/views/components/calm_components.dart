import 'package:flutter/material.dart';

import '../../core/layout/responsive.dart';
import '../../features/notes/domain/hope_note.dart';

class CalmScreen extends StatelessWidget {
  const CalmScreen({
    super.key,
    required this.title,
    this.subtitle,
    required this.children,
  });

  final String title;
  final String? subtitle;
  final List<Widget> children;

  @override
  Widget build(BuildContext context) {
    return SafeArea(
      child: ListView(
        padding: Responsive.screenPadding(context),
        children: [
          ResponsiveCenter(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(title, style: Theme.of(context).textTheme.displaySmall?.copyWith(fontWeight: FontWeight.w800)),
                if (subtitle != null) ...[
                  const SizedBox(height: 8),
                  Text(subtitle!, style: Theme.of(context).textTheme.titleMedium),
                ],
                const SizedBox(height: 24),
                ...children,
              ],
            ),
          ),
        ],
      ),
    );
  }
}

class CalmCard extends StatelessWidget {
  const CalmCard({super.key, required this.child});
  final Widget child;

  @override
  Widget build(BuildContext context) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(20),
        child: child,
      ),
    );
  }
}

class SectionHeader extends StatelessWidget {
  const SectionHeader({super.key, this.eyebrow, required this.title, this.subtitle});

  final String? eyebrow;
  final String title;
  final String? subtitle;

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        if (eyebrow != null)
          Text(
            eyebrow!.toUpperCase(),
            style: Theme.of(context).textTheme.labelSmall?.copyWith(
                  letterSpacing: 1.2,
                  color: Theme.of(context).colorScheme.onSurfaceVariant,
                  fontWeight: FontWeight.bold,
                ),
          ),
        Text(title, style: Theme.of(context).textTheme.headlineSmall?.copyWith(fontWeight: FontWeight.w800)),
        if (subtitle != null)
          Text(subtitle!, style: Theme.of(context).textTheme.bodyLarge?.copyWith(color: Theme.of(context).colorScheme.onSurfaceVariant)),
      ],
    );
  }
}

class MoodChip extends StatelessWidget {
  const MoodChip({super.key, required this.mood, required this.selected, required this.onTap});
  final MoodType mood;
  final bool selected;
  final VoidCallback onTap;

  @override
  Widget build(BuildContext context) {
    return Semantics(
      button: true,
      selected: selected,
      label: 'Mood ${mood.title}',
      child: ChoiceChip(
        label: Text(mood.title),
        selected: selected,
        onSelected: (_) => onTap(),
      ),
    );
  }
}

class HopeNoteCard extends StatelessWidget {
  const HopeNoteCard({
    super.key,
    required this.note,
    this.onHelpful,
    this.onSave,
    this.onReport,
    this.onBlock,
  });

  final HopeNote note;
  final VoidCallback? onHelpful;
  final VoidCallback? onSave;
  final VoidCallback? onReport;
  final VoidCallback? onBlock;

  @override
  Widget build(BuildContext context) {
    return CalmCard(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Wrap(
            spacing: 8,
            runSpacing: 8,
            children: note.tags.map((tag) => Chip(label: Text(tag.title))).toList(),
          ),
          const SizedBox(height: 12),
          Text(note.text, style: Theme.of(context).textTheme.bodyLarge),
          const SizedBox(height: 16),
          Wrap(
            spacing: 8,
            runSpacing: 8,
            alignment: WrapAlignment.spaceBetween,
            children: [
              if (onHelpful != null) OutlinedButton(onPressed: onHelpful, child: const Text('This helped')),
              if (onSave != null) FilledButton.tonal(onPressed: onSave, child: Text(note.isSaved ? 'Saved' : 'Save')),
              TextButton.icon(onPressed: null, icon: const Icon(Icons.favorite), label: Text('${note.helpedCount}')),
              if (onReport != null || onBlock != null)
                PopupMenuButton<String>(
                  tooltip: 'More actions',
                  onSelected: (value) {
                    if (value == 'report') onReport?.call();
                    if (value == 'block') onBlock?.call();
                  },
                  itemBuilder: (context) => [
                    if (onReport != null) const PopupMenuItem(value: 'report', child: Text('Report')),
                    if (onBlock != null) const PopupMenuItem(value: 'block', child: Text('Block posts from this source')),
                  ],
                  child: const Padding(
                    padding: EdgeInsets.all(8),
                    child: Icon(Icons.more_horiz),
                  ),
                ),
            ],
          ),
        ],
      ),
    );
  }
}

class CrisisBanner extends StatelessWidget {
  const CrisisBanner({super.key});

  @override
  Widget build(BuildContext context) {
    return Semantics(
      liveRegion: true,
      child: CalmCard(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text('Get immediate help now', style: Theme.of(context).textTheme.titleLarge?.copyWith(fontWeight: FontWeight.bold)),
            const SizedBox(height: 8),
            const Text('If you may be in immediate danger or might hurt yourself, call emergency services now. In the U.S., call or text 988.'),
          ],
        ),
      ),
    );
  }
}
