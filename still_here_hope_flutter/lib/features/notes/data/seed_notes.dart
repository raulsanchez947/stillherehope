import '../domain/hope_note.dart';
import '../../../services/id_service.dart';

List<HopeNote> seedHopeNotes() {
  HopeNote note(String text, List<MoodType> tags, int helped) => HopeNote(
        id: createLocalId(),
        text: text,
        createdAt: DateTime.now(),
        tags: tags,
        helpedCount: helped,
        isSaved: false,
        anonymousUserId: 'seed.${tags.first.name}.$helped',
        moderationStatus: ModerationStatus.visible,
      );

  return [
    note('You do not need to solve your whole life tonight. Make the next five minutes softer.', [MoodType.overwhelmed, MoodType.anxious], 18),
    note('If all you can do today is stay, breathe, and try again later, that still counts.', [MoodType.low, MoodType.numb], 25),
    note('A slower exhale counts as progress.', [MoodType.anxious], 12),
    note('Try not to ask tomorrow questions with tonight tired mind.', [MoodType.low, MoodType.overwhelmed], 9),
    note('You can pause before answering. The feeling does not need the steering wheel.', [MoodType.angry], 7),
  ];
}
