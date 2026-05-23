import 'dart:convert';

enum MoodType { overwhelmed, anxious, low, angry, numb, calm }

extension MoodTypeInfo on MoodType {
  String get title => switch (this) {
        MoodType.overwhelmed => 'Overwhelmed',
        MoodType.anxious => 'Anxious',
        MoodType.low => 'Low',
        MoodType.angry => 'Angry',
        MoodType.numb => 'Numb',
        MoodType.calm => 'Calm',
      };

  String get key => name;
}

enum QuickAction { groundMe, slowDown, alone, nextHour, overwhelmed, anxious }

extension QuickActionInfo on QuickAction {
  String get title => switch (this) {
        QuickAction.groundMe => 'Ground me',
        QuickAction.slowDown => 'Help me slow down',
        QuickAction.alone => 'I feel alone',
        QuickAction.nextHour => 'Get me through the next hour',
        QuickAction.overwhelmed => "I'm overwhelmed",
        QuickAction.anxious => 'I feel anxious',
      };
}

enum ChatRole { user, assistant }
enum ModerationStatus { visible, underReview }
enum ReportCategory {
  harmfulOrTriggering,
  harassmentOrAbuse,
  encouragesSelfHarm,
  hateSpeech,
  spam,
}

extension ReportCategoryInfo on ReportCategory {
  String get title => switch (this) {
        ReportCategory.harmfulOrTriggering => 'Harmful or triggering',
        ReportCategory.harassmentOrAbuse => 'Harassment or abuse',
        ReportCategory.encouragesSelfHarm => 'Encourages self-harm',
        ReportCategory.hateSpeech => 'Hate speech',
        ReportCategory.spam => 'Spam',
      };
}

class MoodCheckIn {
  MoodCheckIn({
    required this.id,
    required this.date,
    required this.mood,
    required this.note,
    required this.suggestedAction,
  });

  final String id;
  final DateTime date;
  final MoodType mood;
  final String note;
  final String suggestedAction;

  Map<String, dynamic> toJson() => {
        'id': id,
        'date': date.toIso8601String(),
        'mood': mood.name,
        'note': note,
        'suggestedAction': suggestedAction,
      };

  factory MoodCheckIn.fromJson(Map<String, dynamic> json) => MoodCheckIn(
        id: json['id'] as String,
        date: DateTime.parse(json['date'] as String),
        mood: MoodType.values.byName(json['mood'] as String),
        note: json['note'] as String,
        suggestedAction: json['suggestedAction'] as String,
      );
}

class HopeNote {
  HopeNote({
    required this.id,
    required this.text,
    required this.createdAt,
    required this.tags,
    required this.helpedCount,
    required this.isSaved,
    required this.anonymousUserId,
    required this.moderationStatus,
  });

  final String id;
  String text;
  DateTime createdAt;
  List<MoodType> tags;
  int helpedCount;
  bool isSaved;
  String anonymousUserId;
  ModerationStatus moderationStatus;

  bool get isHidden => moderationStatus != ModerationStatus.visible;

  Map<String, dynamic> toJson() => {
        'id': id,
        'text': text,
        'createdAt': createdAt.toIso8601String(),
        'tags': tags.map((tag) => tag.name).toList(),
        'helpedCount': helpedCount,
        'isSaved': isSaved,
        'anonymousUserId': anonymousUserId,
        'moderationStatus': moderationStatus.name,
      };

  factory HopeNote.fromJson(Map<String, dynamic> json) => HopeNote(
        id: json['id'] as String,
        text: json['text'] as String,
        createdAt: DateTime.parse(json['createdAt'] as String),
        tags: (json['tags'] as List<dynamic>)
            .map((tag) => MoodType.values.byName(tag as String))
            .toList(),
        helpedCount: json['helpedCount'] as int,
        isSaved: json['isSaved'] as bool,
        anonymousUserId:
            (json['anonymousUserId'] ?? json['sourceUserId'] ?? 'legacy.local')
                as String,
        moderationStatus: ModerationStatus.values.byName(
          (json['moderationStatus'] ?? ModerationStatus.visible.name) as String,
        ),
      );
}

class ChatMessage {
  ChatMessage({
    required this.id,
    required this.role,
    required this.text,
    required this.timestamp,
  });

  final String id;
  final ChatRole role;
  final String text;
  final DateTime timestamp;
}

class NoteReport {
  NoteReport({
    required this.id,
    required this.noteId,
    required this.reportReason,
    required this.timestamp,
    required this.reporterUserId,
    required this.reportedUserId,
  });

  final String id;
  final String noteId;
  final ReportCategory reportReason;
  final DateTime timestamp;
  final String reporterUserId;
  final String reportedUserId;

  Map<String, dynamic> toJson() => {
        'id': id,
        'noteId': noteId,
        'reportReason': reportReason.name,
        'timestamp': timestamp.toIso8601String(),
        'reporterUserId': reporterUserId,
        'reportedUserId': reportedUserId,
      };

  factory NoteReport.fromJson(Map<String, dynamic> json) => NoteReport(
        id: json['id'] as String,
        noteId: json['noteId'] as String,
        reportReason: ReportCategory.values.byName(json['reportReason'] as String),
        timestamp: DateTime.parse(json['timestamp'] as String),
        reporterUserId: json['reporterUserId'] as String,
        reportedUserId: json['reportedUserId'] as String,
      );
}

class FlaggedNote {
  FlaggedNote({
    required this.id,
    required this.noteId,
    required this.content,
    required this.reportReason,
    required this.timestamp,
    required this.userId,
  });

  final String id;
  final String? noteId;
  final String content;
  final ReportCategory reportReason;
  final DateTime timestamp;
  final String userId;

  Map<String, dynamic> toJson() => {
        'id': id,
        'noteId': noteId,
        'content': content,
        'reportReason': reportReason.name,
        'timestamp': timestamp.toIso8601String(),
        'userId': userId,
      };

  factory FlaggedNote.fromJson(Map<String, dynamic> json) => FlaggedNote(
        id: json['id'] as String,
        noteId: json['noteId'] as String?,
        content: json['content'] as String,
        reportReason: ReportCategory.values.byName(json['reportReason'] as String),
        timestamp: DateTime.parse(json['timestamp'] as String),
        userId: json['userId'] as String,
      );
}

class UserStats {
  const UserStats({
    required this.streakCount,
    required this.totalSavedNotes,
    required this.totalHelpfulNotes,
  });

  final int streakCount;
  final int totalSavedNotes;
  final int totalHelpfulNotes;
}

class CheckInPresentation {
  const CheckInPresentation({
    required this.mood,
    required this.supportiveLine,
    required this.suggestedAction,
    required this.groundingPrompt,
    required this.sharedNote,
  });

  final MoodType mood;
  final String supportiveLine;
  final String suggestedAction;
  final String groundingPrompt;
  final HopeNote? sharedNote;
}

String encodeList<T>(List<T> values, Map<String, dynamic> Function(T) encode) {
  return jsonEncode(values.map(encode).toList());
}
