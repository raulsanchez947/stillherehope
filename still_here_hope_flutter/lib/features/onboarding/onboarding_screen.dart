import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../views/components/calm_components.dart';
import 'onboarding_controller.dart';

class OnboardingScreen extends ConsumerStatefulWidget {
  const OnboardingScreen({super.key});

  @override
  ConsumerState<OnboardingScreen> createState() => _OnboardingScreenState();
}

class _OnboardingScreenState extends ConsumerState<OnboardingScreen> {
  bool terms = false;
  bool guidelines = false;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: CalmScreen(
        title: 'Still Here Hope',
        subtitle: 'A calm space to check in, slow down, and find one gentler next step.',
        children: [
          const CalmCard(
            child: Text('Still Here Hope is not emergency, medical, crisis, diagnosis, or therapy care.'),
          ),
          const SizedBox(height: 16),
          CalmCard(
            child: Column(
              children: [
                SwitchListTile.adaptive(
                  title: const Text('I agree to the Terms'),
                  value: terms,
                  onChanged: (value) => setState(() => terms = value),
                ),
                SwitchListTile.adaptive(
                  title: const Text('I agree to the Community Guidelines'),
                  value: guidelines,
                  onChanged: (value) => setState(() => guidelines = value),
                ),
              ],
            ),
          ),
          const SizedBox(height: 20),
          FilledButton(
            onPressed: terms && guidelines
                ? () => ref.read(onboardingControllerProvider).accept()
                : null,
            child: const Text('Continue'),
          ),
        ],
      ),
    );
  }
}
