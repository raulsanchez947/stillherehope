import Foundation

protocol RemoteSupportProviding {
    func reply(to message: String, history: [ChatMessage]) async throws -> String
}

struct SafetyClassifier {
    private let triggerPhrases = [
        "suicide",
        "self-harm",
        "can't stay safe",
        "cant stay safe",
        "want to die",
        "hurt myself",
        "kill myself",
        "emergency"
    ]

    func assess(_ text: String) -> SafetyAssessment {
        let lowercased = text.lowercased()
        let match = triggerPhrases.first(where: lowercased.contains)
        return SafetyAssessment(shouldEscalate: match != nil, matchedPhrase: match)
    }
}

struct SupportResponseEngine {
    var remoteProvider: RemoteSupportProviding?
    private let defaultWordLimit = 90

    func makeCheckInPresentation(
        mood: MoodType,
        note: String,
        sharedNote: HopeNote?
    ) -> CheckInPresentation {
        let pairedNote = pairNote(sharedNote, fallback: fallbackNote(for: mood))

        return CheckInPresentation(
            mood: mood,
            supportiveLine: supportiveLine(for: mood),
            suggestedAction: refinedAction(for: mood, note: note),
            groundingPrompt: groundingPrompt(for: mood),
            sharedNote: pairedNote
        )
    }

    func quickActionPresentation(
        _ action: QuickAction,
        sharedNote: HopeNote?
    ) -> CheckInPresentation {
        let mood = mood(for: action)
        let pairedNote = pairNote(sharedNote, fallback: fallbackNote(for: action))

        return CheckInPresentation(
            mood: mood,
            supportiveLine: supportiveLine(for: action),
            suggestedAction: quickActionSuggestion(for: action),
            groundingPrompt: regulationPrompt(for: action),
            sharedNote: pairedNote
        )
    }

    func safetyEscalationResponse() -> String {
        limited("""
        I’m really glad you said that. If you might hurt yourself or can’t stay safe, call or text 988 now in the U.S., or contact emergency services where you are. Move closer to other people if you can, and put distance between you and anything you could use to hurt yourself.
        """)
    }

    func localChatReply(for text: String) -> String {
        let intent = detectIntent(in: text)
        let response: String

        switch intent {
        case .quickAction(let action):
            response = composeReply(
                supportive: supportiveLine(for: action),
                action: quickActionSuggestion(for: action),
                regulation: regulationPrompt(for: action),
                note: fallbackNote(for: action)
            )
        case .mood(let mood):
            response = composeReply(
                supportive: supportiveLine(for: mood),
                action: refinedAction(for: mood, note: text),
                regulation: combinedPrompt(for: mood),
                note: fallbackNote(for: mood)
            )
        case .generic:
            response = composeReply(
                supportive: "Thank you for saying it plainly. We can stay with just this part.",
                action: "Name the heaviest piece, then choose one small act of care for the next ten minutes.",
                regulation: "Take one slow breath out, longer than in.",
                note: genericFallbackNote()
            )
        }

        return limited(response)
    }

    private func detectIntent(in text: String) -> DetectedIntent {
        let lowercased = text.lowercased()

        if lowercased.contains("next hour") || lowercased.contains("get through the next hour") {
            return .quickAction(.nextHour)
        }
        if lowercased.contains("ground me") || lowercased.contains("grounding") {
            return .quickAction(.groundMe)
        }
        if lowercased.contains("slow down") || lowercased.contains("too fast") {
            return .quickAction(.slowDown)
        }
        if lowercased.contains("alone") || lowercased.contains("lonely") {
            return .quickAction(.alone)
        }
        if lowercased.contains("overwhelmed") || lowercased.contains("too much") {
            return .mood(.overwhelmed)
        }
        if lowercased.contains("anxious") || lowercased.contains("panic") || lowercased.contains("spiral") {
            return .mood(.anxious)
        }
        if lowercased.contains("angry") || lowercased.contains("mad") || lowercased.contains("furious") {
            return .mood(.angry)
        }
        if lowercased.contains("numb") || lowercased.contains("empty") || lowercased.contains("shut down") {
            return .mood(.numb)
        }
        if lowercased.contains("low") || lowercased.contains("sad") || lowercased.contains("heavy") {
            return .mood(.low)
        }
        if lowercased.contains("calm") || lowercased.contains("steady") {
            return .mood(.calm)
        }

        return .generic
    }

