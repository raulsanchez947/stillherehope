import '../../../features/notes/domain/hope_note.dart';

class SupportResponseEngine {
  CheckInPresentation makeCheckIn({
    required MoodType mood,
    required String note,
    required HopeNote? sharedNote,
  }) {
    return CheckInPresentation(
      mood: mood,
      supportiveLine: _supportiveLineForMood(mood),
      suggestedAction: _actionForMood(mood, note),
      groundingPrompt: _groundingForMood(mood),
      sharedNote: sharedNote ?? _fallbackNoteForMood(mood),
    );
  }

  CheckInPresentation quickAction(QuickAction action, HopeNote? sharedNote) {
    final mood = _moodForAction(action);
    return CheckInPresentation(
      mood: mood,
      supportiveLine: _supportiveLineForAction(action),
      suggestedAction: _actionForQuickAction(action),
      groundingPrompt: _regulationForAction(action),
      sharedNote: sharedNote ?? _fallbackNoteForMood(mood),
    );
  }

  String localChatReply(String text) {
    final lower = text.toLowerCase();
    if (lower.contains('next hour')) {
      return _compose(QuickAction.nextHour);
    }
    if (lower.contains('ground') || lower.contains('panic')) {
      return _compose(QuickAction.groundMe);
    }
    if (lower.contains('slow')) {
      return _compose(QuickAction.slowDown);
    }
    if (lower.contains('alone') || lower.contains('lonely')) {
      return _compose(QuickAction.alone);
    }
    if (lower.contains('anxious')) {
      return _compose(QuickAction.anxious);
    }
    if (lower.contains('overwhelmed') || lower.contains('too much')) {
      return _compose(QuickAction.overwhelmed);
    }
    return _limit(
      'Thank you for saying it plainly. Name the heaviest piece, then choose one small act of care for the next ten minutes. Breathe out longer than in.',
    );
  }

  String safetyEscalationResponse() {
    return _limit(
      'I am really glad you said that. If you might hurt yourself or cannot stay safe, call or text 988 now in the U.S., or contact emergency services where you are. Move closer to other people if you can.',
    );
  }

  String _compose(QuickAction action) {
    return _limit(
      '${_supportiveLineForAction(action)} ${_actionForQuickAction(action)} ${_regulationForAction(action)}',
    );
  }

  String _supportiveLineForMood(MoodType mood) => switch (mood) {
        MoodType.overwhelmed => 'You do not need to carry the whole pile at once.',
        MoodType.anxious => 'Your body may be loud right now, but you can still move gently.',
        MoodType.low => 'You do not need big hope to care for this hour.',
        MoodType.angry => 'Anger can be real without choosing the next move for you.',
        MoodType.numb => 'Numb does not mean gone. It may mean overloaded.',
        MoodType.calm => 'This steadier moment is worth protecting.',
      };

  String _supportiveLineForAction(QuickAction action) => switch (action) {
        QuickAction.groundMe => 'Let us help the room feel real again.',
        QuickAction.slowDown => 'You are allowed to go slower than the pressure says.',
        QuickAction.alone => 'Feeling alone can make everything sound harsher.',
        QuickAction.nextHour => 'You only need a softer plan for the next hour.',
        QuickAction.overwhelmed => _supportiveLineForMood(MoodType.overwhelmed),
        QuickAction.anxious => _supportiveLineForMood(MoodType.anxious),
      };

  String _actionForMood(MoodType mood, String note) {
    final lower = note.toLowerCase();
    if (lower.contains('work')) return 'Pick one work task to delay and one to finish imperfectly.';
    if (lower.contains('sleep') || lower.contains('tired')) {
      return 'Choose relief before productivity: water, dim light, and a quieter next hour.';
    }
    return switch (mood) {
      MoodType.overwhelmed => 'Put both feet down and choose one thing that can wait.',
      MoodType.anxious => 'Unclench your jaw, drop your shoulders, and sip something cool or warm.',
      MoodType.low => 'Open the curtain, drink water, and finish one tiny task.',
      MoodType.angry => 'Pause the reply and write one sentence about what feels crossed.',
      MoodType.numb => 'Try cool water on your hands or step outside for one minute.',
      MoodType.calm => 'Choose one kind habit that protects this steadier mood.',
    };
  }

  String _actionForQuickAction(QuickAction action) => switch (action) {
        QuickAction.groundMe => 'Name three colors, two sounds, and one thing touching your skin.',
        QuickAction.slowDown => 'Pause one task and do one gentler thing before you continue.',
        QuickAction.alone => 'Send one low-pressure text or sit somewhere other people are nearby.',
        QuickAction.nextHour => 'Pick one anchor: water, food, shower, outside air, or one safe person.',
        QuickAction.overwhelmed => 'Pause one task and do one gentler thing before you continue.',
        QuickAction.anxious => 'Name three colors, two sounds, and one thing touching your skin.',
      };

  String _groundingForMood(MoodType mood) => switch (mood) {
        MoodType.overwhelmed => 'Inhale for 4, exhale for 6, three times.',
        MoodType.anxious => 'Press your feet down and notice one sound in the room.',
        MoodType.low => 'Take three slower breaths and let your shoulders soften.',
        MoodType.angry => 'Exhale twice as long as you inhale for three rounds.',
        MoodType.numb => 'Name one texture, one temperature, and one sound.',
        MoodType.calm => 'Take one slow breath to help this steadiness stay.',
      };

  String _regulationForAction(QuickAction action) => switch (action) {
        QuickAction.groundMe => 'Breathe out longer than you breathe in.',
        QuickAction.slowDown => 'Put one hand on your chest and count five slow exhales.',
        QuickAction.alone => 'Say: I do not need to carry this without contact forever.',
        QuickAction.nextHour => 'Ask: what would make the next 60 minutes 5% easier?',
        QuickAction.overwhelmed => 'Inhale for 4, exhale for 6, three times.',
        QuickAction.anxious => 'Breathe in for 4, hold for 2, out for 6.',
      };

  MoodType _moodForAction(QuickAction action) => switch (action) {
        QuickAction.groundMe || QuickAction.anxious => MoodType.anxious,
        QuickAction.alone => MoodType.low,
        _ => MoodType.overwhelmed,
      };

  HopeNote _fallbackNoteForMood(MoodType mood) {
    final text = switch (mood) {
      MoodType.overwhelmed => 'You can let one thing be unfinished and still be doing enough.',
      MoodType.anxious => 'A slower exhale counts as progress.',
      MoodType.low => 'Staying for this hour is already a real thing.',
      MoodType.angry => 'You can pause before giving the feeling the microphone.',
      MoodType.numb => 'Small sensation is still sensation.',
      MoodType.calm => 'Protecting a steadier moment is its own kind of care.',
    };
    return HopeNote(
      id: 'fallback-${mood.name}',
      text: text,
      createdAt: DateTime.now(),
      tags: [mood],
      helpedCount: 0,
      isSaved: false,
      anonymousUserId: 'system.fallback',
      moderationStatus: ModerationStatus.visible,
    );
  }

  String _limit(String text) {
    final words = text.split(RegExp('\\s+'));
    if (words.length <= 90) return text;
    return '${words.take(90).join(' ')}...';
  }
}
