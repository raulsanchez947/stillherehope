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

enum ModerationStatus: String, Codable, Hashable {
    case visible
    case underReview
}

enum ReportCategory: String, CaseIterable, Codable, Identifiable, Hashable {
    case harmfulOrTriggering = "harmful or triggering"
    case harassmentOrAbuse = "harassment or abuse"
    case encouragesSelfHarm = "encourages self-harm"
    case hateSpeech = "hate speech"
    case spam = "spam"

    var id: String { rawValue }

    var title: String {
        rawValue.capitalized
    }
}

struct NoteReport: Identifiable, Codable, Hashable {
    let id: UUID
    let noteID: UUID
    let reportReason: ReportCategory
    let timestamp: Date
    let reporterUserID: String
    let reportedUserID: String
}

struct FlaggedNote: Identifiable, Codable, Hashable {
    let id: UUID
    let noteID: UUID?
    let content: String
    let reportReason: ReportCategory
    let timestamp: Date
    let userID: String
}

enum ModerationReviewState: Equatable {
    case clear
    case pendingReview(category: ReportCategory, message: String)
    case blocked(category: ReportCategory, message: String)
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
    var sourceUserID: String
    var moderationStatus: ModerationStatus

    var userID: String {
        sourceUserID
    }

    var isHidden: Bool {
        get { moderationStatus != .visible }
        set { moderationStatus = newValue ? .underReview : .visible }
    }

    private enum CodingKeys: String, CodingKey {
        case id
        case text
        case createdAt
        case tags
        case helpedCount
        case isSaved
        case sourceUserID
        case moderationStatus
    }

    init(
        id: UUID,
        text: String,
        createdAt: Date,
        tags: [MoodType],
        helpedCount: Int,
        isSaved: Bool,
        sourceUserID: String,
        moderationStatus: ModerationStatus
    ) {
        self.id = id
        self.text = text
        self.createdAt = createdAt
        self.tags = tags
        self.helpedCount = helpedCount
        self.isSaved = isSaved
        self.sourceUserID = sourceUserID
        self.moderationStatus = moderationStatus
    }

    init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        id = try container.decode(UUID.self, forKey: .id)
        text = try container.decode(String.self, forKey: .text)
        createdAt = try container.decode(Date.self, forKey: .createdAt)
        tags = try container.decode([MoodType].self, forKey: .tags)
        helpedCount = try container.decode(Int.self, forKey: .helpedCount)
        isSaved = try container.decode(Bool.self, forKey: .isSaved)
        sourceUserID = try container.decodeIfPresent(String.self, forKey: .sourceUserID) ?? "legacy.local"
        moderationStatus = try container.decodeIfPresent(ModerationStatus.self, forKey: .moderationStatus) ?? .visible
    }
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
