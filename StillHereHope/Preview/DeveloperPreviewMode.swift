import SwiftUI

#if DEBUG
enum DeveloperPreviewMode {
    static var isEnabled: Bool {
        let processInfo = ProcessInfo.processInfo
        return processInfo.arguments.contains("--still-here-hope-preview-mode")
            || processInfo.environment["STILL_HERE_HOPE_PREVIEW_MODE"] == "1"
    }
}

enum ScreenshotState: String, CaseIterable, Identifiable {
    case onboarding
    case homeCheckIn
    case talk
    case notesFeed
    case savedNotes
    case moodHistory

    var id: String { rawValue }

    var title: String {
        switch self {
        case .onboarding: "Onboarding"
        case .homeCheckIn: "Home Check-In"
        case .talk: "Talk Screen"
        case .notesFeed: "Notes Feed"
        case .savedNotes: "Saved Notes"
        case .moodHistory: "Mood History"
        }
    }

    var subtitle: String {
        switch self {
        case .onboarding: "Calm first impression"
        case .homeCheckIn: "Supportive daily check-in state"
        case .talk: "Warm conversation with short guidance"
        case .notesFeed: "Community notes with gentle hierarchy"
        case .savedNotes: "Collected support worth keeping"
        case .moodHistory: "Quiet progress and patterns"
        }
    }

    var symbol: String {
        switch self {
        case .onboarding: "sun.horizon"
        case .homeCheckIn: "house"
        case .talk: "message"
        case .notesFeed: "heart.text.square"
        case .savedNotes: "bookmark"
        case .moodHistory: "chart.line.uptrend.xyaxis"
        }
    }

    func marketingCopy(for locale: ScreenshotLocaleVariant) -> ScreenshotMarketingCopy {
        switch (self, locale) {
        case (.onboarding, .englishUS):
            return .init(eyebrow: "Still Here Hope", headline: "A softer way through today", subheadline: "Check in, slow down, and find one gentler next step.")
        case (.onboarding, .englishUK):
            return .init(eyebrow: "Still Here Hope", headline: "A calmer way through today", subheadline: "Check in, slow down, and find one gentler next step.")
        case (.onboarding, .spanishUS):
            return .init(eyebrow: "Still Here Hope", headline: "Un espacio más suave para hoy", subheadline: "Haz una pausa, baja el ritmo y encuentra un siguiente paso más amable.")

        case (.homeCheckIn, .englishUS):
            return .init(eyebrow: "Daily Check-In", headline: "Support that meets the moment", subheadline: "Mood-based guidance that stays short, warm, and steady.")
        case (.homeCheckIn, .englishUK):
            return .init(eyebrow: "Daily Check-In", headline: "Support for the moment you are in", subheadline: "Mood-based guidance that stays short, warm, and steady.")
        case (.homeCheckIn, .spanishUS):
            return .init(eyebrow: "Chequeo diario", headline: "Apoyo para este momento", subheadline: "Guía breve y tranquila según cómo se siente tu día.")

        case (.talk, .englishUS):
            return .init(eyebrow: "Talk", headline: "A calm companion in your pocket", subheadline: "Short responses, grounding prompts, and a gentler next hour.")
        case (.talk, .englishUK):
            return .init(eyebrow: "Talk", headline: "A calm companion when things feel heavy", subheadline: "Short replies, grounding prompts, and a gentler next hour.")
        case (.talk, .spanishUS):
            return .init(eyebrow: "Habla", headline: "Compañía tranquila cuando pesa el día", subheadline: "Respuestas breves, ejercicios de calma y apoyo para la próxima hora.")

        case (.notesFeed, .englishUS):
            return .init(eyebrow: "Anonymous Notes", headline: "Words worth borrowing", subheadline: "Small reminders from people still trying, just like you.")
        case (.notesFeed, .englishUK):
            return .init(eyebrow: "Anonymous Notes", headline: "Words you can lean on", subheadline: "Small reminders from people still trying, just like you.")
        case (.notesFeed, .spanishUS):
            return .init(eyebrow: "Notas anónimas", headline: "Palabras para apoyarte", subheadline: "Mensajes breves de otras personas que también siguen adelante.")

        case (.savedNotes, .englishUS):
            return .init(eyebrow: "Saved Support", headline: "Keep what helps close", subheadline: "Save the lines you want nearby on harder days.")
        case (.savedNotes, .englishUK):
            return .init(eyebrow: "Saved Support", headline: "Keep what helps nearby", subheadline: "Save the lines you want close on harder days.")
        case (.savedNotes, .spanishUS):
            return .init(eyebrow: "Guardado", headline: "Guarda lo que sí ayuda", subheadline: "Ten a mano las palabras que te sostienen en los días difíciles.")

        case (.moodHistory, .englishUS):
            return .init(eyebrow: "Mood History", headline: "Notice patterns without pressure", subheadline: "A gentle record of what has helped and what has felt heavy.")
        case (.moodHistory, .englishUK):
            return .init(eyebrow: "Mood History", headline: "Spot patterns without pressure", subheadline: "A gentle record of what has helped and what has felt heavy.")
        case (.moodHistory, .spanishUS):
            return .init(eyebrow: "Historial", headline: "Mira patrones sin exigirte más", subheadline: "Un registro suave de lo que ayuda y de lo que pesa.")
        }
    }
}

