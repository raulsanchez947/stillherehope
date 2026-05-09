import SwiftUI

struct NotesFeedView: View {
    @StateObject var viewModel: NotesFeedViewModel
    @StateObject var writeNoteViewModel: WriteNoteViewModel
    @Environment(\.horizontalSizeClass) private var horizontalSizeClass

    var body: some View {
        AppScreen(
            title: "Notes",
            subtitle: "Small anonymous words from people still trying."
        ) {
            filterRow

            if viewModel.notes.isEmpty {
                EmptyStateCard(
                    title: "No notes in this mood yet",
                    message: "You could be the first person to leave a gentler sentence here.",
                    systemImage: "heart"
                )
            } else {
                VStack(spacing: AppTheme.Spacing.small) {
                    ForEach(viewModel.notes) { note in
                        HopeNoteCard(
                            note: note,
                            showsActions: true,
                            onHelpful: { viewModel.markHelpful(note) },
                            onSave: { viewModel.toggleSaved(note) },
                            onReport: { viewModel.beginReport(for: note) }
                        )
                    }
                }
            }

            if let moderationNotice = viewModel.moderationNotice {
                Text(moderationNotice)
                    .font(.footnote)
                    .foregroundStyle(AppTheme.Colors.textSecondary)
                    .padding(.horizontal, AppTheme.Spacing.xxSmall)
            }

            Button {
                writeNoteViewModel.reset()
                viewModel.showsWriteNote = true
            } label: {
                Label("Add a note", systemImage: "square.and.pencil")
                    .font(.body.weight(.semibold))
                    .frame(maxWidth: .infinity, minHeight: 56)
            }
            .buttonStyle(.borderedProminent)
            .tint(AppTheme.Colors.accent)
        }
        .sheet(isPresented: $viewModel.showsWriteNote) {
            NavigationStack {
                WriteNoteView(viewModel: writeNoteViewModel)
            }
        }
        .sheet(item: $viewModel.reportTargetNote) { note in
            NavigationStack {
                ReportNoteView(note: note) { reason, shouldBlockSource in
                    viewModel.submitReport(for: note, reason: reason, shouldBlockSource: shouldBlockSource)
                }
            }
        }
        .navigationTitle("Notes")
        .navigationBarTitleDisplayMode(.inline)
    }

    private var filterRow: some View {
        VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
            SectionHeader(
                eyebrow: "Browse gently",
                title: "Notes feed",
                subtitle: "Filter by feeling if you want something a little closer to your day."
            )

            ScrollView(.horizontal, showsIndicators: false) {
                HStack(spacing: AppTheme.Spacing.xxSmall) {
                    filterPill(title: "All", isSelected: viewModel.selectedMood == nil) {
                        viewModel.selectedMood = nil
                    }

                    ForEach(MoodType.allCases) { mood in
                        filterPill(title: mood.title, isSelected: viewModel.selectedMood == mood) {
                            viewModel.selectedMood = mood
                        }
                    }
                }
            }
        }
    }

    private func filterPill(title: String, isSelected: Bool, action: @escaping () -> Void) -> some View {
        Button(action: action) {
            Text(title)
                .font(.subheadline.weight(.semibold))
                .lineLimit(nil)
                .minimumScaleFactor(0.8)
                .padding(.horizontal, 14)
                .padding(.vertical, 10)
                .background(isSelected ? AppTheme.Colors.accent : AppTheme.Colors.backgroundSoft)
                .foregroundStyle(isSelected ? Color.white : AppTheme.Colors.textPrimary)
                .clipShape(Capsule())
                .overlay(
                    Capsule()
                        .stroke(isSelected ? Color.clear : AppTheme.Colors.border, lineWidth: 1)
                )
        }
        .buttonStyle(.plain)
    }
}

struct WriteNoteView: View {
    @Environment(\.dismiss) private var dismiss
    @ObservedObject var viewModel: WriteNoteViewModel
    @Environment(\.horizontalSizeClass) private var horizontalSizeClass

