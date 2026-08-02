import Cocoa
import FlutterMacOS

class MainFlutterWindow: NSWindow {
  override func awakeFromNib() {
    let flutterViewController = FlutterViewController()
    let windowFrame = self.frame
    self.contentViewController = flutterViewController
    self.setFrame(windowFrame, display: true)

    let channel = FlutterMethodChannel(
      name: "cleankeep/files",
      binaryMessenger: flutterViewController.engine.binaryMessenger
    )
    channel.setMethodCallHandler { call, result in
      switch call.method {
      case "chooseDirectory":
        let panel = NSOpenPanel()
        panel.canChooseFiles = false
        panel.canChooseDirectories = true
        panel.allowsMultipleSelection = false
        panel.canCreateDirectories = false
        panel.prompt = "Scan Folder"
        panel.message = "Choose a folder CleanKeep can scan locally."

        if panel.runModal() == .OK {
          result(panel.url?.path)
        } else {
          result(nil)
        }
      case "defaultScanRoots":
        let home = FileManager.default.homeDirectoryForCurrentUser
        result([
          home.appendingPathComponent("Downloads").path,
          home.appendingPathComponent("Desktop").path,
          home.appendingPathComponent("Documents").path,
          NSTemporaryDirectory()
        ])
      default:
        result(FlutterMethodNotImplemented)
      }
    }

    RegisterGeneratedPlugins(registry: flutterViewController)

    super.awakeFromNib()
  }
}