enum ScreenshotLocaleVariant: String, CaseIterable, Identifiable {
    case englishUS
    case englishUK
    case spanishUS

    var id: String { rawValue }

    var title: String {
        switch self {
        case .englishUS: "EN-US"
        case .englishUK: "EN-UK"
        case .spanishUS: "ES-US"
        }
    }
}

enum ScreenshotSizePreset: String, CaseIterable, Identifiable {
    case iphone69Portrait
    case iphone65Portrait
    case iphone65LegacyPortrait

    var id: String { rawValue }

    var title: String {
        switch self {
        case .iphone69Portrait: "6.9\""
        case .iphone65Portrait: "6.5\""
        case .iphone65LegacyPortrait: "6.5\" Legacy"
        }
    }

    var exportPixels: CGSize {
        switch self {
        case .iphone69Portrait:
            return CGSize(width: 1290, height: 2796)
        case .iphone65Portrait:
            return CGSize(width: 1284, height: 2778)
        case .iphone65LegacyPortrait:
            return CGSize(width: 1242, height: 2688)
        }
    }

    var devicePoints: CGSize {
        switch self {
        case .iphone69Portrait:
            return CGSize(width: 430, height: 932)
        case .iphone65Portrait:
            return CGSize(width: 428, height: 926)
        case .iphone65LegacyPortrait:
            return CGSize(width: 414, height: 896)
        }
    }

    var exportLabel: String {
        "\(Int(exportPixels.width)) × \(Int(exportPixels.height))"
    }
}

struct ScreenshotMarketingCopy {
    let eyebrow: String
    let headline: String
    let subheadline: String
}

@MainActor
struct DeveloperPreviewGalleryView: View {
    @State private var locale: ScreenshotLocaleVariant = .englishUS
    @State private var sizePreset: ScreenshotSizePreset = .iphone69Portrait

