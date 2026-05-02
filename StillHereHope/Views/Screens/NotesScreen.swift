import SwiftUI

struct NotesFeedView: View {
    @StateObject var viewModel: NotesFeedViewModel
    @StateObject var writeNoteViewModel: WriteNoteViewModel

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
                            onSave: { viewModel.toggleSaved(note) }
                        )
                    }
                }
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

    var body: some View {
        AppScreen(title: "Write a note", subtitle: "What would you tell someone feeling like you?") {
            VStack(alignment: .leading, spacing: AppTheme.Spacing.medium) {
                TextEditor(text: $viewModel.text)
                    .frame(minHeight: 180)
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
                    if let guardrailMessage = viewModel.guardrailMessage {
                        Text(guardrailMessage)
                            .font(.footnote)
                            .foregroundStyle(AppTheme.Colors.warning)
                    }
                }

                VStack(alignment: .leading, spacing: AppTheme.Spacing.small) {
                    Text("Optional mood tags")
                        .font(.headline)
                        .foregroundStyle(AppTheme.Colors.textPrimary)
                    LazyVGrid(columns: [GridItem(.flexible()), GridItem(.flexible())], spacing: AppTheme.Spacing.small) {
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
                        title: "Your words may help someone keep going.",
                        message: "Thank you for leaving something steady behind.",
                        systemImage: "sparkles"
                    )
                }

                Button("Post anonymously") {
                    viewModel.submit()
                }
                .buttonStyle(.borderedProminent)
                .tint(AppTheme.Colors.accent)
                .controlSize(.large)
                .disabled(!viewModel.canSubmit || viewModel.didSubmit)

                if viewModel.didSubmit {
                    Button("Done") {
                        dismiss()
                    }
                    .buttonStyle(.bordered)
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
                        HopeNoteCard(note: note, showsActions: false, onHelpful: nil, onSave: nil)
                    }
                }
            }
        }
        .navigationTitle("Saved")
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

#Preview("Saved Notes Empty") {
    NavigationStack {
        SavedNotesView(notes: [])
    }
}
