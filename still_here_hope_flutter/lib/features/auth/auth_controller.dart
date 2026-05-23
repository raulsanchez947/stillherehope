import 'package:firebase_auth/firebase_auth.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:hive/hive.dart';

import '../../core/firebase/firebase_runtime.dart';
import '../../core/observability/analytics_service.dart';
import '../../core/persistence/hive_boxes.dart';
import '../../services/id_service.dart';

final anonymousUserIdProvider = Provider<String>((ref) {
  final box = Hive.box<String>(HiveBoxes.app);
  final existing = box.get('anonymousUserId');
  if (existing != null) return existing;
  final created = createLocalId();
  box.put('anonymousUserId', created);
  return created;
});

final authControllerProvider = StreamProvider<User?>((ref) async* {
  final anonymousUserId = ref.watch(anonymousUserIdProvider);
  await AnalyticsService.instance.setAnonymousUser(anonymousUserId);

  if (!FirebaseRuntime.isConfigured) {
    yield null;
    return;
  }

  if (FirebaseAuth.instance.currentUser == null) {
    await FirebaseAuth.instance.signInAnonymously();
  }

  yield* FirebaseAuth.instance.authStateChanges();
});
