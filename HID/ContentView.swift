//
//  ContentView.swift
//  WindowApp
//
//  Created by Dirt Plantman on 2025-02-03.
//

import SwiftUI



struct ContentView: View {
    @ObservedObject var joystick: Joystick
    @State private var textInput: String = ""

    
    var body: some View {

        
        VStack {
            Text("Press Buttons")
            
            HStack {
                Group {

                    RoundedRectangle(cornerRadius: 2.0)
                        .fill(Color(.orange))
                        .stroke(Color(.systemMint), lineWidth: 20)
                        .frame(width: 12, height: 12)
                        .padding([.horizontal], 50)
                        .padding([.top, .bottom], 20)
     
                    .simultaneousGesture(
                        DragGesture(minimumDistance: 0).onChanged({ _ in
                            print("press start")
                            joystick.pressAButton()
                        })
                        .onEnded({ _ in
                            print("press end")
                            joystick.reset()
                        })
                    )
                }
            }
            Button("Search Hardware Joystick") {
                Task {
                    print("Searching")
                    do {
                      try await joystick.deviceSearch(productName: "MAGIC-NS")
                    }
                    catch {
                        print("Search failed")
                    }
                }
            }
            Button("Search Virtual Joystick") {
                Task {
                    print("Searching")
                    do {
                        try await joystick.deviceSearch(productName: "DualShock 4")
                    }
                    catch {
                        print("Search failed")
                    }
                }
            }
//            Text("\(isPressed ? "Button Pressed" : "Button Released")")
//                .padding(.top, 40)
        }
        .padding()
    }
}

#Preview {
    ContentView(joystick: Joystick(id: 1))
}

