import SwiftUI

struct AppRootView: View {
    let repository: any HopeDataStore
    let supportEngine: SupportResponseEngine
    let safetyClassifier: SafetyClassifier

    var body: some View {
        TabView {
            NavigationStack {
                HomeView(viewModel: HomeViewModel(repository: repository, supportEngine: supportEngine))
            }
            .tabItem { Label("Home", systemImage: "house") }

            NavigationStack {
                TalkView(viewModel: TalkViewModel(supportEngine: supportEngine, safetyClassifier: safetyClassifier))
            }
            .tabItem { Label("Talk", systemImage: "message") }

            NavigationStack {
                NotesFeedView(
                    viewModel: NotesFeedViewModel(repository: repository),
                    writeNoteViewModel: WriteNoteViewModel(repository: repository)
                )
            }
            .tabItem { Label("Notes", systemImage: "heart.text.square") }

            NavigationStack {
                YouView(viewModel: YouViewModel(repository: repository))
            }
            .tabItem { Label("You", systemImage: "person") }
        }
        .tint(AppTheme.Colors.accent)
        .background(AppTheme.Colors.background)
    }
}

#Preview("App Root") {
    AppRootView(
        repository: HopeRepository(),
        supportEngine: SupportResponseEngine(),
        safetyClassifier: SafetyClassifier()
    )
}
