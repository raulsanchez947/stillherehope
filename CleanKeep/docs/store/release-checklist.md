# CleanKeep Release Checklist

## Product

- Replace placeholder support email, company name, support URL, and privacy policy URL.
- Add final app icon, launch assets, and screenshots for iPhone, iPad if supported, and macOS.
- Confirm all user-facing copy avoids unsupported “confirmed malware removal” or “whole device cleanup” claims.
- Add restore/delete management for quarantined files if shipping beyond MVP.

## Engineering

- Run `flutter test`.
- Run `flutter analyze`.
- Run `flutter build ios --release` from a Mac with signing configured.
- Run `flutter build macos --release`.
- Verify sandboxed macOS folder access from a TestFlight or notarized build.
- Verify iOS behavior on physical devices.

## App Store Connect

- Complete App Privacy answers: no account, no tracking, no third-party advertising SDKs, no uploaded file data for this version.
- Upload privacy policy and support URLs.
- Provide review notes from `docs/store/review-notes.md`.
- Confirm category is Utilities.
- Confirm age rating answers.

## Legal

- Have counsel review `docs/legal/privacy-policy.md`.
- Have counsel review `docs/legal/terms-of-service.md`.
- Publish hosted legal pages before submission.

## Security

- Keep scan processing local.
- Do not permanently delete files without an additional restore path and confirmation.
- Maintain clear language that security-review findings are heuristic.
