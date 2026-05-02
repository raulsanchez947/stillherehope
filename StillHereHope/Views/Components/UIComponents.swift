import SwiftUI

struct AppScreen<Content: View>: View {
    let title: String
    let subtitle: String?
    @ViewBuilder var content: Content

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: AppTheme.Spacing.xLarge) {
                VStack(alignment: .leading, spacing: AppTheme.Spacing.xSmall) {
                    Text(title)
                        .font(AppTheme.Typography.hero)
                        .foregroundStyle(AppTheme.Colors.textPrimary)
                    if let subtitle {
                        Text(subtitle)
                        .font(.body)
                            .foregroundStyle(AppTheme.Colors.textSecondary)
                            .fixedSize(horizontal: false, vertical: true)
                    }
                }
                .padding(.top, AppTheme.Spacing.small)

                content
            }
            .padding(.horizontal, AppTheme.Spacing.medium)
            .padding(.top, AppTheme.Spacing.large)
            .padding(.bottom, 136)
        }
        .background {
            ZStack {
                AppTheme.backgroundWash
                AppTheme.accentGlow
                    .blendMode(.plusLighter)
                    .offset(x: 80, y: -120)
            }
            .ignoresSafeArea()
        }
        .scrollIndicators(.hidden)
    }
}

struct SectionHeader: View {
    let eyebrow: String?
    let title: String
    let subtitle: String?

    init(eyebrow: String? = nil, title: String, subtitle: String? = nil) {
        self.eyebrow = eyebrow
        self.title = title
        self.subtitle = subtitle
    }

    var body: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.xxxSmall) {
            if let eyebrow {
                Text(eyebrow.uppercased())
                    .font(.caption.weight(.semibold))
                    .tracking(0.8)
                    .foregroundStyle(AppTheme.Colors.textTertiary)
            }

            Text(title)
                .font(AppTheme.Typography.sectionTitle)
                .foregroundStyle(AppTheme.Colors.textPrimary)

            if let subtitle {
                Text(subtitle)
                    .font(.body)
                    .foregroundStyle(AppTheme.Colors.textSecondary)
                    .fixedSize(horizontal: false, vertical: true)
            }
        }
    }
}

struct MoodChip: View {
    let mood: MoodType
    let isSelected: Bool
    let action: () -> Void

    var body: some View {
        Button(action: action) {
            HStack(spacing: AppTheme.Spacing.xxSmall) {
                Image(systemName: mood.symbol)
                    .font(.subheadline.weight(.semibold))
                Text(mood.title)
                    .font(.subheadline.weight(.semibold))
            }
            .frame(maxWidth: .infinity)
            .padding(.vertical, 12)
            .padding(.horizontal, AppTheme.Spacing.small)
            .background(isSelected ? AppTheme.Colors.accent : AppTheme.Colors.backgroundSoft)
            .foregroundStyle(isSelected ? Color.white : AppTheme.Colors.textPrimary)
            .clipShape(RoundedRectangle(cornerRadius: AppTheme.Corners.chip, style: .continuous))
            .overlay(
                RoundedRectangle(cornerRadius: AppTheme.Corners.chip, style: .continuous)
                    .stroke(isSelected ? Color.clear : AppTheme.Colors.border, lineWidth: 1)
            )
        }
        .buttonStyle(.plain)
        .accessibilityLabel("Mood \(mood.title)")
    }
}

struct QuietActionButton: View {
    let title: String
    let systemImage: String
    let action: () -> Void

    var body: some View {
        Button(action: action) {
            HStack(spacing: AppTheme.Spacing.xxSmall) {
                Image(systemName: systemImage)
                    .font(.body.weight(.semibold))
                Text(title)
                    .font(AppTheme.Typography.bodyStrong)
                    .multilineTextAlignment(.leading)
                Spacer(minLength: 0)
                Image(systemName: "chevron.right")
                    .font(.footnote.weight(.bold))
                    .foregroundStyle(AppTheme.Colors.textTertiary)
            }
            .padding(AppTheme.Spacing.small)
            .frame(maxWidth: .infinity, minHeight: 56)
            .background(AppTheme.Colors.backgroundElevated)
            .foregroundStyle(AppTheme.Colors.textPrimary)
            .quietSurfaceStyle()
        }
        .buttonStyle(.plain)
    }
}

struct CrisisBanner: View {
    var body: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
            Label("Get immediate help now", systemImage: "cross.case.fill")
                .font(.headline)
                .foregroundStyle(AppTheme.Colors.warning)

            Text("If you may be in immediate danger or might hurt yourself, call emergency services now. In the U.S., call or text 988 for immediate crisis support.")
                .font(.body)
                .foregroundStyle(AppTheme.Colors.textPrimary)