    var body: some View {
        NavigationStack {
            AppScreen(
                title: "Screenshot Mode",
                subtitle: "Developer-only states for polished App Store captures."
            ) {
                VStack(alignment: .leading, spacing: AppTheme.Spacing.medium) {
                    VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
                        SectionHeader(
                            eyebrow: "Copy variant",
                            title: "Store-ready language",
                            subtitle: "Switch headline copy while keeping the same polished screenshot state."
                        )

                        HStack(spacing: AppTheme.Spacing.xxSmall) {
                            ForEach(ScreenshotLocaleVariant.allCases) { variant in
                                localePill(for: variant)
                            }
                        }
                    }

                    VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
                        SectionHeader(
                            eyebrow: "Export size",
                            title: "Exact App Store preset",
                            subtitle: "Use Apple’s accepted portrait targets for current and legacy large iPhone screenshots."
                        )

                        HStack(spacing: AppTheme.Spacing.xxSmall) {
                            ForEach(ScreenshotSizePreset.allCases) { preset in
                                sizePill(for: preset)
                            }
                        }
                    }

                    VStack(spacing: AppTheme.Spacing.small) {
                        ForEach(ScreenshotState.allCases) { state in
                            NavigationLink {
                                ScreenshotCanvasView(
                                    state: state,
                                    locale: locale,
                                    sizePreset: sizePreset
                                )
                            } label: {
                                HStack(spacing: AppTheme.Spacing.small) {
                                    Image(systemName: state.symbol)
                                        .font(.title3.weight(.semibold))
                                        .foregroundStyle(AppTheme.Colors.accent)
                                        .frame(width: 28)

                                    VStack(alignment: .leading, spacing: 2) {
                                        Text(state.title)
                                            .font(.headline)
                                            .foregroundStyle(AppTheme.Colors.textPrimary)
                                        Text(state.subtitle)
                                            .font(.subheadline)
                                            .foregroundStyle(AppTheme.Colors.textSecondary)
                                    }

                                    Spacer()

                                    Text(locale.title)
                                        .font(.caption.weight(.semibold))
                                        .foregroundStyle(AppTheme.Colors.textTertiary)

                                    Text(sizePreset.title)
                                        .font(.caption.weight(.semibold))
                                        .foregroundStyle(AppTheme.Colors.textTertiary)

                                    Image(systemName: "chevron.right")
                                        .font(.footnote.weight(.bold))
                                        .foregroundStyle(AppTheme.Colors.textTertiary)
                                }
                                .padding(AppTheme.Spacing.medium)
                                .hopeCardStyle(padding: 0)
                            }
                            .buttonStyle(.plain)
                        }
                    }
                }
            }
            .navigationTitle("Preview")
            .navigationBarTitleDisplayMode(.inline)
        }
    }

    private func localePill(for variant: ScreenshotLocaleVariant) -> some View {
        Button {
            locale = variant
        } label: {
            Text(variant.title)
                .font(.subheadline.weight(.semibold))
                .padding(.horizontal, 14)
                .padding(.vertical, 10)
                .background(locale == variant ? AppTheme.Colors.accent : AppTheme.Colors.backgroundSoft)
                .foregroundStyle(locale == variant ? Color.white : AppTheme.Colors.textPrimary)
                .clipShape(Capsule())
                .overlay(
                    Capsule()
                        .stroke(locale == variant ? Color.clear : AppTheme.Colors.border, lineWidth: 1)
                )
        }
        .buttonStyle(.plain)
    }

    private func sizePill(for preset: ScreenshotSizePreset) -> some View {
        Button {
            sizePreset = preset
        } label: {
            Text("\(preset.title)  \(preset.exportLabel)")
                .font(.subheadline.weight(.semibold))
                .padding(.horizontal, 14)
                .padding(.vertical, 10)
                .background(sizePreset == preset ? AppTheme.Colors.accent : AppTheme.Colors.backgroundSoft)
                .foregroundStyle(sizePreset == preset ? Color.white : AppTheme.Colors.textPrimary)
                .clipShape(Capsule())
                .overlay(
                    Capsule()
                        .stroke(sizePreset == preset ? Color.clear : AppTheme.Colors.border, lineWidth: 1)
                )
        }
        .buttonStyle(.plain)
    }
}

@MainActor
struct ScreenshotCanvasView: View {
    let state: ScreenshotState
    let locale: ScreenshotLocaleVariant
    let sizePreset: ScreenshotSizePreset

    private let copy: ScreenshotMarketingCopy

