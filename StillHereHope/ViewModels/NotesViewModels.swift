import Combine
import Foundation

@MainActor
final class NotesFeedViewModel: ObservableObject {
    @Published var selectedMood: MoodType?
    @Published var showsWriteNote = false

    private let repository: any HopeDataStore
    private var cancellables = Set<AnyCancellable>()

    init(repository: any HopeDataStore) {
        self.repository = repository
        repository.changePublisher
            .sink { [weak self] _ in self?.objectWillChange.send() }
            .store(in: &cancellables)
    }

    var notes: [HopeNote] {
        guard let selectedMood else { return repository.notes }
        return repository.notes.filter { $0.tags.contains(selectedMood) }
    }

    func toggleSaved(_ note: HopeNote) {
        repository.toggleSaved(for: note.id)
    }

    func markHelpful(_ note: HopeNote) {
        repository.markHelpful(for: note.id)
    }
}

@MainActor
final class WriteNoteViewModel: ObservableObject {
    @Published var text = ""
    @Published var selectedTags: Set<MoodType> = []
    @Published var didSubmit = false

    private let repository: any HopeDataStore

    init(repository: any HopeDataStore) {
        self.repository = repository
    }

    var charactersRemaining: Int {
        max(0, 180 - text.count)
    }

    var guardrailMessage: String? {
        let lowered = text.lowercased()
        let blockedPhrases = ["kill yourself", "you should disappear", "no point living"]

        if blockedPhrases.contains(where: lowered.contains) {
            return "Try rewriting this as something gentle, grounding, or encouraging."
        }

        return nil
    }

    var canSubmit: Bool {
        let trimmed = text.trimmingCharacters(in: .whitespacesAndNewlines)
        return !trimmed.isEmpty && trimmed.count >= 12 && guardrailMessage == nil
    }

    func submit() {
        guard canSubmit else { return }
        repository.addNote(
            text: text.trimmingCharacters(in: .whitespacesAndNewlines),
            tags: selectedTags.isEmpty ? [.overwhelmed] : Array(selectedTags).sorted { $0.rawValue < $1.rawValue }
        )
        didSubmit = true
    }

    func reset() {
        text = ""
        selectedTags = []
        didSubmit = false
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
