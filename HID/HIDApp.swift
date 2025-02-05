//
//  WindowAppApp.swift
//  WindowApp
//
//  Created by Dirt Plantman on 2025-02-03.
//

import SwiftUI


@main
struct HIDApp: App {
    var body: some Scene {
        WindowGroup {
            ContentView(joystick: Joystick(id: 1))
        }
    }
 
}

