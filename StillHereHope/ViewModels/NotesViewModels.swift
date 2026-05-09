import Combine
import Foundation

@MainActor
final class NotesFeedViewModel: ObservableObject {
    @Published var selectedMood: MoodType?
    @Published var showsWriteNote = false
    @Published var reportTargetNote: HopeNote?
    @Published var moderationNotice: String?

    private let repository: any HopeDataStore
    private var cancellables = Set<AnyCancellable>()

    init(repository: any HopeDataStore) {
        self.repository = repository
        repository.changePublisher
            .sink { [weak self] _ in self?.objectWillChange.send() }
            .store(in: &cancellables)
    }

    var notes: [HopeNote] {
        guard let selectedMood else { return repository.visibleNotes }
        return repository.visibleNotes.filter { $0.tags.contains(selectedMood) }
    }

    var blockedUserIDs: Set<String> {
        repository.blockedSourceUserIDs
    }

    func toggleSaved(_ note: HopeNote) {
        repository.toggleSaved(for: note.id)
    }

    func markHelpful(_ note: HopeNote) {
        repository.markHelpful(for: note.id)
    }

    func beginReport(for note: HopeNote) {
        reportTargetNote = note
    }

    func submitReport(for note: HopeNote, reason: ReportCategory, shouldBlockSource: Bool) {
        repository.report(noteID: note.id, reason: reason)
        if shouldBlockSource {
            repository.blockSource(userID: note.sourceUserID)
        }
        moderationNotice = "Thank you. That note is hidden while it’s under review."
        reportTargetNote = nil
    }

    func reportNote(note: HopeNote) {
        submitReport(for: note, reason: .harmfulOrTriggering, shouldBlockSource: false)
    }
}

@MainActor
final class WriteNoteViewModel: ObservableObject {
    @Published var text = ""
    @Published var selectedTags: Set<MoodType> = []
    @Published var didSubmit = false
    @Published var submissionTitle: String?
    @Published var submissionMessage: String?

    private let repository: any HopeDataStore
    private let moderationService: ContentModerationService

    init(repository: any HopeDataStore, moderationService: ContentModerationService = ContentModerationService()) {
        self.repository = repository
        self.moderationService = moderationService
    }

    var charactersRemaining: Int {
        max(0, 180 - text.count)
    }

    var guardrailMessage: String? {
        switch moderationService.evaluate(text) {
        case .clear:
            return nil
        case .pendingReview(_, let message), .blocked(_, let message):
            return message
        }
    }

    var canSubmit: Bool {
        let trimmed = text.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !trimmed.isEmpty && trimmed.count >= 12 else { return false }

        if case .blocked = moderationService.evaluate(trimmed) {
            return false
        }

        return true
    }

    func submit() {
        guard canSubmit else { return }
        let trimmed = text.trimmingCharacters(in: .whitespacesAndNewlines)
        let tags = selectedTags.isEmpty ? [.overwhelmed] : Array(selectedTags).sorted { $0.rawValue < $1.rawValue }

        let moderationState = moderationService.evaluate(trimmed)

        if moderationService.isFlagged(trimmed) {
            let blockedCategory: ReportCategory
            if case .blocked(let category, _) = moderationState {
                blockedCategory = category
            } else {
                blockedCategory = .harassmentOrAbuse
            }

            repository.saveFlaggedNote(
                FlaggedNote(
                    id: UUID(),
                    noteID: nil,
                    content: trimmed,
                    reportReason: blockedCategory,
                    timestamp: .now,
                    userID: repository.currentUserID
                )
            )
            submissionTitle = nil
            submissionMessage = "This message can't be posted."
            didSubmit = false
            return
        }

        switch moderationState {
        case .clear:
            _ = repository.addNote(text: trimmed, tags: tags, moderationStatus: .visible)
            didSubmit = true
            submissionTitle = "Your words may help someone keep going."
            submissionMessage = "Thank you for leaving something steady behind."

        case .pendingReview(let category, let message):
            let note = repository.addNote(text: trimmed, tags: tags, moderationStatus: .underReview)
            repository.saveFlaggedNote(
                FlaggedNote(
                    id: UUID(),
                    noteID: note.id,
                    content: note.text,
                    reportReason: category,
                    timestamp: .now,
                    userID: repository.currentUserID
                )
            )
            didSubmit = true
            submissionTitle = "Your note is under review."
            submissionMessage = message

        case .blocked(let category, let message):
            repository.saveFlaggedNote(
                FlaggedNote(
                    id: UUID(),
                    noteID: nil,
                    content: trimmed,
                    reportReason: category,
                    timestamp: .now,
                    userID: repository.currentUserID
                )
            )
            submissionTitle = nil
            submissionMessage = message
            didSubmit = false
        }
    }

    func reset() {
        text = ""
        selectedTags = []
        didSubmit = false
        submissionTitle = nil
        submissionMessage = nil
    }
}

@MainActor
final class YouViewModel: ObservableObject {
    private let repository: any HopeDataStore
    private var cancellables = Set<AnyCancellable>()

    init(repository: any HopeDataStore) {
        self.repository = repository
        repository.changePublisher
            .sink { [weak self] _ in self?.objectWillChange.send() }
            .store(in: &cancellables)
    }

    var moodHistory: [MoodCheckIn] {
        Array(repository.checkIns.prefix(7))
    }

    var savedNotes: [HopeNote] {
        repository.savedNotes
    }

    var stats: UserStats {
        repository.stats
    }
}