    init(state: ScreenshotState, locale: ScreenshotLocaleVariant, sizePreset: ScreenshotSizePreset) {
        self.state = state
        self.locale = locale
        self.sizePreset = sizePreset
        self.copy = state.marketingCopy(for: locale)
    }

    var body: some View {
        ZStack {
            screenshotBackground

            VStack(alignment: .leading, spacing: 28) {
                VStack(alignment: .leading, spacing: 10) {
                    Text(copy.eyebrow.uppercased())
                        .font(.caption.weight(.semibold))
                        .tracking(1.0)
                        .foregroundStyle(Color.white.opacity(0.82))

                    Text(copy.headline)
                        .font(.system(size: 42, weight: .semibold, design: .rounded))
                        .foregroundStyle(Color.white)
                        .fixedSize(horizontal: false, vertical: true)

                    Text(copy.subheadline)
                        .font(.title3)
                        .foregroundStyle(Color.white.opacity(0.9))
                        .fixedSize(horizontal: false, vertical: true)
                }
                .padding(.horizontal, 36)
                .padding(.top, 44)

                Spacer(minLength: 0)

                HStack {
                    Spacer()
                    AppStoreDeviceFrame(preset: sizePreset) {
                        appContent
                    }
                    .frame(width: sizePreset.devicePoints.width + 28, height: sizePreset.devicePoints.height + 28)
                    Spacer()
                }

                Spacer(minLength: 12)
            }
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .navigationTitle(state.title)
        .navigationBarTitleDisplayMode(.inline)
        .toolbar {
            ToolbarItem(placement: .topBarTrailing) {
                Text("\(locale.title) • \(sizePreset.exportLabel)")
                    .font(.caption.weight(.semibold))
                    .foregroundStyle(AppTheme.Colors.textSecondary)
            }
        }
    }

    private var screenshotBackground: some View {
        ZStack {
            LinearGradient(
                colors: [
                    Color(red: 0.27, green: 0.39, blue: 0.51),
                    Color(red: 0.55, green: 0.68, blue: 0.76),
                    Color(red: 0.90, green: 0.94, blue: 0.95)
                ],
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )

            RadialGradient(
                colors: [
                    Color.white.opacity(0.34),
                    Color.white.opacity(0.0)
                ],
                center: .topLeading,
                startRadius: 20,
                endRadius: 420
            )
            .offset(x: -80, y: -120)
        }
        .ignoresSafeArea()
    }

    @ViewBuilder
    private var appContent: some View {
        switch state {
        case .onboarding:
            OnboardingView { }
        case .homeCheckIn:
            NavigationStack {
                HomeView(viewModel: PreviewFactory.homeViewModel)
            }
        case .talk:
            NavigationStack {
                TalkView(viewModel: PreviewFactory.talkViewModel)
            }
        case .notesFeed:
            NavigationStack {
                NotesFeedView(
                    viewModel: PreviewFactory.notesFeedViewModel,
                    writeNoteViewModel: PreviewFactory.writeNoteViewModel
                )
            }
        case .savedNotes:
            NavigationStack {
                SavedNotesView(notes: PreviewFactory.repository.savedNotes)
            }
        case .moodHistory:
            NavigationStack {
                YouView(viewModel: PreviewFactory.youViewModel)
            }
        }
    }
}

struct AppStoreDeviceFrame<Content: View>: View {
    let preset: ScreenshotSizePreset
    @ViewBuilder var content: Content

