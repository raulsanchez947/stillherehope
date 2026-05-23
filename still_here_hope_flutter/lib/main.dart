import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'app/bootstrap.dart';
import 'app/still_here_hope_app.dart';
import 'core/observability/crash_reporting_service.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await bootstrapApp();

  FlutterError.onError = (details) {
    FlutterError.presentError(details);
    CrashReportingService.instance.recordFlutterError(details);
  };

  PlatformDispatcher.instance.onError = (error, stack) {
    CrashReportingService.instance.recordError(error, stack);
    return true;
  };

  runApp(const ProviderScope(child: StillHereHopeApp()));
}
