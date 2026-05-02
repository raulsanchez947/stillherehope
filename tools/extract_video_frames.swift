import AVFoundation
import Foundation
import AppKit

let args = CommandLine.arguments
guard args.count >= 3 else {
    fputs("usage: extract_video_frames.swift video output_dir\n", stderr)
    exit(2)
}

let videoURL = URL(fileURLWithPath: args[1])
let outputURL = URL(fileURLWithPath: args[2], isDirectory: true)
try FileManager.default.createDirectory(at: outputURL, withIntermediateDirectories: true)

let asset = AVURLAsset(url: videoURL)
let duration = CMTimeGetSeconds(asset.duration)
guard duration.isFinite, duration > 0 else {
    fputs("could not read video duration\n", stderr)
    exit(1)
}

let generator = AVAssetImageGenerator(asset: asset)
generator.appliesPreferredTrackTransform = true
generator.requestedTimeToleranceBefore = .zero
generator.requestedTimeToleranceAfter = .zero

let frameCount = 12
for index in 0..<frameCount {
    let seconds = duration * Double(index) / Double(frameCount - 1)
    let time = CMTime(seconds: seconds, preferredTimescale: 600)
    do {
        let cgImage = try generator.copyCGImage(at: time, actualTime: nil)
        let bitmap = NSBitmapImageRep(cgImage: cgImage)
        guard let data = bitmap.representation(using: .png, properties: [:]) else {
            continue
        }
        let path = outputURL.appendingPathComponent(String(format: "frame_%02d.png", index))
        try data.write(to: path)
        print(String(format: "frame_%02d %.2fs", index, seconds))
    } catch {
        print(String(format: "frame_%02d %.2fs failed: %@", index, seconds, String(describing: error)))
    }
}
