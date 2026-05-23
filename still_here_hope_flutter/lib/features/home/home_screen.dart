import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';

import '../../models/app_models.dart';
import '../../views/components/calm_components.dart';
import 'home_controller.dart';

class HomeScreen extends ConsumerWidget {
  const HomeScreen({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final state = ref.watch(homeControllerProvider);
    final controller = ref.read(homeControllerProvider.notifier);

    return Scaffold(
      floatingActionButton: FloatingActionButton.extended(
        heroTag: 'crisis-home',
        onPressed: () => context.push('/crisis'),
        icon: const Icon(Icons.health_and_safety_outlined),
        label: const Text('Help now'),
      ),
      body: CalmScreen(
        title: 'Good morning',
        subtitle: 'A calm space to check in, slow down, and find one gentler next step.',
        children: [
          CalmCard(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                const SectionHeader(
                  eyebrow: 'Today',
                  title: 'Daily check-in',
                  subtitle: 'How does today feel in your body right now?',
                ),
                const SizedBox(height: 16),
                Wrap(
                  spacing: 10,
                  runSpacing: 10,
                  children: MoodType.values
                      .map(
                        (mood) => MoodChip(
                          mood: mood,
                          selected: state.selectedMood == mood,
                          onTap: () => controller.selectMood(mood),
                        ),
                      )
                      .toList(),
                ),
                if (state.selectedMood != null) ...[
                  const SizedBox(height: 16),
                  TextField(
                    minLines: 1,
                    maxLines: 3,
                    textInputAction: TextInputAction.done,
                    onSubmitted: (_) => FocusScope.of(context).unfocus(),
                    onTapOutside: (_) => FocusScope.of(context).unfocus(),
                    onChanged: controller.setHeaviness,
                    decoration: const InputDecoration(
                      labelText: 'What feels heaviest right now?',
                      border: OutlineInputBorder(),
                    ),
                  ),
                ],
              ],
            ),
          ),
          if (state.presentation != null) ...[
            const SizedBox(height: 16),
            _CheckInResultCard(
              presentation: state.presentation!,
              savedMessage: state.savedMessage,
              onSave: controller.saveCheckIn,
            ),
          ],
          const SizedBox(height: 28),
          const SectionHeader(
            eyebrow: 'Support',
            title: 'Quick support',
            subtitle: 'Choose one small kind of help for the next few minutes.',
          ),
          const SizedBox(height: 12),
          ...QuickAction.values.take(4).map(
                (action) => Padding(
                  padding: const EdgeInsets.only(bottom: 10),
                  child: _QuickActionTile(
                    action: action,
                    onTap: () => controller.quickAction(action),
                  ),
                ),
              ),
        ],
      ),
    );
  }
}

class _CheckInResultCard extends StatelessWidget {
  const _CheckInResultCard({
    required this.presentation,
    required this.onSave,
    this.savedMessage,
  });

  final CheckInPresentation presentation;
  final Future<void> Function() onSave;
  final String? savedMessage;

  @override
  Widget build(BuildContext context) {
    return CalmCard(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Text(presentation.supportiveLine, style: Theme.of(context).textTheme.titleLarge),
          const SizedBox(height: 14),
          Text('One small action', style: Theme.of(context).textTheme.labelLarge),
          Text(presentation.suggestedAction, style: Theme.of(context).textTheme.bodyLarge),
          const SizedBox(height: 14),
          Text('Grounding', style: Theme.of(context).textTheme.labelLarge),
          Text(presentation.groundingPrompt, style: Theme.of(context).textTheme.bodyLarge),
          if (presentation.sharedNote != null) ...[
            const SizedBox(height: 14),
            DecoratedBox(
              decoration: BoxDecoration(
                color: Theme.of(context).colorScheme.secondaryContainer,
                borderRadius: BorderRadius.circular(18),
              ),
              child: Padding(
                padding: const EdgeInsets.all(16),
                child: Text('From someone else: "${presentation.sharedNote!.text}"'),
              ),
            ),
          ],
          const SizedBox(height: 18),
          FilledButton.icon(
            onPressed: onSave,
            icon: const Icon(Icons.bookmark_add_outlined),
            label: const Text('Save check-in'),
          ),
          if (savedMessage != null) ...[
            const SizedBox(height: 8),
            Text(savedMessage!, style: Theme.of(context).textTheme.bodySmall),
          ],
        ],
      ),
    );
  }
}

class _QuickActionTile extends StatelessWidget {
  const _QuickActionTile({required this.action, required this.onTap});

  final QuickAction action;
  final VoidCallback onTap;

  @override
  Widget build(BuildContext context) {
    return Semantics(
      button: true,
      label: action.title,
      child: ListTile(
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(18)),
        tileColor: Theme.of(context).colorScheme.surface,
        leading: const Icon(Icons.self_improvement),
        title: Text(action.title, maxLines: 2),
        trailing: const Icon(Icons.chevron_right),
        onTap: onTap,
      ),
    );
  }
}
