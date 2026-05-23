import 'package:firebase_analytics/firebase_analytics.dart';

import '../firebase/firebase_runtime.dart';

class AnalyticsService {
  AnalyticsService._();
  static final instance = AnalyticsService._();

  final FirebaseAnalytics _analytics = FirebaseAnalytics.instance;

  Future<void> track(String name, {Map<String, Object>? parameters}) {
    if (!FirebaseRuntime.isConfigured) return Future.value();
    return _analytics.logEvent(name: name, parameters: parameters);
  }

  Future<void> setAnonymousUser(String anonymousUserId) {
    if (!FirebaseRuntime.isConfigured) return Future.value();
    return _analytics.setUserId(id: anonymousUserId);
  }
}
