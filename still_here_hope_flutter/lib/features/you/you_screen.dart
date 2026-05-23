import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';

import '../../models/app_models.dart';
import '../../views/components/calm_components.dart';
import '../notes/data/hope_repository.dart';

class YouScreen extends ConsumerWidget {
  const YouScreen({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final repo = ref.watch(hopeRepositoryProvider);
    final stats = repo.stats;

    return Scaffold(
      body: CalmScreen(
        title: 'You',
        subtitle: 'A quiet view of what has helped and what you have carried.',
        children: [
          LayoutBuilder(
            builder: (context, constraints) {
              final wide = constraints.maxWidth > 520;
              final cards = [
                _StatCard(icon: Icons.local_fire_department_outlined, value: '${stats.streakCount}', label: 'day streak'),
                _StatCard(icon: Icons.bookmark_border, value: '${stats.totalSavedNotes}', label: 'saved notes'),
              ];
              return wide
                  ? Row(children: cards.map((card) => Expanded(child: Padding(padding: const EdgeInsets.only(right: 12), child: card))).toList())
                  : Column(children: cards.map((card) => Padding(padding: const EdgeInsets.only(bottom: 12), child: card)).toList());
            },
          ),
          const SizedBox(height: 16),
          CalmCard(
            child: Text('Your notes helped ${stats.totalHelpfulNotes} people\n\nEven short words can help someone keep going.'),
          ),
          const SizedBox(height: 22),
          ListTile(
            shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(18)),
            tileColor: Theme.of(context).colorScheme.surface,
            leading: const Icon(Icons.bookmark_border),
            title: const Text('Saved notes'),
            subtitle: Text(repo.savedNotes.isEmpty ? 'No saved notes yet' : '${repo.savedNotes.length} saved'),
          ),
          const SizedBox(height: 10),
          ListTile(
            shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(18)),
            tileColor: Theme.of(context).colorScheme.surface,
            leading: const Icon(Icons.mail_outline),
            title: const Text('Report a concern'),
            subtitle: const Text('support@stillherehope.com'),
          ),
          const SizedBox(height: 10),
          ListTile(
            shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(18)),
            tileColor: Theme.of(context).colorScheme.surface,
            leading: const Icon(Icons.settings_outlined),
            title: const Text('Settings'),
            trailing: const Icon(Icons.chevron_right),
            onTap: () => context.push('/settings'),
          ),
          const SizedBox(height: 22),
          const SectionHeader(
            eyebrow: 'Patterns',
            title: 'Mood history',
            subtitle: 'A soft record of what days have asked from you.',
          ),
          const SizedBox(height: 12),
          if (repo.checkIns.isEmpty)
            const CalmCard(child: Text('No check-ins yet.'))
          else
            ...repo.checkIns.take(12).map(
                  (checkIn) => Padding(
                    padding: const EdgeInsets.only(bottom: 10),
                    child: CalmCard(
                      child: Text('${checkIn.mood.title}: ${checkIn.suggestedAction}'),
                    ),
                  ),
                ),
        ],
      ),
    );
  }
}

class _StatCard extends StatelessWidget {
  const _StatCard({required this.icon, required this.value, required this.label});
  final IconData icon;
  final String value;
  final String label;

  @override
  Widget build(BuildContext context) {
    return CalmCard(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Icon(icon, color: Theme.of(context).colorScheme.primary),
          const SizedBox(height: 14),
          Text(value, style: Theme.of(context).textTheme.headlineMedium?.copyWith(fontWeight: FontWeight.bold)),
          Text(label),
        ],
      ),
    );
  }
}
