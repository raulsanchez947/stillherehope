class SafetyAssessment {
  const SafetyAssessment({required this.shouldEscalate, this.matchedPhrase});

  final bool shouldEscalate;
  final String? matchedPhrase;
}

class SafetyClassifier {
  static const _phrases = [
    'suicide',
    'self-harm',
    "can't stay safe",
    'cant stay safe',
    'want to die',
    'hurt myself',
    'kill myself',
    'emergency',
  ];

  SafetyAssessment assess(String text) {
    final lower = text.toLowerCase();
    for (final phrase in _phrases) {
      if (lower.contains(phrase)) {
        return SafetyAssessment(shouldEscalate: true, matchedPhrase: phrase);
      }
    }
    return const SafetyAssessment(shouldEscalate: false);
  }
}