    var body: some View {
        AppScreen(title: "Write a note", subtitle: "What would you tell someone feeling like you?") {
            VStack(alignment: .leading, spacing: AppTheme.Spacing.medium) {
                TextEditor(text: $viewModel.text)
                    .frame(maxWidth: .infinity, minHeight: 180)
                    .padding(AppTheme.Spacing.small)
                    .background(AppTheme.Colors.cardStrong)
                    .clipShape(RoundedRectangle(cornerRadius: AppTheme.Corners.card, style: .continuous))
                    .overlay(
                        RoundedRectangle(cornerRadius: AppTheme.Corners.card, style: .continuous)
                            .stroke(AppTheme.Colors.border, lineWidth: 1)
                    )

                HStack {
                    Text("\(viewModel.charactersRemaining) characters left")
                        .font(.footnote)
                        .foregroundStyle(AppTheme.Colors.textSecondary)
                    Spacer()
                    if let guardrailMessage = viewModel.submissionMessage ?? viewModel.guardrailMessage {
                        Text(guardrailMessage)
                            .font(.footnote)
                            .foregroundStyle(AppTheme.Colors.warning)
                    }
                }

                VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
                    Text("Optional mood tags")
                        .font(.headline)
                        .foregroundStyle(AppTheme.Colors.textPrimary)
                    LazyVGrid(columns: tagColumns, spacing: AppTheme.Spacing.small) {
                        ForEach(MoodType.allCases) { mood in
                            MoodChip(mood: mood, isSelected: viewModel.selectedTags.contains(mood)) {
                                if viewModel.selectedTags.contains(mood) {
                                    viewModel.selectedTags.remove(mood)
                                } else {
                                    viewModel.selectedTags.insert(mood)
                                }
                            }
                        }
                    }
                }

                if viewModel.didSubmit {
                    EmptyStateCard(
                        title: viewModel.submissionTitle ?? "Your words may help someone keep going.",
                        message: viewModel.submissionMessage ?? "Thank you for leaving something steady behind.",
                        systemImage: "sparkles"
                    )
                }

                Button("Post anonymously") {
                    viewModel.submit()
                }
                .buttonStyle(.borderedProminent)
                .tint(AppTheme.Colors.accent)
                .controlSize(.large)
                .frame(maxWidth: .infinity, minHeight: 44)
                .disabled(!viewModel.canSubmit || viewModel.didSubmit)

                if viewModel.didSubmit {
                    Button("Done") {
                        dismiss()
                    }
                    .buttonStyle(.bordered)
                    .frame(maxWidth: .infinity, minHeight: 44)
                }
            }
        }
        .toolbar {
            ToolbarItem(placement: .topBarTrailing) {
                Button("Close") {
                    dismiss()
                }
            }
        }
    }

    private var tagColumns: [GridItem] {
        let count = horizontalSizeClass == .regular ? 3 : 2
        return Array(repeating: GridItem(.flexible(), spacing: AppTheme.Spacing.small), count: count)
    }
}

struct SavedNotesView: View {
    let notes: [HopeNote]

    var body: some View {
        AppScreen(title: "Saved notes", subtitle: "Words you wanted to keep close.") {
            if notes.isEmpty {
                EmptyStateCard(
                    title: "Nothing saved yet",
                    message: "When a note helps, save it here so it’s easy to find again.",
                    systemImage: "bookmark"
                )
            } else {
                VStack(spacing: AppTheme.Spacing.small) {
                    ForEach(notes) { note in
                        HopeNoteCard(note: note, showsActions: false, onHelpful: nil, onSave: nil, onReport: nil)
                    }
                }
            }
        }
        .navigationTitle("Saved")
        .navigationBarTitleDisplayMode(.inline)
    }
}

struct ReportNoteView: View {
    @Environment(\.dismiss) private var dismiss
    let note: HopeNote
    let onSubmit: (ReportCategory, Bool) -> Void

    @State private var selectedCategory: ReportCategory = .harmfulOrTriggering
    @State private var shouldBlockSource = false

    var body: some View {
        AppScreen(title: "Report note", subtitle: "We’ll hide this note right away and keep the next step simple.") {
            VStack(alignment: .leading, spacing: AppTheme.Spacing.medium) {
                Text("“\(note.text)”")
                    .font(.body)
                    .foregroundStyle(AppTheme.Colors.textPrimary)
                    .padding(AppTheme.Spacing.small)
                    .background(AppTheme.Colors.cardStrong)
                    .clipShape(RoundedRectangle(cornerRadius: AppTheme.Corners.card, style: .continuous))

                VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
                    Text("What feels off about it?")
                        .font(.headline)
                        .foregroundStyle(AppTheme.Colors.textPrimary)

                    ForEach(ReportCategory.allCases) { category in
                        Button {
                            selectedCategory = category
                        } label: {
                            HStack {
                                Text(category.title)
                                    .foregroundStyle(AppTheme.Colors.textPrimary)
                                    .lineLimit(nil)
                                    .minimumScaleFactor(0.8)
                                Spacer()
                                if selectedCategory == category {
                                    Image(systemName: "checkmark.circle.fill")
                                        .foregroundStyle(AppTheme.Colors.accent)
                                }
                            }
                            .padding(AppTheme.Spacing.small)
                            .quietSurfaceStyle()
                        }
                        .buttonStyle(.plain)
                    }
                }

                Toggle("Also hide future notes from this source", isOn: $shouldBlockSource)
                    .toggleStyle(.switch)
                    .foregroundStyle(AppTheme.Colors.textPrimary)

                Button("Submit report") {
                    onSubmit(selectedCategory, shouldBlockSource)
                    dismiss()
                }
                .buttonStyle(.borderedProminent)
                .tint(AppTheme.Colors.accent)

                Button("Cancel") {
                    dismiss()
                }
                .buttonStyle(.bordered)
            }
        }
        .navigationTitle("Report")
        .navigationBarTitleDisplayMode(.inline)
    }
}

#Preview("Notes Feed") {
    NavigationStack {
        NotesFeedView(
            viewModel: NotesFeedViewModel(repository: HopeRepository()),
            writeNoteViewModel: WriteNoteViewModel(repository: HopeRepository())
        )
    }
}

#Preview("Notes Feed - iPad", traits: .fixedLayout(width: 820, height: 1180)) {
    NavigationStack {
        NotesFeedView(
            viewModel: NotesFeedViewModel(repository: HopeRepository()),
            writeNoteViewModel: WriteNoteViewModel(repository: HopeRepository())
        )
    }
}

#Preview("Write Note - iPad Landscape", traits: .fixedLayout(width: 1180, height: 820)) {
    NavigationStack {
        WriteNoteView(viewModel: WriteNoteViewModel(repository: HopeRepository()))
    }
}

#Preview("Saved Notes Empty") {
    NavigationStack {
        SavedNotesView(notes: [])
    }
}
