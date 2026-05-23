import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:go_router/go_router.dart';

import '../features/app_shell/app_shell_screen.dart';
import '../features/auth/auth_controller.dart';
import '../features/crisis/crisis_resources_screen.dart';
import '../features/moderation/report_note_screen.dart';
import '../features/notes/domain/hope_note.dart';
import '../features/onboarding/onboarding_controller.dart';
import '../features/onboarding/onboarding_screen.dart';
import '../features/settings/settings_screen.dart';

final rootNavigatorKey = GlobalKey<NavigatorState>();

final appRouterProvider = Provider<GoRouter>((ref) {
  final onboardingAccepted = ref.watch(onboardingAcceptedProvider);
  final authState = ref.watch(authControllerProvider);

  return GoRouter(
    navigatorKey: rootNavigatorKey,
    initialLocation: '/',
    refreshListenable: RouterRefreshNotifier(ref),
    redirect: (context, state) {
      final accepted = onboardingAccepted.valueOrNull ?? false;
      final authed = authState.valueOrNull != null;
      final isOnboarding = state.matchedLocation == '/onboarding';

      if (!accepted && !isOnboarding) return '/onboarding';
      if (accepted && isOnboarding) return '/';
      if (accepted && !authed) return null;
      return null;
    },
    routes: [
      GoRoute(
        path: '/',
        builder: (context, state) => const AppShellScreen(),
      ),
      GoRoute(
        path: '/onboarding',
        builder: (context, state) => const OnboardingScreen(),
      ),
      GoRoute(
        path: '/settings',
        builder: (context, state) => const SettingsScreen(),
      ),
      GoRoute(
        path: '/crisis',
        builder: (context, state) => const CrisisResourcesScreen(),
      ),
      GoRoute(
        path: '/report',
        builder: (context, state) {
          final note = state.extra as HopeNote;
          return ReportNoteScreen(note: note);
        },
      ),
    ],
  );
});

class RouterRefreshNotifier extends ChangeNotifier {
  RouterRefreshNotifier(this.ref) {
    ref.listen(onboardingAcceptedProvider, (_, __) => notifyListeners());
    ref.listen(authControllerProvider, (_, __) => notifyListeners());
  }

  final Ref ref;
}
