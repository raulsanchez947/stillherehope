import 'package:flutter/material.dart';

import '../../views/components/calm_components.dart';

class CrisisResourcesScreen extends StatelessWidget {
  const CrisisResourcesScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Help now')),
      body: const CalmScreen(
        title: 'Get help now',
        subtitle: 'If there is immediate danger, use real-world emergency support.',
        children: [
          CalmCard(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text('If you might hurt yourself or cannot stay safe, call emergency services now.'),
                SizedBox(height: 12),
                Text('In the U.S., call or text 988 for the Suicide & Crisis Lifeline.'),
                SizedBox(height: 12),
                Text('If you can, move near another person and away from anything you could use to hurt yourself.'),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
