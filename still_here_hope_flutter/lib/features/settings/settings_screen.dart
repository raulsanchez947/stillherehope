import 'package:flutter/material.dart';

import '../../views/components/calm_components.dart';

class SettingsScreen extends StatelessWidget {
  const SettingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Settings')),
      body: const CalmScreen(
        title: 'Settings',
        subtitle: 'Simple privacy and support information.',
        children: [
          CalmCard(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text('Privacy summary', style: TextStyle(fontWeight: FontWeight.bold)),
                SizedBox(height: 8),
                Text('The app uses an anonymous local ID for notes, reports, and blocking. No public usernames are shown.'),
                SizedBox(height: 16),
                Text('Data handling', style: TextStyle(fontWeight: FontWeight.bold)),
                SizedBox(height: 8),
                Text('MVP data is stored locally with Firebase-ready service hooks for analytics, crash reporting, messaging, and anonymous auth.'),
                SizedBox(height: 16),
                Text('Disclaimer', style: TextStyle(fontWeight: FontWeight.bold)),
                SizedBox(height: 8),
                Text('Still Here Hope is not a substitute for medical, mental health, or crisis care.'),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
