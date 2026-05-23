import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';

import '../../models/app_models.dart';
import '../../views/components/calm_components.dart';
import 'talk_controller.dart';

class TalkScreen extends ConsumerStatefulWidget {
  const TalkScreen({super.key});

  @override
  ConsumerState<TalkScreen> createState() => _TalkScreenState();
}

class _TalkScreenState extends ConsumerState<TalkScreen> {
  final textController = TextEditingController();

  @override
  void dispose() {
    textController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final state = ref.watch(talkControllerProvider);
    final controller = ref.read(talkControllerProvider.notifier);
    if (textController.text != state.draft) {
      textController.value = TextEditingValue(
        text: state.draft,
        selection: TextSelection.collapsed(offset: state.draft.length),
      );
    }

    return Scaffold(
      appBar: AppBar(
        title: const Text('Talk'),
        actions: [
          IconButton(
            tooltip: 'Crisis resources',
            onPressed: () => context.push('/crisis'),
            icon: const Icon(Icons.health_and_safety_outlined),
          ),
        ],
      ),
      body: SafeArea(
        child: Column(
          children: [
            Expanded(
              child: ListView(
                padding: const EdgeInsets.all(20),
                children: [
                  const SectionHeader(
                    eyebrow: 'Talk',
                    title: 'Start where you are',
                    subtitle: 'Type freely or choose a simple prompt.',
                  ),
                  const SizedBox(height: 12),
                  Wrap(
                    spacing: 8,
                    runSpacing: 8,
                    children: QuickAction.values.map((action) {
                      return ActionChip(
                        label: Text(action.title),
                        onPressed: () => controller.send(action.title),
                      );
                    }).toList(),
                  ),
                  if (state.showsCrisisCard) ...[
                    const SizedBox(height: 16),
                    const CrisisBanner(),
                  ],
                  const SizedBox(height: 16),
                  if (state.messages.isEmpty)
                    _MessageBubble(
                      ChatMessage(
                        id: 'intro',
                        role: ChatRole.assistant,
                        text: 'I am here with you. Tell me what feels heaviest, and we will look for one calmer next step.',
                        timestamp: DateTime.now(),
                      ),
                    ),
                  ...state.messages.map((message) => _MessageBubble(message)),
                ],
              ),
            ),
            Padding(
              padding: const EdgeInsets.fromLTRB(16, 8, 16, 16),
              child: Row(
                children: [
                  Expanded(
                    child: TextField(
                      controller: textController,
                      minLines: 1,
                      maxLines: 4,
                      textInputAction: TextInputAction.send,
                      onChanged: controller.setDraft,
                      onSubmitted: controller.send,
                      decoration: const InputDecoration(
                        hintText: 'Tell me what feels heaviest right now',
                        border: OutlineInputBorder(),
                      ),
                    ),
                  ),
                  const SizedBox(width: 10),
                  IconButton.filled(
                    tooltip: 'Send',
                    onPressed: () {
                      controller.send(textController.text);
                      FocusScope.of(context).unfocus();
                    },
                    icon: const Icon(Icons.arrow_upward),
                  ),
                ],
              ),
            ),
            Padding(
              padding: const EdgeInsets.only(bottom: 8),
              child: Text(
                'Still Here Hope is not emergency or medical care.',
                style: Theme.of(context).textTheme.bodySmall,
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class _MessageBubble extends StatelessWidget {
  const _MessageBubble(this.message);
  final ChatMessage message;

  @override
  Widget build(BuildContext context) {
    final isUser = message.role == ChatRole.user;
    return Align(
      alignment: isUser ? Alignment.centerRight : Alignment.centerLeft,
      child: ConstrainedBox(
        constraints: const BoxConstraints(maxWidth: 560),
        child: Card(
          color: isUser ? Theme.of(context).colorScheme.primaryContainer : null,
          child: Padding(
            padding: const EdgeInsets.all(16),
            child: Text(message.text),
          ),
        ),
      ),
    );
  }
}
