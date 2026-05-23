import 'dart:convert';

import 'package:hive/hive.dart';

import '../../core/persistence/hive_boxes.dart';

class BlockedUserManager {
  Box<String> get _box => Hive.box<String>(HiveBoxes.blocked);

  Set<String> get blockedAnonymousUserIds {
    final encoded = _box.get('blockedIds');
    if (encoded == null) return {};
    return (jsonDecode(encoded) as List<dynamic>).cast<String>().toSet();
  }

  Future<void> block(String anonymousUserId) async {
    final blocked = blockedAnonymousUserIds..add(anonymousUserId);
    await _box.put('blockedIds', jsonEncode(blocked.toList()));
  }

  bool isBlocked(String anonymousUserId) {
    return blockedAnonymousUserIds.contains(anonymousUserId);
  }
}
