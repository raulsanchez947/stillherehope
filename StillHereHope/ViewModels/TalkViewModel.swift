import Combine
import Foundation

@MainActor
final class TalkViewModel: ObservableObject {
    @Published var messages: [ChatMessage]
    @Published var draft = ""
    @Published var showsCrisisCard = false
    @Published var isReplying = false

    let starterChips: [QuickAction] = [.groundMe, .overwhelmed, .anxious, .alone, .slowDown, .nextHour]

    private let supportEngine: SupportResponseEngine
    private let safetyClassifier: SafetyClassifier

    init(
        supportEngine: SupportResponseEngine,
        safetyClassifier: SafetyClassifier
    ) {
        self.supportEngine = supportEngine
        self.safetyClassifier = safetyClassifier
        self.messages = [
            ChatMessage(
                id: UUID(),
                role: .assistant,
                text: "I’m here with you. Tell me what feels heaviest, and we’ll look for one calmer next step.",
                timestamp: .now
            )
        ]
    }

    func sendStarter(_ action: QuickAction) {
        send(action.prompt)
    }

    func sendCurrentDraft() {
        send(draft)
    }

    private func send(_ text: String) {
        let trimmed = text.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !trimmed.isEmpty else { return }

        draft = ""
        messages.append(
            ChatMessage(id: UUID(), role: .user, text: trimmed, timestamp: .now)
        )

        let safety = safetyClassifier.assess(trimmed)
        if safety.shouldEscalate {
            showsCrisisCard = true
            messages.append(
                ChatMessage(
                    id: UUID(),
                    role: .assistant,
                    text: supportEngine.safetyEscalationResponse(),
                    timestamp: .now
                )
            )
            return
        }

        isReplying = true
        let history = messages

        Task {
            let reply: String
            if let remoteProvider = supportEngine.remoteProvider {
                reply = (try? await remoteProvider.reply(to: trimmed, history: history))
                    ?? supportEngine.localChatReply(for: trimmed)
            } else {
                reply = supportEngine.localChatReply(for: trimmed)
            }

            await MainActor.run {
                self.isReplying = false
                self.messages.append(
                    ChatMessage(
                        id: UUID(),
                        role: .assistant,
                        text: reply,
                        timestamp: .now
                    )
                )
            }
        }
    }
}
