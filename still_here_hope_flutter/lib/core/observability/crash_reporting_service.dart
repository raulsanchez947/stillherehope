import 'package:firebase_crashlytics/firebase_crashlytics.dart';
import 'package:flutter/foundation.dart';

import '../firebase/firebase_runtime.dart';

class CrashReportingService {
  CrashReportingService._();
  static final instance = CrashReportingService._();

  Future<void> recordFlutterError(FlutterErrorDetails details) {
    if (!FirebaseRuntime.isConfigured) return Future.value();
    return FirebaseCrashlytics.instance.recordFlutterFatalError(details);
  }

  Future<void> recordError(Object error, StackTrace stack) {
    if (!FirebaseRuntime.isConfigured) return Future.value();
    return FirebaseCrashlytics.instance.recordError(error, stack, fatal: true);
  }
}
