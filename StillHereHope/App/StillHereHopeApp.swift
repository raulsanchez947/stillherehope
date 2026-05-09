import SwiftUI

@main
struct StillHereHopeApp: App {
    @AppStorage("hasAcceptedStillHereHopeTermsAndGuidelines")
    private var hasAcceptedTermsAndGuidelines = false

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
        if hasAcceptedTermsAndGuidelines {
            AppRootView(
                repository: dataStoreProvider.store,
                supportEngine: supportEngine,
                safetyClassifier: safetyClassifier
            )
        } else {
            OnboardingView {
                hasAcceptedTermsAndGuidelines = true
            }
        }
    }
}
