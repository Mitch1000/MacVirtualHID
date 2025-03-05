//
//  OpenVirtualHIDApp.swift
//  OpenVirtualHID
//
//  Created by Dirt Plantman on 2025-03-04.
//

import SwiftUI

@main
struct OpenVirtualHIDApp: App {
  var body: some Scene {
      WindowGroup {
          #if os(macOS)
          DriverLoadingView()
          #else
          DriverLoadingView_iOS()
          #endif
      }
  }
}
