import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../services/id_service.dart';
import '../../services/safety_classifier.dart';
import '../notes/domain/hope_note.dart';
import 'ai_chat_providers.dart';

final talkControllerProvider =
    StateNotifierProvider<TalkController, TalkState>((ref) {
  return TalkController(ref);
});

class TalkState {
  const TalkState({
    this.draft = '',
    this.messages = const [],
    this.showsCrisisCard = false,
  });

  final String draft;
  final List<ChatMessage> messages;
  final bool showsCrisisCard;

  TalkState copyWith({
    String? draft,
    List<ChatMessage>? messages,
    bool? showsCrisisCard,
  }) {
    return TalkState(
      draft: draft ?? this.draft,
      messages: messages ?? this.messages,
      showsCrisisCard: showsCrisisCard ?? this.showsCrisisCard,
    );
  }
}

class TalkController extends StateNotifier<TalkState> {
  TalkController(this.ref) : super(const TalkState());
  final Ref ref;
  final _safety = SafetyClassifier();

  void setDraft(String value) => state = state.copyWith(draft: value);

  void send(String text) {
    final trimmed = text.trim();
    if (trimmed.isEmpty) return;
    final engine = ref.read(supportResponseEngineProvider);
    final assessment = _safety.assess(trimmed);
    final user = ChatMessage(
      id: createLocalId(),
      role: ChatRole.user,
      text: trimmed,
      timestamp: DateTime.now(),
    );
    final assistant = ChatMessage(
      id: createLocalId(),
      role: ChatRole.assistant,
      text: assessment.shouldEscalate
          ? engine.safetyEscalationResponse()
          : engine.localChatReply(trimmed),
      timestamp: DateTime.now(),
    );
    state = state.copyWith(
      draft: '',
      showsCrisisCard: assessment.shouldEscalate,
      messages: [...state.messages, user, assistant],
    );
  }
}
