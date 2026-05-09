import SwiftUI

struct OnboardingView: View {
    let onContinue: () -> Void
    @State private var hasAcceptedTerms = false
    @State private var hasAcceptedGuidelines = false
    @State private var activeDocument: AgreementDocument?
    @Environment(\.horizontalSizeClass) private var horizontalSizeClass

    var body: some View {
        GeometryReader { geometry in
            let isWideLayout = AppTheme.Layout.isWideLayout(width: geometry.size.width, sizeClass: horizontalSizeClass)
            let contentWidth = AppTheme.Layout.contentMaxWidth(for: geometry.size.width, sizeClass: horizontalSizeClass)

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
                        .lineLimit(nil)

                    Text("A calm space to check in, slow down, and find one gentler next step.")
                        .font(.title3)
                        .foregroundStyle(AppTheme.Colors.textPrimary)
                        .fixedSize(horizontal: false, vertical: true)

                    Text("Still Here Hope is not emergency or medical care.")
                        .font(.body)
                        .foregroundStyle(AppTheme.Colors.textSecondary)
                }
                .hopeCardStyle(padding: isWideLayout ? AppTheme.Spacing.xLarge + AppTheme.Spacing.small : AppTheme.Spacing.xLarge)

                VStack(alignment: .leading, spacing: AppTheme.Spacing.medium) {
                    Text("Before you continue")
                        .font(.headline)
                        .foregroundStyle(AppTheme.Colors.textPrimary)

                    agreementRow(
                        title: "I agree to the Terms",
                        isOn: $hasAcceptedTerms
                    ) {
                        activeDocument = .terms
                    }

                    agreementRow(
                        title: "I agree to the Community Guidelines",
                        isOn: $hasAcceptedGuidelines
                    ) {
                        activeDocument = .guidelines
                    }
                }
                .hopeCardStyle()

                Spacer(minLength: 0)

                Button(action: onContinue) {
                    Text("Continue")
                        .font(AppTheme.Typography.bodyStrong)
                        .lineLimit(nil)
                        .frame(maxWidth: .infinity, minHeight: 56)
                }
                .buttonStyle(.borderedProminent)
                .tint(AppTheme.Colors.accent)
                .disabled(!canContinue)
            }
            .frame(maxWidth: contentWidth)
            .frame(maxWidth: .infinity)
            .padding(AppTheme.Layout.horizontalPadding(for: geometry.size.width, sizeClass: horizontalSizeClass))
            .padding(.vertical, AppTheme.Layout.verticalPadding(for: geometry.size.width, sizeClass: horizontalSizeClass))
            .background {
                ZStack {
                    AppTheme.backgroundWash
                    AppTheme.accentGlow
                        .offset(x: 60, y: -100)
                }
                .ignoresSafeArea()
            }
        }
        .sheet(item: $activeDocument) { document in
            NavigationStack {
                AgreementDetailView(document: document)
            }
        }
    }

    private var canContinue: Bool {
        hasAcceptedTerms && hasAcceptedGuidelines
    }

    private func agreementRow(
        title: String,
        isOn: Binding<Bool>,
        onRead: @escaping () -> Void
    ) -> some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
            Toggle(title, isOn: isOn)
                .toggleStyle(.switch)
                .foregroundStyle(AppTheme.Colors.textPrimary)

            Button("Read") {
                onRead()
            }
            .font(.footnote.weight(.semibold))
            .buttonStyle(.plain)
            .foregroundStyle(AppTheme.Colors.accent)
            .lineLimit(nil)
        }
    }
}

enum AgreementDocument: String, Identifiable {
    case terms
    case guidelines

    var id: String { rawValue }
}

struct AgreementDetailView: View {
    @Environment(\.dismiss) private var dismiss
    let document: AgreementDocument

    var body: some View {
        AppScreen(title: documentTitle, subtitle: documentSubtitle) {
            VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
                ForEach(documentLines, id: \.self) { line in
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
        .navigationTitle(documentTitle)
        .navigationBarTitleDisplayMode(.inline)
        .toolbar {
            ToolbarItem(placement: .topBarTrailing) {
                Button("Done") {
                    dismiss()
                }
            }
        }
    }

    private var documentTitle: String {
        switch document {
        case .terms: "Terms"
        case .guidelines: "Community Guidelines"
        }
    }

    private var documentSubtitle: String {
        switch document {
        case .terms:
            "A simple agreement for using the app safely and privately."
        case .guidelines:
            "Keep shared notes gentle, anonymous, and safe for others."
        }
    }

    private var documentLines: [String] {
        switch document {
        case .terms:
            [
                "Still Here Hope is for grounding, reflection, and emotional support only.",
                "It is not therapy, diagnosis, emergency response, or medical care.",
                "Please do not post content that threatens, harasses, or harms other people.",
                "Shared notes may be hidden or reviewed if they feel unsafe."
            ]
        case .guidelines:
            [
                "Write as if someone overwhelmed might read your words right away.",
                "Do not encourage self-harm, abuse, threats, hate speech, or harassment.",
                "Keep posts anonymous and do not share personal identifying details.",
                "If a note feels unsafe, report it and the app will hide it during review."
            ]
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
                        "Saved notes are kept on-device."
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

struct ReportConcernView: View {
    var body: some View {
        AppScreen(title: "Report a concern", subtitle: "If something in the app feels unsafe or worrying, here is the simplest next step.") {
            VStack(alignment: .leading, spacing: AppTheme.Spacing.medium) {
                reportSection(
                    title: "How to report",
                    lines: [
                        "Use Report on any note that feels harmful, abusive, unsafe, or spammy.",
                        "Reported notes are hidden right away while they are under review."
                    ]
                )

                reportSection(
                    title: "Support email",
                    lines: [
                        "support@stillherehope.com"
                    ]
                )
            }
        }
        .navigationTitle("Report a Concern")
        .navigationBarTitleDisplayMode(.inline)
    }

    private func reportSection(title: String, lines: [String]) -> some View {
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

#Preview("Onboarding - iPad", traits: .fixedLayout(width: 820, height: 1180)) {
    OnboardingView { }
}

#Preview("Crisis Resources") {
    NavigationStack {
        CrisisResourcesView()
    }
}

#Preview("Settings - iPad", traits: .fixedLayout(width: 820, height: 1180)) {
    NavigationStack {
        SettingsView()
    }
}
