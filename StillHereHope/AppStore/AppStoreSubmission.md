# Still Here Hope App Store Submission Packet

## App Summary

Still Here Hope is a calm, anonymous self-support app designed to help people slow down, ground themselves, and find one gentler next step. It is not therapy, diagnosis, medical treatment, or emergency care.

## What To Tell Apple In App Review Notes

Use this in the **Notes for Review** field in App Store Connect:

> Still Here Hope is a self-guided emotional grounding and reflection app. There is no login, no account creation, and no paywall. The app is designed to be lightweight and private by default.
>
> The app helps users:
> - check in with a current mood
> - receive short grounding or calming prompts
> - read and save anonymous supportive notes
> - write anonymous notes locally
>
> The app is not intended to provide medical advice, diagnosis, therapy, or crisis intervention. It includes clear crisis-language and directs users to emergency services or 988 in the U.S. when safety-risk phrases are detected.
>
> No login is required to access any feature. The app is intended to be usable immediately and anonymously.

## If Apple Asks Why There Is No Login

Use this:

> This app intentionally does not require account creation. The product goal is immediate, low-friction emotional grounding for overwhelmed users. Requiring sign-up would add friction at the exact moment the user may need the app most. The current version is designed for anonymous, local-first use.

## If Apple Asks About Anonymity

Use this:

> The app is designed around anonymity. Users are not asked for a name, email address, phone number, or profile. There is no login system. In the current app configuration, user-created content and check-ins remain on-device. Anonymous notes are not tied to a user identity.

## Recommended App Description

Still Here Hope is a calm space to check in, slow down, and find one gentler next step.

Use it when the day feels heavy and you need something simple, steady, and immediate:

- check in with how you feel
- get short grounding and breathing prompts
- read supportive anonymous notes
- save the words that help
- track your mood gently over time

Still Here Hope is not therapy, medical care, or crisis care. If you may be in immediate danger or may hurt yourself, call emergency services or 988 in the U.S.

## Recommended Subtitle

Grounding support for hard days

## Suggested Keywords

grounding,calm,anxiety,self care,mood,journal,breathing,wellness,mental health,support

## Recommended Privacy Answers In App Store Connect

These answers are appropriate for the current configuration of the app as checked in now:

- **Does this app collect data?**  
  `No, we do not collect data from this app.`

Important: this is appropriate only if you keep the app in its current configuration:

- `AppConfig.dataStoreKind = .local`
- no login/account system
- no third-party analytics/ads SDKs
- no remote AI provider enabled
- no CloudKit capability enabled for release

If you later enable CloudKit sync or any remote service for user content, revisit the privacy answers before submission.

## Privacy Positioning For Metadata

Use this wording consistently:

- No account required
- No login required
- Anonymous by design
- Local-first storage in the current version
- Not a substitute for medical, mental health, or crisis care

## Export Compliance

If App Store Connect asks about encryption, the app uses standard Apple platform networking/storage behavior only and does not appear to implement custom encryption. Answer the export compliance prompts based on App Store Connect’s current questionnaire.

## Final Pre-Submission Checklist

- Confirm app name displays as `Still Here Hope`
- Confirm scheme is `StillHereHope`
- Confirm no stale `HousingNav` strings appear in shipping UI
- Keep `AppConfig.dataStoreKind` set to `.local` for this submission
- Do not enable CloudKit for this release unless you also update privacy answers
- Add a public privacy policy URL in App Store Connect
- Add a public support URL in App Store Connect
- Support contact email: stillherehope@housingnav.net
- Include the review note above verbatim or close to it
