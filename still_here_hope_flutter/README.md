# Still Here Hope Flutter

Separate Flutter/Android version of Still Here Hope for Google Play.

## Build

Install Flutter, then run:

```sh
flutter pub get
flutter run
flutter build appbundle --release
```

The Android package ID is `com.raulsanchez.stillherehope`.

For Android builds, install Android Studio or the Android command line tools and make sure `ANDROID_HOME` points to the SDK. Add Firebase config files before production release:

- Android: `android/app/google-services.json`
- iOS: `ios/Runner/GoogleService-Info.plist`

## Product Notes

- No login
- Local-first MVP
- Anonymous source IDs only
- Local moderation, reporting, and blocking
- Not therapy, diagnosis, emergency, medical, or crisis care
