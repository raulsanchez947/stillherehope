import 'package:firebase_messaging/firebase_messaging.dart';

class PushNotificationService {
  PushNotificationService._();
  static final instance = PushNotificationService._();

  Future<void> prepare() async {
    await FirebaseMessaging.instance.requestPermission(
      alert: true,
      badge: true,
      sound: true,
      provisional: true,
    );
  }

  Future<String?> token() => FirebaseMessaging.instance.getToken();
}
