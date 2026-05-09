import SwiftUI

struct YouView: View {
    @StateObject var viewModel: YouViewModel

    var body: some View {
        AppScreen(title: "You", subtitle: "A quiet view of what has helped and what you’ve carried.") {
            statsGrid
            impactCard
            moodHistorySection

            NavigationLink {
                SavedNotesView(notes: viewModel.savedNotes)
            } label: {
                QuietActionRow(title: "Saved notes", systemImage: "bookmark.fill")
            }
            .buttonStyle(.plain)

            NavigationLink {
                SettingsView()
            } label: {
                QuietActionRow(title: "Settings", systemImage: "gearshape.fill")
            }
            .buttonStyle(.plain)

            NavigationLink {
                ReportConcernView()
            } label: {
                QuietActionRow(title: "Report a concern", systemImage: "flag.fill")
            }
            .buttonStyle(.plain)
        }
        .navigationTitle("You")
        .navigationBarTitleDisplayMode(.inline)
    }

    private var statsGrid: some View {
        HStack(alignment: .top, spacing: AppTheme.Spacing.small) {
            StatCard(title: "day streak", value: "\(viewModel.stats.streakCount)", systemImage: "flame")
            StatCard(title: "saved notes", value: "\(viewModel.stats.totalSavedNotes)", systemImage: "bookmark")
        }
    }

    private var impactCard: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.xxSmall) {
            Text("Impact".uppercased())
                .font(.caption.weight(.semibold))
                .tracking(0.8)
                .foregroundStyle(AppTheme.Colors.textTertiary)
            Text("Your notes helped \(viewModel.stats.totalHelpfulNotes) people")
                .font(.title3.weight(.semibold))
                .foregroundStyle(AppTheme.Colors.textPrimary)
            Text("Even short words can help someone keep going.")
                .font(.body)
                .foregroundStyle(AppTheme.Colors.textSecondary)
        }
        .hopeCardStyle()
    }

    private var moodHistorySection: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
            SectionHeader(
                eyebrow: "Patterns",
                title: "Mood history",
                subtitle: "A soft record of what days have asked from you."
            )

            if viewModel.moodHistory.isEmpty {
                EmptyStateCard(
                    title: "No check-ins yet",
                    message: "Your check-ins will gently collect here so you can notice patterns without pressure.",
                    systemImage: "chart.bar"
                )
            } else {
                VStack(spacing: AppTheme.Spacing.small) {
                    ForEach(viewModel.moodHistory) { checkIn in
                        HStack(spacing: AppTheme.Spacing.small) {
                            Image(systemName: checkIn.mood.symbol)
                                .foregroundStyle(AppTheme.Colors.accent)
                                .frame(width: 26)
                            VStack(alignment: .leading, spacing: 2) {
                                Text(checkIn.mood.title)
                                    .font(.headline)
                                    .foregroundStyle(AppTheme.Colors.textPrimary)
                                Text(checkIn.suggestedAction)
                                    .font(.subheadline)
                                    .foregroundStyle(AppTheme.Colors.textSecondary)
                                    .lineLimit(2)
                            }
                            Spacer()
                            Text(checkIn.date.formatted(date: .abbreviated, time: .omitted))
                                .font(.caption)
                                .foregroundStyle(AppTheme.Colors.textSecondary)
                        }
                        .padding(AppTheme.Spacing.small)
                        .background(AppTheme.Colors.cardStrong)
                        .clipShape(RoundedRectangle(cornerRadius: AppTheme.Corners.button, style: .continuous))
                        .overlay(
                            RoundedRectangle(cornerRadius: AppTheme.Corners.button, style: .continuous)
                                .stroke(AppTheme.Colors.border, lineWidth: 1)
                        )
                    }
                }
                .hopeCardStyle(padding: AppTheme.Spacing.small)
            }
        }
    }
}

#Preview("You") {
    NavigationStack {
        YouView(viewModel: YouViewModel(repository: HopeRepository()))
    }
}

#Preview("You - iPad", traits: .fixedLayout(width: 820, height: 1180)) {
    NavigationStack {
        YouView(viewModel: YouViewModel(repository: HopeRepository()))
    }
}
