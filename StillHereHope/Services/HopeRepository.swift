import Combine
import CloudKit
import Foundation

@MainActor
protocol HopeDataStore: AnyObject {
    var changePublisher: AnyPublisher<Void, Never> { get }
    var checkIns: [MoodCheckIn] { get }
    var notes: [HopeNote] { get }
    var authoredNoteIDs: Set<UUID> { get }
    var savedNotes: [HopeNote] { get }
    var stats: UserStats { get }

    func saveCheckIn(_ checkIn: MoodCheckIn)
    func addNote(text: String, tags: [MoodType])
    func toggleSaved(for noteID: UUID)
    func markHelpful(for noteID: UUID)
    func noteForMood(_ mood: MoodType?) -> HopeNote?
}

enum HopeDataStoreKind: String, CaseIterable {
    case local
    case cloudKit
    case backendAPI
}

@MainActor
final class HopeRepository: ObservableObject, HopeDataStore {
    @Published private(set) var checkIns: [MoodCheckIn]
    @Published private(set) var notes: [HopeNote]
    @Published private(set) var authoredNoteIDs: Set<UUID>

    private let defaults: UserDefaults
    private let encoder = JSONEncoder()
    private let decoder = JSONDecoder()

    private enum StorageKey {
        static let checkIns = "stillHereHope.checkIns"
        static let notes = "stillHereHope.notes"
        static let authoredNoteIDs = "stillHereHope.authoredNoteIDs"
    }

    init(defaults: UserDefaults = .standard) {
        self.defaults = defaults
        self.encoder.dateEncodingStrategy = .iso8601
        self.decoder.dateDecodingStrategy = .iso8601

        self.checkIns = Self.load([MoodCheckIn].self, key: StorageKey.checkIns, defaults: defaults, decoder: decoder) ?? []
        self.notes = Self.load([HopeNote].self, key: StorageKey.notes, defaults: defaults, decoder: decoder) ?? SeedContent.notes
        self.authoredNoteIDs = Self.load(Set<UUID>.self, key: StorageKey.authoredNoteIDs, defaults: defaults, decoder: decoder) ?? []

        persistNotesIfNeeded()
    }

    var changePublisher: AnyPublisher<Void, Never> {
        objectWillChange
            .map { _ in () }
            .eraseToAnyPublisher()
    }

    var savedNotes: [HopeNote] {
        notes.filter(\.isSaved)
    }

    var stats: UserStats {
        let helpfulImpact = notes
            .filter { authoredNoteIDs.contains($0.id) }
            .reduce(0) { $0 + $1.helpedCount }

        return UserStats(
            streakCount: computeStreak(),
            totalSavedNotes: savedNotes.count,
            totalHelpfulNotes: helpfulImpact
        )
    }

    func saveCheckIn(_ checkIn: MoodCheckIn) {
        checkIns.insert(checkIn, at: 0)
        persist(checkIns, key: StorageKey.checkIns)
    }

    func mergeRemoteCheckIns(_ incoming: [MoodCheckIn]) {
        let merged = Dictionary(
            uniqueKeysWithValues: (checkIns + incoming).map { ($0.id, $0) }
        )
        checkIns = merged.values.sorted { $0.date > $1.date }
        persist(checkIns, key: StorageKey.checkIns)
    }

    func addNote(text: String, tags: [MoodType]) {
        let note = HopeNote(
            id: UUID(),
            text: text,
            createdAt: .now,
            tags: tags,
            helpedCount: 0,
            isSaved: false
        )
        notes.insert(note, at: 0)
        authoredNoteIDs.insert(note.id)
        persist(notes, key: StorageKey.notes)
        persist(authoredNoteIDs, key: StorageKey.authoredNoteIDs)
    }

    func toggleSaved(for noteID: UUID) {
        guard let index = notes.firstIndex(where: { $0.id == noteID }) else { return }
        notes[index].isSaved.toggle()
        persist(notes, key: StorageKey.notes)
    }

    func markHelpful(for noteID: UUID) {
        guard let index = notes.firstIndex(where: { $0.id == noteID }) else { return }
        notes[index].helpedCount += 1
        persist(notes, key: StorageKey.notes)
    }

    func noteForMood(_ mood: MoodType?) -> HopeNote? {
        let filtered: [HopeNote]
        if let mood {
            filtered = notes.filter { $0.tags.contains(mood) }
        } else {
            filtered = notes
        }

        return (filtered.isEmpty ? notes : filtered)
            .sorted { $0.helpedCount > $1.helpedCount }
            .first
    }