            NavigationLink {
                CrisisResourcesView()
            } label: {
                Text("Open crisis resources")
                    .font(AppTheme.Typography.bodyStrong)
                    .frame(maxWidth: .infinity, minHeight: 50)
            }
            .buttonStyle(.borderedProminent)
            .tint(AppTheme.Colors.warning)
        }
        .padding(AppTheme.Spacing.medium)
        .background(AppTheme.Colors.warningSoft)
        .clipShape(RoundedRectangle(cornerRadius: AppTheme.Corners.card, style: .continuous))
        .overlay(
            RoundedRectangle(cornerRadius: AppTheme.Corners.card, style: .continuous)
                .stroke(AppTheme.Colors.warning.opacity(0.18), lineWidth: 1)
        )
    }
}

struct EmptyStateCard: View {
    let title: String
    let message: String
    let systemImage: String

    var body: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
            ZStack {
                Circle()
                    .fill(AppTheme.Colors.accentSoft)
                    .frame(width: 52, height: 52)
                Image(systemName: systemImage)
                    .font(.system(size: 22, weight: .semibold))
                    .foregroundStyle(AppTheme.Colors.accent)
            }

            Text(title)
                .font(AppTheme.Typography.sectionTitle)
                .foregroundStyle(AppTheme.Colors.textPrimary)

            Text(message)
                .font(.body)
                .foregroundStyle(AppTheme.Colors.textSecondary)
                .fixedSize(horizontal: false, vertical: true)
        }
        .frame(maxWidth: .infinity)
        .padding(AppTheme.Spacing.xLarge)
        .hopeCardStyle()
    }
}

struct HopeNoteCard: View {
    let note: HopeNote
    let showsActions: Bool
    let onHelpful: (() -> Void)?
    let onSave: (() -> Void)?

    var body: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
            if !note.tags.isEmpty {
                HStack(spacing: AppTheme.Spacing.xxSmall) {
                    ForEach(note.tags, id: \.id) { tag in
                        Text(tag.title)
                            .font(.caption.weight(.medium))
                            .padding(.horizontal, 10)
                            .padding(.vertical, 6)
                            .background(AppTheme.Colors.accentSoft)
                            .foregroundStyle(AppTheme.Colors.accent)
                            .clipShape(Capsule())
                    }
                }
            }

            Text(note.text)
                .font(.body)
                .foregroundStyle(AppTheme.Colors.textPrimary)
                .fixedSize(horizontal: false, vertical: true)

            if showsActions {
                HStack(spacing: AppTheme.Spacing.small) {
                    Button("This helped") {
                        onHelpful?()
                    }
                    .buttonStyle(.bordered)

                    Button(note.isSaved ? "Saved" : "Save") {
                        onSave?()
                    }
                    .buttonStyle(.borderedProminent)
                    .tint(note.isSaved ? AppTheme.Colors.calm : AppTheme.Colors.accent)

                    Spacer()

                    Label("\(note.helpedCount)", systemImage: "heart.fill")
                        .font(.footnote)
                        .foregroundStyle(AppTheme.Colors.textSecondary)
                }
            }
        }
        .hopeCardStyle()
    }
}

struct MessageBubble: View {
    let message: ChatMessage

    var body: some View {
        HStack {
            if message.role == .assistant {
                bubble
                Spacer(minLength: 48)
            } else {
                Spacer(minLength: 48)
                bubble
            }
        }
    }

    private var bubble: some View {
        VStack(alignment: .leading, spacing: 8) {
            Text(message.text)
                .font(.body)
                .foregroundStyle(message.role == .assistant ? AppTheme.Colors.textPrimary : Color.white)
                .fixedSize(horizontal: false, vertical: true)

            Text(message.timestamp.formatted(date: .omitted, time: .shortened))
                .font(.caption)
                .foregroundStyle(message.role == .assistant ? AppTheme.Colors.textSecondary : Color.white.opacity(0.8))
        }
        .padding(AppTheme.Spacing.small)
        .background(message.role == .assistant ? AppTheme.Colors.cardStrong : AppTheme.Colors.accent)
        .clipShape(RoundedRectangle(cornerRadius: 22, style: .continuous))
        .overlay(
            RoundedRectangle(cornerRadius: 22, style: .continuous)
                .stroke(message.role == .assistant ? AppTheme.Colors.border : Color.clear, lineWidth: 1)
        )
        .shadow(color: message.role == .assistant ? AppTheme.Colors.shadowSoft : Color.clear, radius: 10, x: 0, y: 4)
    }
}

struct StatCard: View {
    let title: String
    let value: String
    let systemImage: String

    var body: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.xxSmall) {
            Image(systemName: systemImage)
                .font(.headline)
                .foregroundStyle(AppTheme.Colors.accent)

            Text(value)
                .font(.title2.weight(.semibold))
                .foregroundStyle(AppTheme.Colors.textPrimary)

            Text(title)
                .font(.footnote)
                .foregroundStyle(AppTheme.Colors.textSecondary)
        }
        .frame(minHeight: 104, alignment: .topLeading)
        .frame(maxWidth: .infinity, alignment: .leading)
        .hopeCardStyle()
    }
}
