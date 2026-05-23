import 'package:flutter/material.dart';

import '../ai_chat/talk_screen.dart';
import '../home/home_screen.dart';
import '../notes/presentation/notes_screen.dart';
import '../you/you_screen.dart';

class AppShellScreen extends StatefulWidget {
  const AppShellScreen({super.key});

  @override
  State<AppShellScreen> createState() => _AppShellScreenState();
}

class _AppShellScreenState extends State<AppShellScreen> {
  int index = 0;

  final pages = const [
    HomeScreen(),
    TalkScreen(),
    NotesScreen(),
    YouScreen(),
  ];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: IndexedStack(index: index, children: pages),
      bottomNavigationBar: NavigationBar(
        selectedIndex: index,
        onDestinationSelected: (value) => setState(() => index = value),
        destinations: const [
          NavigationDestination(icon: Icon(Icons.home_outlined), selectedIcon: Icon(Icons.home), label: 'Home'),
          NavigationDestination(icon: Icon(Icons.chat_bubble_outline), selectedIcon: Icon(Icons.chat_bubble), label: 'Talk'),
          NavigationDestination(icon: Icon(Icons.note_alt_outlined), selectedIcon: Icon(Icons.note_alt), label: 'Notes'),
          NavigationDestination(icon: Icon(Icons.person_outline), selectedIcon: Icon(Icons.person), label: 'You'),
        ],
      ),
    );
  }
}
