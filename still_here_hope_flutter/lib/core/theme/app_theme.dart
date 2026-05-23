import 'package:flutter/material.dart';

class AppTheme {
  static const seed = Color(0xFF5E82A8);
  static const calm = Color(0xFF9EC8B9);
  static const warning = Color(0xFFB85B4A);

  static ThemeData light() {
    return _base(
      brightness: Brightness.light,
      background: const Color(0xFFF6F7F4),
      surface: Colors.white,
      text: const Color(0xFF20252D),
    );
  }

  static ThemeData dark() {
    return _base(
      brightness: Brightness.dark,
      background: const Color(0xFF11161A),
      surface: const Color(0xFF1B2228),
      text: const Color(0xFFE9EEF1),
    );
  }

  static ThemeData _base({
    required Brightness brightness,
    required Color background,
    required Color surface,
    required Color text,
  }) {
    final colorScheme = ColorScheme.fromSeed(
      seedColor: seed,
      brightness: brightness,
      surface: surface,
    );

    return ThemeData(
      useMaterial3: true,
      brightness: brightness,
      colorScheme: colorScheme.copyWith(
        primary: seed,
        secondary: calm,
        error: warning,
        surface: surface,
      ),
      scaffoldBackgroundColor: background,
      fontFamily: 'Roboto',
      textTheme: Typography.material2021().black.apply(
            bodyColor: text,
            displayColor: text,
          ),
      cardTheme: CardThemeData(
        elevation: 0,
        color: surface,
        margin: EdgeInsets.zero,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(24),
          side: BorderSide(color: colorScheme.outlineVariant.withAlpha(128)),
        ),
      ),
      filledButtonTheme: FilledButtonThemeData(
        style: FilledButton.styleFrom(
          minimumSize: const Size.fromHeight(52),
          shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(18)),
        ),
      ),
    );
  }
}
