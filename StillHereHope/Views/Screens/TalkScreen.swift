import SwiftUI

struct TalkView: View {
    @StateObject var viewModel: TalkViewModel

    var body: some View {
        VStack(spacing: 0) {
            if viewModel.showsCrisisCard {
                CrisisBanner()
                    .padding(.horizontal, AppTheme.Spacing.medium)
                    .padding(.top, AppTheme.Spacing.medium)
            }

            ScrollViewReader { proxy in
                ScrollView {
                    VStack(alignment: .leading, spacing: AppTheme.Spacing.medium) {
                        talkHeader
                        starterChips

                        ForEach(viewModel.messages) { message in
                            MessageBubble(message: message)
                                .id(message.id)
                        }

                        if viewModel.isReplying {
                            ProgressView()
                                .padding(.horizontal, AppTheme.Spacing.small)
                        }
                    }
                    .padding(AppTheme.Spacing.medium)
                }
                .background(AppTheme.Colors.background)
                .onChange(of: viewModel.messages.count) { _, _ in
                    if let last = viewModel.messages.last?.id {
                        withAnimation(.easeOut(duration: 0.2)) {
                            proxy.scrollTo(last, anchor: .bottom)
                        }
                    }
                }
            }

            composer
        }
        .navigationTitle("Talk")
        .navigationBarTitleDisplayMode(.inline)
        .background(AppTheme.Colors.background.ignoresSafeArea())
    }

    private var starterChips: some View {
        ScrollView(.horizontal, showsIndicators: false) {
            HStack(spacing: AppTheme.Spacing.xxSmall) {
                ForEach(viewModel.starterChips) { chip in
                    Button(action: {
                        viewModel.sendStarter(chip)
                    }) {
                        Text(chip.rawValue)
                            .font(.subheadline.weight(.semibold))
                            .padding(.horizontal, 14)
                            .padding(.vertical, 10)
                            .background(AppTheme.Colors.backgroundSoft)
                            .foregroundStyle(AppTheme.Colors.textPrimary)
                            .clipShape(Capsule())
                            .overlay(
                                Capsule()
                                    .stroke(AppTheme.Colors.border, lineWidth: 1)
                            )
                    }
                    .buttonStyle(.plain)
                }
            }
        }
    }

    private var talkHeader: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
            SectionHeader(
                eyebrow: "Talk",
                title: "Start where you are",
                subtitle: "You can type freely or choose a simple prompt."
            )

            if viewModel.messages.isEmpty {
                EmptyStateCard(
                    title: "A quiet place to begin",
                    message: "You do not need the right words. One sentence is enough.",
                    systemImage: "text.bubble"
                )
            }
        }
    }

    private var composer: some View {
        VStack(spacing: AppTheme.Spacing.small) {
            HStack(alignment: .bottom, spacing: AppTheme.Spacing.small) {
                TextField("Tell me what feels heaviest right now…", text: $viewModel.draft, axis: .vertical)
                    .textFieldStyle(.plain)
                    .padding(AppTheme.Spacing.small)
                    .background(AppTheme.Colors.cardStrong)
                    .clipShape(RoundedRectangle(cornerRadius: AppTheme.Corners.button, style: .continuous))
                    .overlay(
                        RoundedRectangle(cornerRadius: AppTheme.Corners.button, style: .continuous)
                            .stroke(AppTheme.Colors.border, lineWidth: 1)
                    )

                Button {
                    viewModel.sendCurrentDraft()
                } label: {
                    Image(systemName: "arrow.up")
                        .font(.headline.weight(.bold))
                        .frame(width: 50, height: 50)
                }
                .buttonStyle(.borderedProminent)
                .tint(AppTheme.Colors.accent)
                .disabled(viewModel.draft.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty)
            }

            Text("Still Here Hope is not emergency or medical care.")
                .font(.caption)
                .foregroundStyle(AppTheme.Colors.textSecondary)
                .frame(maxWidth: .infinity, alignment: .leading)
        }
        .padding(AppTheme.Spacing.medium)
        .background(AppTheme.Colors.card.opacity(0.92))
        .overlay(alignment: .top) {
            Rectangle()
                .fill(AppTheme.Colors.border)
                .frame(height: 1)
        }
        .shadow(color: AppTheme.Colors.shadowSoft, radius: 12, x: 0, y: -4)
    }
}

#Preview("Talk") {
    NavigationStack {
        TalkView(viewModel: TalkViewModel(supportEngine: SupportResponseEngine(), safetyClassifier: SafetyClassifier()))
    }
}

#Preview("Talk - Crisis") {
    let vm = TalkViewModel(supportEngine: SupportResponseEngine(), safetyClassifier: SafetyClassifier())
    vm.sendStarter(.groundMe)
    vm.draft = "I want to die"
    vm.sendCurrentDraft()

    return NavigationStack {
        TalkView(viewModel: vm)
    }
}
