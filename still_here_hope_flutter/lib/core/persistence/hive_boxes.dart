import 'package:hive_flutter/hive_flutter.dart';

class HiveBoxes {
  static const app = 'app';
  static const notes = 'notes';
  static const checkIns = 'checkIns';
  static const reports = 'reports';
  static const flagged = 'flagged';
  static const blocked = 'blocked';
  static const chat = 'chat';

  static Future<void> open() async {
    await Future.wait([
      Hive.openBox<String>(app),
      Hive.openBox<String>(notes),
      Hive.openBox<String>(checkIns),
      Hive.openBox<String>(reports),
      Hive.openBox<String>(flagged),
      Hive.openBox<String>(blocked),
      Hive.openBox<String>(chat),
    ]);
  }
}