    private func computeStreak() -> Int {
        let calendar = Calendar.current
        let days = Set(checkIns.map { calendar.startOfDay(for: $0.date) })

        guard !days.isEmpty else { return 0 }

        var currentDay = calendar.startOfDay(for: .now)
        var streak = 0

        while days.contains(currentDay) {
            streak += 1
            guard let previous = calendar.date(byAdding: .day, value: -1, to: currentDay) else {
                break
            }
            currentDay = previous
        }

        return streak
    }

    private func persistNotesIfNeeded() {
        if defaults.data(forKey: StorageKey.notes) == nil {
            persist(notes, key: StorageKey.notes)
        }
    }

    private func persist<T: Encodable>(_ value: T, key: String) {
        guard let data = try? encoder.encode(value) else { return }
        defaults.set(data, forKey: key)
    }

    private static func load<T: Decodable>(
        _ type: T.Type,
        key: String,
        defaults: UserDefaults,
        decoder: JSONDecoder
    ) -> T? {
        guard let data = defaults.data(forKey: key) else { return nil }
        return try? decoder.decode(T.self, from: data)
    }
}

@MainActor
final class CloudKitHopeDataStore: HopeDataStore {
    private let syncEngine = CloudKitCheckInSyncEngine()
    private let localFallback: HopeRepository

    init(localFallback: HopeRepository? = nil) {
        self.localFallback = localFallback ?? HopeRepository()

        Task { [weak self] in
            await self?.performInitialSync()
        }
    }

    var changePublisher: AnyPublisher<Void, Never> { localFallback.changePublisher }
    var checkIns: [MoodCheckIn] { localFallback.checkIns }
    var notes: [HopeNote] { localFallback.notes }
    var authoredNoteIDs: Set<UUID> { localFallback.authoredNoteIDs }
    var savedNotes: [HopeNote] { localFallback.savedNotes }
    var stats: UserStats { localFallback.stats }

    func saveCheckIn(_ checkIn: MoodCheckIn) {
        localFallback.saveCheckIn(checkIn)

        Task {
            await syncEngine.upsert(checkIn)
        }
    }

    func addNote(text: String, tags: [MoodType]) {
        localFallback.addNote(text: text, tags: tags)
    }

    func toggleSaved(for noteID: UUID) {
        localFallback.toggleSaved(for: noteID)
    }

    func markHelpful(for noteID: UUID) {
        localFallback.markHelpful(for: noteID)
    }

    func noteForMood(_ mood: MoodType?) -> HopeNote? {
        localFallback.noteForMood(mood)
    }

    private func performInitialSync() async {
        let remoteCheckIns = await syncEngine.fetchCheckIns()
        guard !remoteCheckIns.isEmpty else {
            await syncLocalCheckInsToCloud()
            return
        }

        localFallback.mergeRemoteCheckIns(remoteCheckIns)
        await syncLocalCheckInsToCloud()
    }

    private func syncLocalCheckInsToCloud() async {
        let remoteIDs = Set(await syncEngine.fetchCheckIns().map(\.id))
        let unsyncedLocal = localFallback.checkIns.filter { !remoteIDs.contains($0.id) }

        for checkIn in unsyncedLocal {
            await syncEngine.upsert(checkIn)
        }
    }
}

@MainActor
final class BackendHopeDataStore: HopeDataStore {
    private let localFallback: HopeRepository

    init(localFallback: HopeRepository? = nil) {
        self.localFallback = localFallback ?? HopeRepository()
    }

    var changePublisher: AnyPublisher<Void, Never> { localFallback.changePublisher }
    var checkIns: [MoodCheckIn] { localFallback.checkIns }
    var notes: [HopeNote] { localFallback.notes }
    var authoredNoteIDs: Set<UUID> { localFallback.authoredNoteIDs }
    var savedNotes: [HopeNote] { localFallback.savedNotes }
    var stats: UserStats { localFallback.stats }

    func saveCheckIn(_ checkIn: MoodCheckIn) {
        localFallback.saveCheckIn(checkIn)
    }

    func addNote(text: String, tags: [MoodType]) {
        localFallback.addNote(text: text, tags: tags)
    }

    func toggleSaved(for noteID: UUID) {
        localFallback.toggleSaved(for: noteID)
    }

    func markHelpful(for noteID: UUID) {
        localFallback.markHelpful(for: noteID)
    }

    func noteForMood(_ mood: MoodType?) -> HopeNote? {
        localFallback.noteForMood(mood)
    }
}

