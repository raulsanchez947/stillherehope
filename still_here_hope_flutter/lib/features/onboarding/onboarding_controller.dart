import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:hive/hive.dart';

import '../../core/persistence/hive_boxes.dart';

final onboardingAcceptedProvider = FutureProvider<bool>((ref) async {
  return Hive.box<String>(HiveBoxes.app).get('acceptedTerms') == 'true';
});

final onboardingControllerProvider = Provider<OnboardingController>((ref) {
  return OnboardingController(ref);
});

class OnboardingController {
  OnboardingController(this.ref);
  final Ref ref;

  Future<void> accept() async {
    await Hive.box<String>(HiveBoxes.app).put('acceptedTerms', 'true');
    ref.invalidate(onboardingAcceptedProvider);
  }
}
