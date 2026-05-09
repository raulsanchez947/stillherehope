import SwiftUI

struct HomeView: View {
    @StateObject var viewModel: HomeViewModel
    @FocusState private var isHeavinessFieldFocused: Bool
    @Environment(\.horizontalSizeClass) private var horizontalSizeClass

    var body: some View {
        AppScreen(
            title: "\(viewModel.greeting)",
            subtitle: "A calm space to check in, slow down, and find one gentler next step."
        ) {
            dailyCheckInCard

            if shouldShowPresentationAfterCheckIn,
               let presentation = viewModel.currentPresentation {
                CheckInResultCard(
                    presentation: presentation,
                    savedStateMessage: viewModel.savedStateMessage,
                    onSave: viewModel.saveCheckIn
                )
            }

            quickActionsSection

            if shouldShowPresentationAfterQuickActions,
               let presentation = viewModel.currentPresentation {
                CheckInResultCard(
                    presentation: presentation,
                    savedStateMessage: viewModel.savedStateMessage,
                    onSave: viewModel.saveCheckIn
                )
            }

            NavigationLink {
                CrisisResourcesView()
            } label: {
                Label("Need urgent help right now?", systemImage: "cross.case.fill")
                    .font(.body.weight(.semibold))
                    .frame(maxWidth: .infinity, minHeight: 56)
            }
            .buttonStyle(.borderedProminent)
            .tint(AppTheme.Colors.warning)
        }
        .navigationTitle("Home")
        .navigationBarTitleDisplayMode(.inline)
        .scrollDismissesKeyboard(.interactively)
        .toolbar {
            ToolbarItemGroup(placement: .keyboard) {
                Spacer()
                Button("Done") {
                    isHeavinessFieldFocused = false
                }
            }
        }
    }

    private var shouldShowPresentationAfterCheckIn: Bool {
        viewModel.presentationOrigin == .moodCheckIn
    }

    private var shouldShowPresentationAfterQuickActions: Bool {
        viewModel.presentationOrigin == .quickAction
    }

    private var dailyCheckInCard: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.medium) {
            SectionHeader(
                eyebrow: "Today",
                title: "Daily check-in",
                subtitle: "How does today feel in your body right now?"
            )

            LazyVGrid(columns: moodColumns, spacing: AppTheme.Spacing.small) {
                ForEach(MoodType.allCases) { mood in
                    MoodChip(
                        mood: mood,
                        isSelected: viewModel.selectedMood == mood
                    ) {
                        viewModel.selectMood(mood)
                    }
                }
            }

            if viewModel.canShowFollowUp {
                VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
                    Text("What feels heaviest right now?")
                        .font(.headline)
                        .foregroundStyle(AppTheme.Colors.textPrimary)

                    TextField("Optional", text: $viewModel.heavinessText, axis: .vertical)
                        .textFieldStyle(.plain)
                        .focused($isHeavinessFieldFocused)
                        .submitLabel(.done)
                        .padding(AppTheme.Spacing.small)
                        .frame(maxWidth: .infinity, minHeight: 88, alignment: .topLeading)
                        .quietSurfaceStyle()
                        .onChange(of: viewModel.heavinessText) { _, _ in
                            viewModel.refreshPresentation()
                        }
                        .onSubmit {
                            isHeavinessFieldFocused = false
                        }
                }
            }
        }
        .hopeCardStyle()
    }

    private var quickActionsSection: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
            SectionHeader(
                eyebrow: "Support",
                title: "Quick support",
                subtitle: "Choose one small kind of help for the next few minutes."
            )

            LazyVGrid(columns: quickActionColumns, spacing: AppTheme.Spacing.small) {
                ForEach(viewModel.quickActions) { action in
                    QuietActionButton(title: action.rawValue, systemImage: action.symbol) {
                        isHeavinessFieldFocused = false
                        viewModel.applyQuickAction(action)
                    }
                }
            }
        }
    }

    private var moodColumns: [GridItem] {
        let count = horizontalSizeClass == .regular ? 3 : 2
        return Array(repeating: GridItem(.flexible(), spacing: AppTheme.Spacing.small), count: count)
    }

    private var quickActionColumns: [GridItem] {
        let count = horizontalSizeClass == .regular ? 2 : 1
        return Array(repeating: GridItem(.flexible(), spacing: AppTheme.Spacing.small), count: count)
    }
}

struct CheckInResultCard: View {
    let presentation: CheckInPresentation
    let savedStateMessage: String?
    let onSave: () -> Void

    var body: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.medium) {
            SectionHeader(
                eyebrow: "A gentler next step",
                title: "For right now",
                subtitle: "You do not need to fix the whole day from here."
            )

            VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
                resultRow(title: "Support", text: presentation.supportiveLine)
                resultRow(title: "One small action", text: presentation.suggestedAction)
                resultRow(title: "Grounding", text: presentation.groundingPrompt)
            }

            if let sharedNote = presentation.sharedNote {
                VStack(alignment: .leading, spacing: AppTheme.Spacing.xxSmall) {
                    Text("From someone else")
                        .font(.caption.weight(.semibold))
                        .foregroundStyle(AppTheme.Colors.textSecondary)
                    Text("“\(sharedNote.text)”")
                        .font(.body)
                        .foregroundStyle(AppTheme.Colors.textPrimary)
                        .fixedSize(horizontal: false, vertical: true)
                }
                .padding(AppTheme.Spacing.small)
                .background(AppTheme.Colors.calmSoft)
                .clipShape(RoundedRectangle(cornerRadius: AppTheme.Corners.button, style: .continuous))
            }

            Button(action: onSave) {
                Label("Save check-in", systemImage: "bookmark")
                    .font(AppTheme.Typography.bodyStrong)
                    .frame(maxWidth: .infinity, minHeight: 52)
            }
            .buttonStyle(.borderedProminent)
            .tint(AppTheme.Colors.accent)

            if let savedStateMessage {
                Text(savedStateMessage)
                    .font(.footnote)
                    .foregroundStyle(AppTheme.Colors.textSecondary)
            }
        }
        .hopeCardStyle()
    }

    private func resultRow(title: String, text: String) -> some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.xxxSmall) {
            Text(title)
                .font(.caption.weight(.semibold))
                .foregroundStyle(AppTheme.Colors.textSecondary)
            Text(text)
                .font(.body)
                .foregroundStyle(AppTheme.Colors.textPrimary)
        }
    }
}

#Preview("Home - Resting") {
    NavigationStack {
        HomeView(viewModel: HomeViewModel(repository: HopeRepository(), supportEngine: SupportResponseEngine()))
    }
}

#Preview("Home - iPad Portrait", traits: .fixedLayout(width: 820, height: 1180)) {
    NavigationStack {
        HomeView(viewModel: HomeViewModel(repository: HopeRepository(), supportEngine: SupportResponseEngine()))
    }
}

#Preview("Home - iPad Landscape", traits: .fixedLayout(width: 1180, height: 820)) {
    NavigationStack {
        HomeView(viewModel: HomeViewModel(repository: HopeRepository(), supportEngine: SupportResponseEngine()))
    }
}

#Preview("Home - Result") {
    let vm = HomeViewModel(repository: HopeRepository(), supportEngine: SupportResponseEngine())
    vm.selectMood(.overwhelmed)
    vm.heavinessText = "Work and family both feel too loud."
    vm.refreshPresentation()

    return NavigationStack {
        HomeView(viewModel: vm)
    }
}