actor CloudKitCheckInSyncEngine {
    private let database: CKDatabase
    private let recordType = "MoodCheckIn"

    init(container: CKContainer = .default()) {
        self.database = container.privateCloudDatabase
    }

    func fetchCheckIns() async -> [MoodCheckIn] {
        let query = CKQuery(recordType: recordType, predicate: NSPredicate(value: true))
        query.sortDescriptors = [NSSortDescriptor(key: "date", ascending: false)]

        do {
            let records = try await fetchRecords(for: query)
            return records.compactMap(MoodCheckIn.init(record:))
        } catch {
            return []
        }
    }

    func upsert(_ checkIn: MoodCheckIn) async {
        let record = checkIn.cloudKitRecord(recordType: recordType)

        do {
            _ = try await database.save(record)
        } catch {
            return
        }
    }

    private func fetchRecords(for query: CKQuery) async throws -> [CKRecord] {
        try await withCheckedThrowingContinuation { continuation in
            var fetchedRecords: [CKRecord] = []
            let operation = CKQueryOperation(query: query)

            operation.recordMatchedBlock = { _, result in
                if case .success(let record) = result {
                    fetchedRecords.append(record)
                }
            }

            operation.queryResultBlock = { result in
                switch result {
                case .success:
                    continuation.resume(returning: fetchedRecords)
                case .failure(let error):
                    continuation.resume(throwing: error)
                }
            }

            database.add(operation)
        }
    }
}

private extension MoodCheckIn {
    init?(record: CKRecord) {
        guard
            let moodRawValue = record["mood"] as? String,
            let mood = MoodType(rawValue: moodRawValue),
            let date = record["date"] as? Date,
            let note = record["note"] as? String,
            let suggestedAction = record["suggestedAction"] as? String,
            let id = UUID(uuidString: record.recordID.recordName)
        else {
            return nil
        }

        self.init(
            id: id,
            date: date,
            mood: mood,
            note: note,
            suggestedAction: suggestedAction
        )
    }

    func cloudKitRecord(recordType: String) -> CKRecord {
        let recordID = CKRecord.ID(recordName: id.uuidString)
        let record = CKRecord(recordType: recordType, recordID: recordID)
        record["date"] = date as CKRecordValue
        record["mood"] = mood.rawValue as CKRecordValue
        record["note"] = note as CKRecordValue
        record["suggestedAction"] = suggestedAction as CKRecordValue
        return record
    }
}

@MainActor
final class HopeDataStoreProvider: ObservableObject {
    let store: any HopeDataStore
    let kind: HopeDataStoreKind

    init(
        kind: HopeDataStoreKind = .local,
        localFallback: HopeRepository? = nil
    ) {
        self.kind = kind
        let resolvedFallback = localFallback ?? HopeRepository()

        switch kind {
        case .local:
            self.store = resolvedFallback
        case .cloudKit:
            self.store = CloudKitHopeDataStore(localFallback: resolvedFallback)
        case .backendAPI:
            self.store = BackendHopeDataStore(localFallback: resolvedFallback)
        }
    }
}

enum SeedContent {
    static let notes: [HopeNote] = [
        HopeNote(
            id: UUID(uuidString: "13E8D55F-A888-4AE8-9F8E-8E20D8648A11") ?? UUID(),
            text: "You do not need to solve your whole life tonight. Make the next five minutes softer.",
            createdAt: .now.addingTimeInterval(-86_400 * 5),
            tags: [.overwhelmed, .anxious],
            helpedCount: 18,
            isSaved: false
        ),
        HopeNote(
            id: UUID(uuidString: "C319B463-4A80-4FF1-9830-F70E61D531D1") ?? UUID(),
            text: "If all you can do today is stay, breathe, and try again later, that still counts.",
            createdAt: .now.addingTimeInterval(-86_400 * 4),
            tags: [.low, .numb],
            helpedCount: 25,
            isSaved: false
        ),
        HopeNote(
            id: UUID(uuidString: "90FD2A93-9335-457E-B8F8-0CC86A17C204") ?? UUID(),
            text: "Someone else has felt this exact kind of heavy and made it to a quieter hour.",
            createdAt: .now.addingTimeInterval(-86_400 * 3),
            tags: [.low, .anxious],
            helpedCount: 11,
            isSaved: false
        ),
        HopeNote(
            id: UUID(uuidString: "8D785B1E-22C8-4028-A976-172220F8FC3E") ?? UUID(),
            text: "Anger can be real without needing to steer the next choice.",
            createdAt: .now.addingTimeInterval(-86_400 * 2),
            tags: [.angry],
            helpedCount: 8,
            isSaved: false
        ),
        HopeNote(
            id: UUID(uuidString: "8E2A8A35-3BD6-4777-B557-F9F1D9A8A6F0") ?? UUID(),
            text: "Try not to ask tomorrow's questions with tonight's exhausted mind.",
            createdAt: .now.addingTimeInterval(-86_400),
            tags: [.overwhelmed, .low],
            helpedCount: 31,
            isSaved: false
        )
    ]
}