    var body: some View {
        ZStack(alignment: .top) {
            RoundedRectangle(cornerRadius: 58, style: .continuous)
                .fill(Color.black)
                .frame(width: preset.devicePoints.width + 24, height: preset.devicePoints.height + 24)
                .overlay(
                    RoundedRectangle(cornerRadius: 58, style: .continuous)
                        .stroke(Color.white.opacity(0.18), lineWidth: 1)
                )
                .shadow(color: Color.black.opacity(0.28), radius: 40, x: 0, y: 24)

            content
                .frame(width: preset.devicePoints.width, height: preset.devicePoints.height)
                .clipShape(RoundedRectangle(cornerRadius: 46, style: .continuous))
                .padding(.top, 12)

            Capsule()
                .fill(Color.black.opacity(0.88))
                .frame(width: 126, height: 34)
                .padding(.top, 24)
        }
        .frame(width: preset.devicePoints.width + 24, height: preset.devicePoints.height + 24)
    }
}

@MainActor
private enum PreviewFactory {
    static let repository = HopeRepository(previewSeed: .appStoreScreenshots)

    static var homeViewModel: HomeViewModel {
        let viewModel = HomeViewModel(repository: repository, supportEngine: SupportResponseEngine())
        viewModel.selectMood(.anxious)
        viewModel.heavinessText = "Everything feels loud and close today."
        viewModel.refreshPresentation()
        return viewModel
    }

    static var talkViewModel: TalkViewModel {
        let viewModel = TalkViewModel(
            supportEngine: SupportResponseEngine(),
            safetyClassifier: SafetyClassifier()
        )
        viewModel.messages = [
            ChatMessage(
                id: UUID(),
                role: .assistant,
                text: "I’m here with you. We can make this next hour feel smaller.",
                timestamp: .now.addingTimeInterval(-320)
            ),
            ChatMessage(
                id: UUID(),
                role: .user,
                text: "I feel anxious and a little alone.",
                timestamp: .now.addingTimeInterval(-250)
            ),
            ChatMessage(
                id: UUID(),
                role: .assistant,
                text: "Your body may be loud right now, but you can still move gently. Send one low-pressure text, then breathe in for 4 and out for 6.",
                timestamp: .now.addingTimeInterval(-180)
            ),
            ChatMessage(
                id: UUID(),
                role: .user,
                text: "I can do one message.",
                timestamp: .now.addingTimeInterval(-90)
            ),
            ChatMessage(
                id: UUID(),
                role: .assistant,
                text: "That is enough for now. After you send it, let your shoulders drop once.",
                timestamp: .now.addingTimeInterval(-35)
            )
        ]
        return viewModel
    }

    static var notesFeedViewModel: NotesFeedViewModel {
        NotesFeedViewModel(repository: repository)
    }

    static var writeNoteViewModel: WriteNoteViewModel {
        WriteNoteViewModel(repository: repository)
    }

    static var youViewModel: YouViewModel {
        YouViewModel(repository: repository)
    }
}

#Preview("Screenshot Canvas • Onboarding") {
    ScreenshotCanvasView(state: .onboarding, locale: .englishUS, sizePreset: .iphone69Portrait)
}

#Preview("Screenshot Canvas • Home") {
    ScreenshotCanvasView(state: .homeCheckIn, locale: .englishUS, sizePreset: .iphone69Portrait)
}

#Preview("Screenshot Canvas • Talk") {
    ScreenshotCanvasView(state: .talk, locale: .englishUS, sizePreset: .iphone69Portrait)
}

#Preview("Screenshot Canvas • Notes") {
    ScreenshotCanvasView(state: .notesFeed, locale: .englishUS, sizePreset: .iphone69Portrait)
}

#Preview("Screenshot Canvas • Saved") {
    ScreenshotCanvasView(state: .savedNotes, locale: .englishUS, sizePreset: .iphone69Portrait)
}

#Preview("Screenshot Canvas • History") {
    ScreenshotCanvasView(state: .moodHistory, locale: .englishUS, sizePreset: .iphone69Portrait)
}

#Preview("Screenshot Canvas • Spanish") {
    ScreenshotCanvasView(state: .homeCheckIn, locale: .spanishUS, sizePreset: .iphone65Portrait)
}

#Preview("Screenshot Canvas • Legacy 6.5") {
    ScreenshotCanvasView(state: .homeCheckIn, locale: .englishUS, sizePreset: .iphone65LegacyPortrait)
}
#endif
