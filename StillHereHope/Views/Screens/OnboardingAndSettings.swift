import SwiftUI

struct OnboardingView: View {
    let onContinue: () -> Void

    var body: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.xLarge) {
            Spacer(minLength: 0)

            VStack(alignment: .leading, spacing: AppTheme.Spacing.medium) {
                ZStack {
                    Circle()
                        .fill(AppTheme.Colors.accentSoft)
                        .frame(width: 76, height: 76)
                    Image(systemName: "sun.horizon.circle.fill")
                        .font(.system(size: 38))
                        .foregroundStyle(AppTheme.Colors.accent)
                }

                Text("Still Here Hope")
                    .font(AppTheme.Typography.hero)
                    .foregroundStyle(AppTheme.Colors.textPrimary)

                Text("A calm space to check in, slow down, and find one gentler next step.")
                    .font(.title3)
                    .foregroundStyle(AppTheme.Colors.textPrimary)
                    .fixedSize(horizontal: false, vertical: true)

                Text("Still Here Hope is not emergency or medical care.")
                    .font(.body)
                    .foregroundStyle(AppTheme.Colors.textSecondary)
            }
            .hopeCardStyle(padding: AppTheme.Spacing.xLarge)

            Spacer(minLength: 0)

            Button(action: onContinue) {
                Text("Continue")
                    .font(AppTheme.Typography.bodyStrong)
                    .frame(maxWidth: .infinity, minHeight: 56)
            }
            .buttonStyle(.borderedProminent)
            .tint(AppTheme.Colors.accent)
        }
        .padding(AppTheme.Spacing.large)
        .background {
            ZStack {
                AppTheme.backgroundWash
                AppTheme.accentGlow
                    .offset(x: 60, y: -100)
            }
            .ignoresSafeArea()
        }
    }
}

struct CrisisResourcesView: View {
    var body: some View {
        AppScreen(title: "Crisis resources", subtitle: "If safety feels uncertain, please reach for immediate human support.") {
            CrisisBanner()

            VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
                SectionHeader(
                    eyebrow: "Immediate support",
                    title: "If there is immediate danger",
                    subtitle: nil
                )
                Text("Call emergency services now or go to the nearest emergency room.")
                    .foregroundStyle(AppTheme.Colors.textSecondary)
                Divider()
                Text("988 Suicide & Crisis Lifeline")
                    .font(.headline)
                Text("In the U.S., call or text 988 for immediate support, any time.")
                    .foregroundStyle(AppTheme.Colors.textSecondary)
            }
            .hopeCardStyle()
        }
        .navigationTitle("Crisis")
        .navigationBarTitleDisplayMode(.inline)
    }
}

struct SettingsView: View {
    var body: some View {
        AppScreen(title: "Settings", subtitle: "Simple, clear, and private by default.") {
            VStack(alignment: .leading, spacing: AppTheme.Spacing.medium) {
                settingsSection(
                    title: "Privacy summary",
                    lines: [
                        "Your check-ins, saved notes, and written notes are stored locally for this MVP.",
                        "You are not asked to create an account.",
                        "Anonymous notes do not include your name."
                    ]
                )

                settingsSection(
                    title: "Data handling summary",
                    lines: [
                        "Mood check-ins are kept on-device.",
                        "Saved notes are kept on-device.",
                        "The app is structured so CloudKit or a backend can be added later if you choose."
                    ]
                )

                settingsSection(
                    title: "Support links",
                    lines: [
                        "Open crisis resources from any screen.",
                        "Use 988 in the U.S. when safety is at risk."
                    ]
                )

                Text("Still Here Hope is not a substitute for medical, mental health, or crisis care.")
                    .font(.footnote)
                    .foregroundStyle(AppTheme.Colors.textSecondary)
                    .padding(.top, AppTheme.Spacing.small)
            }
        }
        .navigationTitle("Settings")
        .navigationBarTitleDisplayMode(.inline)
    }

    private func settingsSection(title: String, lines: [String]) -> some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
            Text(title)
                .font(.headline)
                .foregroundStyle(AppTheme.Colors.textPrimary)
            ForEach(lines, id: \.self) { line in
                HStack(alignment: .top, spacing: AppTheme.Spacing.xxSmall) {
                    Image(systemName: "circle.fill")
                        .font(.system(size: 6))
                        .foregroundStyle(AppTheme.Colors.accent)
                        .padding(.top, 8)
                    Text(line)
                        .foregroundStyle(AppTheme.Colors.textSecondary)
                }
            }
        }
        .hopeCardStyle()
    }
}

#Preview("Onboarding") {
    OnboardingView { }
}

#Preview("Crisis Resources") {
    NavigationStack {
        CrisisResourcesView()
    }
}
