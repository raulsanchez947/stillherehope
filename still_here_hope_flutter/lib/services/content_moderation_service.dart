import '../models/app_models.dart';

sealed class ModerationReviewState {
  const ModerationReviewState();
}

class ModerationClear extends ModerationReviewState {
  const ModerationClear();
}

class ModerationPendingReview extends ModerationReviewState {
  const ModerationPendingReview(this.category, this.message);
  final ReportCategory category;
  final String message;
}

class ModerationBlocked extends ModerationReviewState {
  const ModerationBlocked(this.category, this.message);
  final ReportCategory category;
  final String message;
}

class ContentModerationService {
  bool isFlagged(String text) => evaluate(text) is ModerationBlocked;

  ModerationReviewState evaluate(String text) {
    final normalized = _normalize(text);
    if (_matches(normalized, const [
      'kill yourself',
      'go kill yourself',
      'you should die',
      'people like you should die',
      'i will kill you',
      'i am going to kill you',
      'burn in hell',
    ])) {
      return const ModerationBlocked(
        ReportCategory.encouragesSelfHarm,
        "This message can't be posted.",
      );
    }

    if (_matches(normalized, const [
      'subhuman',
      'dirty immigrant',
      'fag',
      'kike',
      'nigger',
      'spic',
      'tranny',
    ])) {
      return const ModerationBlocked(
        ReportCategory.hateSpeech,
        "This message can't be posted.",
      );
    }

    if (_matches(normalized, const [
      'nobody wants you',
      'you are pathetic',
      "you're pathetic",
      'everyone hates you',
      'you deserve pain',
      'end it already',
      'just disappear',
    ])) {
      return const ModerationPendingReview(
        ReportCategory.harassmentOrAbuse,
        'That note needs review before it can be shared.',
      );
    }

    if (_matches(normalized, const [
      'buy now',
      'click this link',
      'dm me for',
      'make money fast',
    ])) {
      return const ModerationPendingReview(
        ReportCategory.spam,
        'That note is being held for review.',
      );
    }

    return const ModerationClear();
  }

  String _normalize(String text) => text
      .toLowerCase()
      .replaceAll(RegExp('[^a-z0-9\\s]'), ' ')
      .replaceAll(RegExp('\\s+'), ' ')
      .trim();

  bool _matches(String text, List<String> phrases) {
    return phrases.any(text.contains);
  }
}
