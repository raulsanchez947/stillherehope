# App Review Notes

## Demo Instructions

1. Launch CleanKeep.
2. On macOS, choose a test folder with old files, temporary files, documents, and downloaded executables.
3. Tap “Scan now.”
4. Review the grouped findings.
5. Select low- or medium-risk files and tap “Quarantine selected.”
6. Confirm selected files move to a local `.cleankeep_quarantine` folder.

## Privacy Explanation

CleanKeep processes file metadata locally and does not upload files, file paths, or scan results.

## Permission Explanation

The macOS folder picker is used so users explicitly choose which folders CleanKeep can scan. iOS builds are limited to app-accessible storage because iOS does not allow arbitrary device-wide cleanup.

## Malware Claim Clarification

CleanKeep does not claim to be a certified antivirus product. The “Security review” section only flags files that appear executable or suspiciously named so the user can review them manually.

## Test Account

No account is required.
