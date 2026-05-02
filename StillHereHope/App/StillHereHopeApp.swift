import SwiftUI

@main
struct StillHereHopeApp: App {
    @AppStorage("hasCompletedStillHereHopeOnboarding")
    private var hasCompletedOnboarding = false

    @StateObject
    private var dataStoreProvider = HopeDataStoreProvider()

    private let supportEngine = SupportResponseEngine()
    private let safetyClassifier = SafetyClassifier()

    var body: some Scene {
        WindowGroup {
            rootView
            .background(AppTheme.Colors.background.ignoresSafeArea())
        }
    }

    @ViewBuilder
    private var rootView: some View {
#if DEBUG
        if DeveloperPreviewMode.isEnabled {
            DeveloperPreviewGalleryView()
        } else {
            mainFlow
        }
#else
        mainFlow
#endif
    }

    @ViewBuilder
    private var mainFlow: some View {
        if hasCompletedOnboarding {
            AppRootView(
                repository: dataStoreProvider.store,
                supportEngine: supportEngine,
                safetyClassifier: safetyClassifier
            )
        } else {
            OnboardingView {
                hasCompletedOnboarding = true
            }
        }
    }
}