    private func supportiveLine(for mood: MoodType) -> String {
        switch mood {
        case .overwhelmed:
            return "You do not need to carry the whole pile at once."
        case .anxious:
            return "Your body may be loud right now, but you can still move gently."
        case .low:
            return "You do not need big hope to care for this hour."
        case .angry:
            return "Anger can be real without choosing the next move for you."
        case .numb:
            return "Numb does not mean gone. It may mean overloaded."
        case .calm:
            return "This steadier moment is worth protecting."
        }
    }

    private func supportiveLine(for action: QuickAction) -> String {
        switch action {
        case .groundMe:
            return "Let’s help the room feel real again."
        case .slowDown:
            return "You are allowed to go slower than the pressure says."
        case .alone:
            return "Feeling alone can make everything sound harsher."
        case .nextHour:
            return "You only need a softer plan for the next hour."
        case .overwhelmed:
            return supportiveLine(for: MoodType.overwhelmed)
        case .anxious:
            return supportiveLine(for: MoodType.anxious)
        }
    }

    private func refinedAction(for mood: MoodType, note: String) -> String {
        let lowercased = note.lowercased()

        if lowercased.contains("work") {
            return "Pick one work task to delay and one to finish imperfectly."
        }
        if lowercased.contains("sleep") || lowercased.contains("tired") || lowercased.contains("exhausted") {
            return "Choose relief before productivity: water, dim light, and a quieter next hour."
        }
        if lowercased.contains("family") || lowercased.contains("relationship") {
            return "Pause the hardest conversation and choose one boundary for tonight."
        }

        switch mood {
        case .overwhelmed:
            return "Put both feet down and choose one thing that can wait."
        case .anxious:
            return "Unclench your jaw, drop your shoulders, and sip something cool or warm."
        case .low:
            return "Open the curtain, drink water, and finish one tiny task."
        case .angry:
            return "Pause the reply and write one sentence about what feels crossed."
        case .numb:
            return "Try cool water on your hands or step outside for one minute."
        case .calm:
            return "Choose one kind habit that protects this steadier mood."
        }
    }

    private func quickActionSuggestion(for action: QuickAction) -> String {
        switch action {
        case .groundMe:
            return "Name three colors, two sounds, and one thing touching your skin."
        case .slowDown:
            return "Pause one task and do one gentler thing before you continue."
        case .alone:
            return "Send one low-pressure text or sit somewhere other people are nearby."
        case .nextHour:
            return nextHourPlan()
        case .overwhelmed:
            return quickActionSuggestion(for: .slowDown)
        case .anxious:
            return quickActionSuggestion(for: .groundMe)
        }
    }

    private func breathingPrompt(for mood: MoodType) -> String {
        switch mood {
        case .overwhelmed:
            return "Inhale for 4, exhale for 6, three times."
        case .anxious:
            return "Breathe in for 4, hold for 2, out for 6."
        case .low:
            return "Take three slower breaths and let your shoulders soften."
        case .angry:
            return "Exhale twice as long as you inhale for three rounds."
        case .numb:
            return "Take one fuller breath and notice the air at your nose."
        case .calm:
            return "Take one slow breath to help this steadiness stay."
        }
    }

    private func groundingPrompt(for mood: MoodType) -> String {
        switch mood {
        case .overwhelmed:
            return "Name five things you can see and one thing holding your weight."
        case .anxious:
            return "Press your feet down and notice one sound in the room."
        case .low:
            return "Place a hand on your chest and feel its warmth for ten seconds."
        case .angry:
            return "Press your feet into the floor and relax your hands once."
        case .numb:
            return "Name one texture, one temperature, and one sound."
        case .calm:
            return "Notice what already feels a little easier in your body."
        }
    }

    private func regulationPrompt(for action: QuickAction) -> String {
        switch action {
        case .groundMe:
            return "Breathe out longer than you breathe in."
        case .slowDown:
            return "Put one hand on your chest and count five slow exhales."
        case .alone:
            return "Say: I do not need to carry this without contact forever."
        case .nextHour:
            return "Ask: what would make the next 60 minutes 5% easier?"
        case .overwhelmed:
            return combinedPrompt(for: .overwhelmed)
        case .anxious:
            return combinedPrompt(for: .anxious)
        }
    }

    private func combinedPrompt(for mood: MoodType) -> String {
        "\(breathingPrompt(for: mood)) \(groundingPrompt(for: mood))"
    }

    private func nextHourPlan() -> String {
        "Pick one anchor for the next hour: water, food, shower, outside air, or one safe person."
    }

    private func pairNote(_ sharedNote: HopeNote?, fallback: HopeNote) -> HopeNote {
        sharedNote ?? fallback
    }

    private func fallbackNote(for mood: MoodType) -> HopeNote {
        HopeNote(
            id: UUID(),
            text: fallbackNoteText(for: mood),
            createdAt: .now,
            tags: [mood],
            helpedCount: 0,
            isSaved: false,
            sourceUserID: "system.fallback",
            moderationStatus: .visible
        )
    }

