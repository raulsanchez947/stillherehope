import Combine
import Foundation

@MainActor
final class HomeViewModel: ObservableObject {
    enum PresentationOrigin {
        case moodCheckIn
        case quickAction
    }

    @Published var selectedMood: MoodType?
    @Published var heavinessText = ""
    @Published var currentPresentation: CheckInPresentation?
    @Published var savedStateMessage: String?
    @Published private(set) var presentationOrigin: PresentationOrigin = .moodCheckIn

    let quickActions: [QuickAction] = [.groundMe, .slowDown, .alone, .nextHour]

    private let repository: any HopeDataStore
    private let supportEngine: SupportResponseEngine
    private var cancellables = Set<AnyCancellable>()

    init(repository: any HopeDataStore, supportEngine: SupportResponseEngine) {
        self.repository = repository
        self.supportEngine = supportEngine

        repository.changePublisher
            .sink { [weak self] _ in self?.objectWillChange.send() }
            .store(in: &cancellables)
    }

    var greeting: String {
        let hour = Calendar.current.component(.hour, from: .now)
        return switch hour {
        case 5..<12: "Good morning"
        case 12..<18: "Good afternoon"
        default: "Good evening"
        }
    }

    var canShowFollowUp: Bool {
        selectedMood != nil
    }

    func selectMood(_ mood: MoodType) {
        selectedMood = mood
        savedStateMessage = nil
        presentationOrigin = .moodCheckIn
        currentPresentation = supportEngine.makeCheckInPresentation(
            mood: mood,
            note: heavinessText,
            sharedNote: repository.noteForMood(mood)
        )
    }

    func refreshPresentation() {
        guard let selectedMood else { return }
        presentationOrigin = .moodCheckIn
        currentPresentation = supportEngine.makeCheckInPresentation(
            mood: selectedMood,
            note: heavinessText,
            sharedNote: repository.noteForMood(selectedMood)
        )
    }

    func applyQuickAction(_ action: QuickAction) {
        presentationOrigin = .quickAction
        currentPresentation = supportEngine.quickActionPresentation(
            action,
            sharedNote: repository.noteForMood(selectedMood)
        )
        savedStateMessage = nil
    }

    func saveCheckIn() {
        guard let presentation = currentPresentation else { return }
        repository.saveCheckIn(
            MoodCheckIn(
                id: UUID(),
                date: .now,
                mood: presentation.mood,
                note: heavinessText,
                suggestedAction: presentation.suggestedAction
            )
        )
        savedStateMessage = "Saved for today."
    }
}
