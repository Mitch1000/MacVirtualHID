//
//  ContentView.swift
//  HIDDriverProject
//
//  Created by Dirt Plantman on 2025-02-16.
//

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

            
            Button("Open IO Service") {
                Task {
                    print("Searching")
                    do {
                        try await joystick.openService(productName: Joystick.VIRTGAMEPAD_NAME)
                    }
                    catch {
                        print("Search failed")
                    }
                }
            }
            
            Button("Create Virtual Joystick") {
                Task {
                    print("Creating Joystick")
                    do {
                        try await joystick.callCreateMethod()
                    }
                    catch {
                        print("Create failed")
                    }
                }
            }.padding(.top, 40)
            
            Button("Destroy Virtual Joystick") {
                Task {
                    print("Destroying Joystick")
                    do {
                        try await joystick.callDestroyMethod()
                    }
                    catch {
                        print("Create failed")
                    }
                }
            }.padding(.top, 20)
            
            Button("Start Updating Gamepad") {
                Task {
                    print("Updating Joystick")
                    do {
                        try await joystick.updateGamepad()
                    }
                    catch {
                        print("Create failed")
                    }
                }
            }.padding(.top, 20)
            
            Button("Close Connect") {
                Task {
                    print("Closing Connection")
                    joystick.freeGamePad()
                }
            }.padding(.top, 40)
//            Text("\(isPressed ? "Button Pressed" : "Button Released")")
//                .padding(.top, 40)
        }
        .padding()
    }
}

#Preview {
    ContentView(joystick: Joystick(id: 1))
}

