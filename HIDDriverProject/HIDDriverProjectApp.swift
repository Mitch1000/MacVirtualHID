//
//  HIDDriverProjectApp.swift
//  HIDDriverProject
//
//  Created by Dirt Plantman on 2025-02-16.
//

import SwiftUI

@main
struct HIDDriverProjectApp: App {
    var body: some Scene {
        WindowGroup {
            ContentView(joystick: Joystick(id: 1))
                .onReceive(NotificationCenter.default.publisher(for: NSApplication.willTerminateNotification)) { _ in
                    // This code will be executed just before the app terminates
                    print("closing")
                }
        }
    }
}
