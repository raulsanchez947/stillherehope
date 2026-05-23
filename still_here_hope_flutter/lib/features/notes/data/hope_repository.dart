import 'dart:convert';

import 'package:flutter/foundation.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:hive/hive.dart';

import '../../../core/persistence/hive_boxes.dart';
import '../../../services/id_service.dart';
import '../../auth/auth_controller.dart';
import '../../blocking/blocked_user_manager.dart';
import '../domain/hope_note.dart';
import 'seed_notes.dart';

final blockedUserManagerProvider = Provider((ref) => BlockedUserManager());

final hopeRepositoryProvider = ChangeNotifierProvider<HopeRepository>((ref) {
  return HopeRepository(
    anonymousUserId: ref.watch(anonymousUserIdProvider),
    blockedUserManager: ref.watch(blockedUserManagerProvider),
  )..load();
});

class HopeRepository extends ChangeNotifier {
  HopeRepository({
    required this.anonymousUserId,
    required this.blockedUserManager,
  });

  final String anonymousUserId;
  final BlockedUserManager blockedUserManager;

  final _notesBox = Hive.box<String>(HiveBoxes.notes);
  final _checkInsBox = Hive.box<String>(HiveBoxes.checkIns);
  final _reportsBox = Hive.box<String>(HiveBoxes.reports);
  final _flaggedBox = Hive.box<String>(HiveBoxes.flagged);

  List<HopeNote> notes = [];
  List<MoodCheckIn> checkIns = [];
  List<NoteReport> reports = [];
  List<FlaggedNote> flaggedNotes = [];

  List<HopeNote> get visibleNotes => notes
      .where((note) => !note.isHidden)
      .where((note) => !blockedUserManager.isBlocked(note.anonymousUserId))
      .toList();

  List<HopeNote> get savedNotes => visibleNotes.where((note) => note.isSaved).toList();

  UserStats get stats => UserStats(
        streakCount: _streakCount(),
        totalSavedNotes: savedNotes.length,
        totalHelpfulNotes: notes
            .where((note) => note.anonymousUserId == anonymousUserId)
            .fold(0, (sum, note) => sum + note.helpedCount),
      );

  void load() {
    notes = _loadNotes();
    checkIns = _loadCheckIns();
    reports = _loadReports();
    flaggedNotes = _loadFlaggedNotes();
    notifyListeners();
  }

  HopeNote? noteForMood(MoodType? mood) {
    final pool = mood == null
        ? visibleNotes
        : visibleNotes.where((note) => note.tags.contains(mood)).toList();
    final sorted = [...(pool.isEmpty ? visibleNotes : pool)]
      ..sort((a, b) => b.helpedCount.compareTo(a.helpedCount));
    return sorted.isEmpty ? null : sorted.first;
  }

  Future<void> addNote(String text, List<MoodType> tags, ModerationStatus status) async {
    notes.insert(
      0,
      HopeNote(
        id: createLocalId(),
        text: text,
        createdAt: DateTime.now(),
        tags: tags,
        helpedCount: 0,
        isSaved: false,
        anonymousUserId: anonymousUserId,
        moderationStatus: status,
      ),
    );
    await _saveNotes();
  }

  Future<void> saveCheckIn(MoodCheckIn checkIn) async {
    checkIns.insert(0, checkIn);
    await _checkInsBox.put('items', jsonEncode(checkIns.map((c) => c.toJson()).toList()));
    notifyListeners();
  }

  Future<void> toggleSaved(String noteId) async {
    final note = notes.firstWhere((note) => note.id == noteId);
    note.isSaved = !note.isSaved;
    await _saveNotes();
  }

  Future<void> markHelpful(String noteId) async {
    final note = notes.firstWhere((note) => note.id == noteId);
    note.helpedCount += 1;
    await _saveNotes();
  }

  Future<void> report(String noteId, ReportCategory reason, {bool blockSource = false}) async {
    final note = notes.firstWhere((note) => note.id == noteId);
    note.moderationStatus = ModerationStatus.underReview;
    reports.insert(
      0,
      NoteReport(
        id: createLocalId(),
        noteId: note.id,
        reportReason: reason,
        timestamp: DateTime.now(),
        reporterUserId: anonymousUserId,
        reportedUserId: note.anonymousUserId,
      ),
    );
    flaggedNotes.insert(
      0,
      FlaggedNote(
        id: createLocalId(),
        noteId: note.id,
        content: note.text,
        reportReason: reason,
        timestamp: DateTime.now(),
        userId: note.anonymousUserId,
      ),
    );
    if (blockSource) {
      await blockedUserManager.block(note.anonymousUserId);
    }
    await _saveNotes();
    await _reportsBox.put('items', jsonEncode(reports.map((r) => r.toJson()).toList()));
    await _flaggedBox.put('items', jsonEncode(flaggedNotes.map((f) => f.toJson()).toList()));
  }

  Future<void> blockSource(String anonymousSourceId) async {
    await blockedUserManager.block(anonymousSourceId);
    notifyListeners();
  }

  Future<void> _saveNotes() async {
    await _notesBox.put('items', jsonEncode(notes.map((n) => n.toJson()).toList()));
    notifyListeners();
  }

  List<HopeNote> _loadNotes() {
    final encoded = _notesBox.get('items');
    if (encoded == null) return seedHopeNotes();
    return (jsonDecode(encoded) as List<dynamic>)
        .map((json) => HopeNote.fromJson(json as Map<String, dynamic>))
        .toList();
  }

  List<MoodCheckIn> _loadCheckIns() {
    final encoded = _checkInsBox.get('items');
    if (encoded == null) return [];
    return (jsonDecode(encoded) as List<dynamic>)
        .map((json) => MoodCheckIn.fromJson(json as Map<String, dynamic>))
        .toList();
  }

  List<NoteReport> _loadReports() {
    final encoded = _reportsBox.get('items');
    if (encoded == null) return [];
    return (jsonDecode(encoded) as List<dynamic>)
        .map((json) => NoteReport.fromJson(json as Map<String, dynamic>))
        .toList();
  }

  List<FlaggedNote> _loadFlaggedNotes() {
    final encoded = _flaggedBox.get('items');
    if (encoded == null) return [];
    return (jsonDecode(encoded) as List<dynamic>)
        .map((json) => FlaggedNote.fromJson(json as Map<String, dynamic>))
        .toList();
  }

  int _streakCount() {
    if (checkIns.isEmpty) return 0;
    final days = checkIns
        .map((c) => DateTime(c.date.year, c.date.month, c.date.day))
        .toSet();
    var cursor = DateTime.now();
    var count = 0;
    while (days.contains(DateTime(cursor.year, cursor.month, cursor.day))) {
      count++;
      cursor = cursor.subtract(const Duration(days: 1));
    }
    return count;
  }
}
