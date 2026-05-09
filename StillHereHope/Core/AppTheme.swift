import SwiftUI

enum AppTheme {
    enum Colors {
        static let background = Color(uiColor: .systemGroupedBackground)
        static let backgroundElevated = Color(uiColor: .secondarySystemBackground)
        static let backgroundSoft = Color(uiColor: .tertiarySystemBackground)
        static let card = Color(uiColor: .systemBackground).opacity(0.92)
        static let cardStrong = Color(uiColor: .systemBackground)
        static let shadow = Color.black.opacity(0.10)
        static let shadowSoft = Color.black.opacity(0.03)
        static let textPrimary = Color(uiColor: .label)
        static let textSecondary = Color(uiColor: .secondaryLabel)
        static let textTertiary = Color(uiColor: .tertiaryLabel)
        static let accent = Color(red: 0.35, green: 0.49, blue: 0.64)
        static let accentSoft = Color(red: 0.86, green: 0.91, blue: 0.95)
        static let calm = Color(red: 0.60, green: 0.74, blue: 0.69)
        static let calmSoft = Color(red: 0.90, green: 0.95, blue: 0.93)
        static let warning = Color(red: 0.76, green: 0.42, blue: 0.39)
        static let warningSoft = Color(red: 0.98, green: 0.93, blue: 0.91)
        static let border = Color.primary.opacity(0.10)
        static let borderStrong = Color.primary.opacity(0.16)
    }

    enum Spacing {
        static let xxxSmall: CGFloat = 4
        static let xxSmall: CGFloat = 8
        static let xSmall: CGFloat = 12
        static let small: CGFloat = 16
        static let medium: CGFloat = 20
        static let large: CGFloat = 24
        static let xLarge: CGFloat = 32
    }

    enum Corners {
        static let card: CGFloat = 24
        static let chip: CGFloat = 18
        static let button: CGFloat = 20
        static let pill: CGFloat = 999
    }

    enum Typography {
        static let hero = Font.system(.largeTitle, design: .rounded, weight: .semibold)
        static let sectionTitle = Font.title3.weight(.semibold)
        static let bodyStrong = Font.body.weight(.semibold)
    }

    enum Layout {
        static func horizontalPadding(for width: CGFloat, sizeClass: UserInterfaceSizeClass?) -> CGFloat {
            if sizeClass == .regular || width >= 744 { return 32 }
            return Spacing.medium
        }

        static func verticalPadding(for width: CGFloat, sizeClass: UserInterfaceSizeClass?) -> CGFloat {
            if sizeClass == .regular || width >= 744 { return Spacing.xLarge + Spacing.small }
            return Spacing.large
        }

        static func sectionSpacing(for width: CGFloat, sizeClass: UserInterfaceSizeClass?) -> CGFloat {
            if sizeClass == .regular || width >= 744 { return Spacing.xLarge + Spacing.small }
            return Spacing.xLarge
        }

        static func contentMaxWidth(for width: CGFloat, sizeClass: UserInterfaceSizeClass?) -> CGFloat {
            if sizeClass == .regular || width >= 900 {
                return min(width - 64, 960)
            }
            return width
        }

        static func isWideLayout(width: CGFloat, sizeClass: UserInterfaceSizeClass?) -> Bool {
            sizeClass == .regular || width >= 744
        }
    }

    static let cardStroke = LinearGradient(
        colors: [
            Color.white.opacity(0.8),
            Colors.border
        ],
        startPoint: .topLeading,
        endPoint: .bottomTrailing
    )

    static let backgroundWash = LinearGradient(
        colors: [
            Color(uiColor: .secondarySystemGroupedBackground),
            Colors.background,
            Color(uiColor: .systemGroupedBackground)
        ],
        startPoint: .topLeading,
        endPoint: .bottomTrailing
    )

    static let accentGlow = RadialGradient(
        colors: [
            Colors.accentSoft.opacity(0.95),
            Colors.accentSoft.opacity(0.0)
        ],
        center: .topTrailing,
        startRadius: 8,
        endRadius: 240
    )
}

extension View {
    func hopeCardStyle(padding: CGFloat = AppTheme.Spacing.medium) -> some View {
        self
            .padding(padding)
            .background(AppTheme.Colors.card)
            .clipShape(RoundedRectangle(cornerRadius: AppTheme.Corners.card, style: .continuous))
            .overlay(
                RoundedRectangle(cornerRadius: AppTheme.Corners.card, style: .continuous)
                    .stroke(AppTheme.cardStroke, lineWidth: 1)
            )
            .shadow(color: AppTheme.Colors.shadowSoft, radius: 2, x: 0, y: 1)
            .shadow(color: AppTheme.Colors.shadow, radius: 24, x: 0, y: 10)
    }

    func quietSurfaceStyle() -> some View {
        self
            .background(AppTheme.Colors.backgroundElevated)
            .clipShape(RoundedRectangle(cornerRadius: AppTheme.Corners.button, style: .continuous))
            .overlay(
                RoundedRectangle(cornerRadius: AppTheme.Corners.button, style: .continuous)
                    .stroke(AppTheme.Colors.border, lineWidth: 1)
            )
    }
}
