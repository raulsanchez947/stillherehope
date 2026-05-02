import Foundation

enum MoodType: String, CaseIterable, Codable, Identifiable {
    case overwhelmed
    case anxious
    case low
    case angry
    case numb
    case calm

    var id: String { rawValue }

    var title: String {
        rawValue.capitalized
    }

    var symbol: String {
        switch self {
        case .overwhelmed: "wind"
        case .anxious: "leaf"
        case .low: "cloud.drizzle"
        case .angry: "flame"
        case .numb: "moon.zzz"
        case .calm: "sun.min"
        }
    }
}

enum QuickAction: String, CaseIterable, Identifiable {
    case groundMe = "Ground me"
    case slowDown = "Help me slow down"
    case alone = "I feel alone"
    case nextHour = "Get me through the next hour"
    case overwhelmed = "I'm overwhelmed"
    case anxious = "I feel anxious"

    var id: String { rawValue }

    var prompt: String {
        rawValue
    }

    var symbol: String {
        switch self {
        case .groundMe: "figure.mind.and.body"
        case .slowDown: "tortoise"
        case .alone: "person.2"
        case .nextHour: "clock"
        case .overwhelmed: "wind.circle"
        case .anxious: "heart"
        }
    }
}

enum ChatRole: String, Codable {
    case user
    case assistant
}

struct MoodCheckIn: Identifiable, Codable, Hashable {
    let id: UUID
    let date: Date
    let mood: MoodType
    let note: String
    let suggestedAction: String
}

struct HopeNote: Identifiable, Codable, Hashable {
    let id: UUID
    var text: String
    var createdAt: Date
    var tags: [MoodType]
    var helpedCount: Int
    var isSaved: Bool
}

struct ChatMessage: Identifiable, Codable, Hashable {
    let id: UUID
    let role: ChatRole
    let text: String
    let timestamp: Date
}

struct UserStats: Equatable {
    var streakCount: Int
    var totalSavedNotes: Int
    var totalHelpfulNotes: Int
}

struct CheckInPresentation: Equatable {
    let mood: MoodType
    let supportiveLine: String
    let suggestedAction: String
    let groundingPrompt: String
    let sharedNote: HopeNote?
}

struct SafetyAssessment: Equatable {
    let shouldEscalate: Bool
    let matchedPhrase: String?
}
