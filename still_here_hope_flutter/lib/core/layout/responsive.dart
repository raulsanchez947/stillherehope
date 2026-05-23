import 'package:flutter/widgets.dart';

class Responsive {
  static bool isTablet(BuildContext context) {
    return MediaQuery.sizeOf(context).shortestSide >= 600;
  }

  static double maxContentWidth(BuildContext context) {
    return isTablet(context) ? 760 : double.infinity;
  }

  static EdgeInsets screenPadding(BuildContext context) {
    return EdgeInsets.symmetric(
      horizontal: isTablet(context) ? 40 : 20,
      vertical: isTablet(context) ? 32 : 20,
    );
  }
}

class ResponsiveCenter extends StatelessWidget {
  const ResponsiveCenter({super.key, required this.child});

  final Widget child;

  @override
  Widget build(BuildContext context) {
    return Center(
      child: ConstrainedBox(
        constraints: BoxConstraints(maxWidth: Responsive.maxContentWidth(context)),
        child: child,
      ),
    );
  }
}
