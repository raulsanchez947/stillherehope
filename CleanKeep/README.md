# CleanKeep

CleanKeep is a privacy-first cleanup assistant for macOS and iPhone. It scans locally, separates likely junk from important files, flags files older than 365 days, and moves selected items to quarantine instead of permanently deleting them.

## What it does

- Finds files that have not been modified in more than 365 days.
- Identifies likely junk such as temporary, crash, partial download, cache, and backup files.
- Protects likely important files such as documents, spreadsheets, presentations, photos, videos, databases, tax files, and credentials.
- Flags potentially risky downloads and executable files for manual security review.
- Quarantines selected files into local `.cleankeep_quarantine` folders.

## Platform behavior

- macOS: scans default user folders and any folder selected through the native folder picker.
- iPhone/iPad: scans only app-accessible storage because iOS does not permit arbitrary whole-device file cleanup.
- Security review: heuristic only. CleanKeep does not claim certified antivirus or confirmed malware detection.

## Run locally

```bash
cd CleanKeep
flutter pub get
flutter test
flutter run -d macos
flutter run -d ios
```

## App Store readiness

Included launch documents:

- `docs/legal/privacy-policy.md`
- `docs/legal/terms-of-service.md`
- `docs/store/app-store-listing.md`
- `docs/store/review-notes.md`
- `docs/store/release-checklist.md`

Before submission, replace placeholder company/support fields, attach final screenshots, verify signing, and have counsel review legal text.