    private func fallbackNote(for action: QuickAction) -> HopeNote {
        let mood = mood(for: action)
        return HopeNote(
            id: UUID(),
            text: fallbackNoteText(for: mood),
            createdAt: .now,
            tags: [mood],
            helpedCount: 0,
            isSaved: false,
            sourceUserID: "system.fallback",
            moderationStatus: .visible
        )
    }

    private func genericFallbackNote() -> HopeNote {
        HopeNote(
            id: UUID(),
            text: "You do not need a perfect plan to deserve a gentler next ten minutes.",
            createdAt: .now,
            tags: [],
            helpedCount: 0,
            isSaved: false,
            sourceUserID: "system.fallback",
            moderationStatus: .visible
        )
    }

    private func fallbackNoteText(for mood: MoodType) -> String {
        switch mood {
        case .overwhelmed:
            return "You can let one thing be unfinished and still be doing enough."
        case .anxious:
            return "A slower exhale counts as progress."
        case .low:
            return "Staying for this hour is already a real thing."
        case .angry:
            return "You can pause before giving the feeling the microphone."
        case .numb:
            return "Small sensation is still sensation."
        case .calm:
            return "Protecting a steadier moment is its own kind of care."
        }
    }

    private func mood(for action: QuickAction) -> MoodType {
        switch action {
        case .groundMe, .anxious:
            return .anxious
        case .slowDown, .overwhelmed, .nextHour:
            return .overwhelmed
        case .alone:
            return .low
        }
    }

    private func composeReply(
        supportive: String,
        action: String,
        regulation: String,
        note: HopeNote?
    ) -> String {
        var parts = [
            supportive,
            action,
            regulation
        ]

        if let note {
            parts.append("Anonymous note: “\(note.text)”")
        }

        return parts.joined(separator: " ")
    }

    private func limited(_ text: String) -> String {
        let words = text.split(whereSeparator: \.isWhitespace)
        guard words.count > defaultWordLimit else { return text }
        return words.prefix(defaultWordLimit).joined(separator: " ") + "…"
    }
}

private enum DetectedIntent {
    case mood(MoodType)
    case quickAction(QuickAction)
    case generic
}

struct ContentModerationService {
    func isFlagged(_ text: String) -> Bool {
        if case .blocked = evaluate(text) {
            return true
        }
        return false
    }

    func evaluate(_ text: String) -> ModerationReviewState {
        let normalized = normalize(text)

        if matchesAny(in: normalized, phrases: [
            "kill yourself",
            "go kill yourself",
            "you should die",
            "people like you should die",
            "i will kill you",
            "i'm going to kill you",
            "i am going to kill you",
            "i'll hurt you",
            "burn in hell"
        ]) {
            return .blocked(
                category: matchesAny(in: normalized, phrases: ["kill yourself", "go kill yourself"]) ? .encouragesSelfHarm : .harassmentOrAbuse,
                message: "That note can’t be shared here. Try rewriting it as something grounding, gentle, or safe."
            )
        }

        if matchesAny(in: normalized, phrases: [
            "worthless trash",
            "subhuman",
            "dirty immigrant",
            "fag",
            "kike",
            "nigger",
            "spic",
            "tranny"
        ]) {
            return .blocked(
                category: .hateSpeech,
                message: "That note can’t be shared here. Try rewriting it in a way that does not target or degrade anyone."
            )
        }

        if matchesAny(in: normalized, phrases: [
            "nobody wants you",
            "you're pathetic",
            "you are pathetic",
            "everyone hates you",
            "shut up",
            "you deserve pain",
            "end it already",
            "just disappear"
        ]) {
            return .pendingReview(
                category: .harassmentOrAbuse,
                message: "That note needs a review before it can be shared. Try rewriting it as support instead of harm."
            )
        }

        if matchesAny(in: normalized, phrases: [
            "buy now",
            "click this link",
            "dm me for",
            "make money fast"
        ]) {
            return .pendingReview(
                category: .spam,
                message: "That note is being held for review."
            )
        }

        return .clear
    }

    private func normalize(_ text: String) -> String {
        text
            .lowercased()
            .replacingOccurrences(of: "[^a-z0-9\\s]", with: " ", options: .regularExpression)
            .replacingOccurrences(of: "\\s+", with: " ", options: .regularExpression)
            .trimmingCharacters(in: .whitespacesAndNewlines)
    }

    private func matchesAny(in text: String, phrases: [String]) -> Bool {
        phrases.contains(where: text.contains)
    }
}
