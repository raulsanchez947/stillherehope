import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../services/id_service.dart';
import '../ai_chat/ai_chat_providers.dart';
import '../notes/data/hope_repository.dart';
import '../notes/domain/hope_note.dart';

final homeControllerProvider =
    StateNotifierProvider<HomeController, HomeState>((ref) {
  return HomeController(ref);
});

class HomeState {
  const HomeState({
    this.selectedMood,
    this.heaviness = '',
    this.presentation,
    this.savedMessage,
  });

  final MoodType? selectedMood;
  final String heaviness;
  final CheckInPresentation? presentation;
  final String? savedMessage;

  HomeState copyWith({
    MoodType? selectedMood,
    String? heaviness,
    CheckInPresentation? presentation,
    String? savedMessage,
  }) {
    return HomeState(
      selectedMood: selectedMood ?? this.selectedMood,
      heaviness: heaviness ?? this.heaviness,
      presentation: presentation ?? this.presentation,
      savedMessage: savedMessage,
    );
  }
}

class HomeController extends StateNotifier<HomeState> {
  HomeController(this.ref) : super(const HomeState());
  final Ref ref;

  void selectMood(MoodType mood) {
    final repo = ref.read(hopeRepositoryProvider);
    final engine = ref.read(supportResponseEngineProvider);
    state = state.copyWith(
      selectedMood: mood,
      presentation: engine.makeCheckIn(
        mood: mood,
        note: state.heaviness,
        sharedNote: repo.noteForMood(mood),
      ),
      savedMessage: null,
    );
  }

  void setHeaviness(String text) {
    state = state.copyWith(heaviness: text);
    final mood = state.selectedMood;
    if (mood != null) selectMood(mood);
  }

  void quickAction(QuickAction action) {
    final repo = ref.read(hopeRepositoryProvider);
    final engine = ref.read(supportResponseEngineProvider);
    state = state.copyWith(
      presentation: engine.quickAction(action, repo.noteForMood(state.selectedMood)),
      savedMessage: null,
    );
  }

  Future<void> saveCheckIn() async {
    final presentation = state.presentation;
    if (presentation == null) return;
    await ref.read(hopeRepositoryProvider).saveCheckIn(
          MoodCheckIn(
            id: createLocalId(),
            date: DateTime.now(),
            mood: presentation.mood,
            note: state.heaviness,
            suggestedAction: presentation.suggestedAction,
          ),
        );
    state = state.copyWith(savedMessage: 'Saved for today.');
  }
}