#if DEBUG
extension HopeRepository {
    enum PreviewSeed: String {
        case appStoreScreenshots

        var checkIns: [MoodCheckIn] {
            [
                MoodCheckIn(
                    id: UUID(uuidString: "E3D020B0-8DA0-4C3D-9A66-1F834636D001") ?? UUID(),
                    date: .now,
                    mood: .anxious,
                    note: "Everything feels loud before lunch.",
                    suggestedAction: "Unclench your jaw, sip water, and stay with one task only."
                ),
                MoodCheckIn(
                    id: UUID(uuidString: "E3D020B0-8DA0-4C3D-9A66-1F834636D002") ?? UUID(),
                    date: .now.addingTimeInterval(-86_400),
                    mood: .overwhelmed,
                    note: "Work and family both felt too close.",
                    suggestedAction: "Let one task wait and step outside for one slower minute."
                ),
                MoodCheckIn(
                    id: UUID(uuidString: "E3D020B0-8DA0-4C3D-9A66-1F834636D003") ?? UUID(),
                    date: .now.addingTimeInterval(-172_800),
                    mood: .low,
                    note: "I could barely start the day.",
                    suggestedAction: "Open the curtain and finish one tiny thing."
                ),
                MoodCheckIn(
                    id: UUID(uuidString: "E3D020B0-8DA0-4C3D-9A66-1F834636D004") ?? UUID(),
                    date: .now.addingTimeInterval(-259_200),
                    mood: .calm,
                    note: "A steadier afternoon.",
                    suggestedAction: "Protect this calmer stretch with one kind habit."
                )
            ]
        }

        var notes: [HopeNote] {
            [
                HopeNote(
                    id: UUID(uuidString: "B4E85A11-209C-4256-BD93-44FE96700001") ?? UUID(),
                    text: "You do not need to clear the whole fog tonight. A softer hour still counts.",
                    createdAt: .now.addingTimeInterval(-15_000),
                    tags: [.overwhelmed, .anxious],
                    helpedCount: 46,
                    isSaved: true
                ),
                HopeNote(
                    id: UUID(uuidString: "B4E85A11-209C-4256-BD93-44FE96700002") ?? UUID(),
                    text: "Sometimes the bravest plan is water, socks, and one message to someone safe.",
                    createdAt: .now.addingTimeInterval(-28_000),
                    tags: [.low, .anxious],
                    helpedCount: 31,
                    isSaved: true
                ),
                HopeNote(
                    id: UUID(uuidString: "B4E85A11-209C-4256-BD93-44FE96700003") ?? UUID(),
                    text: "You are allowed to answer this moment more gently than the panic expects.",
                    createdAt: .now.addingTimeInterval(-39_000),
                    tags: [.anxious],
                    helpedCount: 18,
                    isSaved: false
                ),
                HopeNote(
                    id: UUID(uuidString: "B4E85A11-209C-4256-BD93-44FE96700004") ?? UUID(),
                    text: "Even numb days change texture when you let one small sensation in.",
                    createdAt: .now.addingTimeInterval(-52_000),
                    tags: [.numb, .low],
                    helpedCount: 12,
                    isSaved: false
                ),
                HopeNote(
                    id: UUID(uuidString: "B4E85A11-209C-4256-BD93-44FE96700005") ?? UUID(),
                    text: "You can pause before replying and still be honest later.",
                    createdAt: .now.addingTimeInterval(-63_000),
                    tags: [.angry, .overwhelmed],
                    helpedCount: 9,
                    isSaved: false
                )
            ]
        }

        var authoredNoteIDs: Set<UUID> {
            [notes[0].id, notes[1].id]
        }
    }

    convenience init(previewSeed: PreviewSeed) {
        let suiteName = "stillHereHope.preview.\(previewSeed.rawValue)"
        let defaults = UserDefaults(suiteName: suiteName) ?? .standard
        defaults.removePersistentDomain(forName: suiteName)

        let encoder = JSONEncoder()
        encoder.dateEncodingStrategy = .iso8601

        defaults.set(try? encoder.encode(previewSeed.checkIns), forKey: StorageKey.checkIns)
        defaults.set(try? encoder.encode(previewSeed.notes), forKey: StorageKey.notes)
        defaults.set(try? encoder.encode(previewSeed.authoredNoteIDs), forKey: StorageKey.authoredNoteIDs)

        self.init(defaults: defaults)
    }
}
#endif
