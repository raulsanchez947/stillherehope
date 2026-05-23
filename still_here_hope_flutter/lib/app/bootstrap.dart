import 'package:firebase_core/firebase_core.dart';
import 'package:hive_flutter/hive_flutter.dart';

import '../core/firebase/firebase_runtime.dart';
import '../core/persistence/hive_boxes.dart';

Future<void> bootstrapApp() async {
  await Hive.initFlutter();
  await HiveBoxes.open();
  try {
    await Firebase.initializeApp();
    FirebaseRuntime.isConfigured = true;
  } catch (_) {
    FirebaseRuntime.isConfigured = false;
  }
}
