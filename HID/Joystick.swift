//
//  Joystick.swift
//  HID
//
//  Created by Dirt Plantman on 2025-02-03.
import CoreFoundation
import SwiftUI
import CoreHID
import IOKit
import Darwin.Mach


enum MyError: Error {
    case runtimeError(String)
}



final class Delegate : HIDVirtualDeviceDelegate {
    // A handler for system requests to send data to the device.
    func hidVirtualDevice(_ device: HIDVirtualDevice, receivedSetReportRequestOfType type: HIDReportType, id: HIDReportID?, data: Data) throws {
        print("Device received a set report request for report type:\(type) id:\(String(describing: id)) with data:[\(data.map { String(format: "%02x", $0) }.joined(separator: " "))]")
    }


    // A handler for system requests to query data from the device.
    func hidVirtualDevice(_ device: HIDVirtualDevice, receivedGetReportRequestOfType type: HIDReportType, id: HIDReportID?, maxSize: size_t) throws -> Data {
        print("Device received a get report request for report type:\(type) id:\(String(describing: id))")
        print(maxSize, type, device)
        return Data([00, 00, 08, 80, 67, 80, 80, 00])
    }
    
}

class Joystick: ObservableObject {
    var device: HIDVirtualDevice?
    var virtualInterator: io_iterator_t = 0;
    var virtgamepadService: io_service_t = 0;
    var virtgamepadConnect: io_connect_t = 0;
    let VIRTGAMEPAD_NAME = "DualShock 4"    // gamepad name
    let VIRTGAMEPAD_SN = "CUH-ZCT1x"        // serial number
    let VIRTGAMEPAD_INPUT_COUNT = 8         // device arguments count
    let SERVICE_NAME = "it_unbit_foohid"    // virthid service id
    let FOOHID_CREATE = 0                   // create selector
    let FOOHID_SEND = 2
    var virtgamepadInput = [UInt64](repeating: 0, count: 8)
    var isAButtonPress = false



    let joystickDescriptor: Data = Data([0x05, 0x01, 0x09, 0x05, 0xA1, 0x01, 0xA1, 0x00, 0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x09, 0x33, 0x09, 0x34, 0x16, 0x00, 0x80, 0x26, 0xFF, 0x7F, 0x75, 0x10, 0x95, 0x04, 0x81, 0x02, 0x05, 0x01, 0x09, 0x32, 0x09, 0x35, 0x15, 0x00, 0x46, 0xFF, 0x00, 0x75, 0x08, 0x95, 0x02, 0x81, 0x02, 0x05, 0x09, 0x19, 0x01, 0x29, 0x10, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x10, 0x81, 0x02, 0xC0, 0xC0
    ])

    
    var id: Int
    
    init(id: Int) {
        self.id = id
        Task {
            do {
                self.device = try await setup()
                try await updateGamepad()
            }
            catch {
                print("Could not setup joystick")
            }
        }
    }
    
    func setup() async throws -> HIDVirtualDevice {

         // Copied from actual controller
         let joystickDescriptor: Data = Data([0x05, 0x01, 0x09, 0x05, 0xa1, 0x01, 0x15, 0x00, 0x25, 0x01, 0x35, 0x00, 0x45, 0x01, 0x75, 0x01, 0x95, 0x0e, 0x05, 0x09, 0x19, 0x01, 0x29, 0x0e, 0x81, 0x02, 0x95, 0x02, 0x81, 0x01, 0x05, 0x01, 0x25, 0x07, 0x46, 0x3b, 0x01, 0x75, 0x04, 0x95, 0x01, 0x65, 0x14, 0x09, 0x39, 0x81, 0x42, 0x65, 0x00, 0x95, 0x01, 0x81, 0x01, 0x26, 0xff, 0x00, 0x46, 0xff, 0x00, 0x09, 0x30, 0x09, 0x31, 0x09, 0x32, 0x09, 0x35, 0x75, 0x08, 0x95, 0x04, 0x81, 0x02, 0x75, 0x08, 0x95, 0x01, 0x81, 0x01, 0xc0])
            //Keyboard
//        let keyboardDescriptor: Data = Data([0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x95, 0x01, 0x75, 0x08, 0x81, 0x01, 0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 0x95, 0x05, 0x75, 0x01, 0x91, 0x02, 0x95, 0x01, 0x75, 0x03, 0x91, 0x01, 0x05, 0x07, 0x19, 0x00, 0x2A, 0xFF, 0x00, 0x95, 0x05, 0x75, 0x08, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x81, 0x00, 0x05, 0xFF, 0x09, 0x03, 0x75, 0x08, 0x95, 0x01, 0x81, 0x02, 0xC0])

        let productName = VIRTGAMEPAD_NAME
        
        let transport = HIDDeviceTransport.usb
//        let properties = HIDVirtualDevice.Properties(descriptor: keyboardDescriptor, vendorID: 1, product: productName)
        let properties = HIDVirtualDevice.Properties(
            descriptor: joystickDescriptor,
            vendorID: 0x20d6,
            productID: 0xa710,
            transport: transport,
            product: productName,
            manufacturer: "Sony",
            modelNumber: "nil",
            serialNumber: VIRTGAMEPAD_SN,
            locationID: 0x40130000
        )
  

        // try await deviceSearch()
        guard let device = HIDVirtualDevice(properties: properties) else {
            print("Device creation failed")
            throw MyError.runtimeError("Device creation failed")
        }
        
        await device.activate(delegate: Delegate())

    
        return device
    }

    func deviceSearch(productName: String) async throws -> Void {
        print("deviceSearch")

        let matchingCriteria = HIDDeviceManager.DeviceMatchingCriteria(primaryUsage: .genericDesktop(.gamepad), product: productName)
//        let matchingCriteria = HIDDeviceManager.DeviceMatchingCriteria(primaryUsage: .genericDesktop(.keyboard), product: productName)
//        let matchingCriteria = HIDDeviceManager.DeviceMatchingCriteria(primaryUsage: .genericDesktop(.gamepad), product: "MAGIC-NS")
//        let matchingCriteria = HIDDeviceManager.DeviceMatchingCriteria(primaryUsage: .genericDesktop(.keyboard), vendorID: 0x4d9, product: "USB Keyboard")
        let manager = HIDDeviceManager()
        let notifications = await manager.monitorNotifications(matchingCriteria: [matchingCriteria])
        for try await notification in notifications {
            switch notification {
            case .deviceMatched(let deviceReference):
//                let optionBits :IOOptionBits = 0x00
//                IOHIDDeviceOpen(deviceReference, optionBits)
                try await handleIO(deviceReference: deviceReference)
//                  try await client.seizeDevice()
//                let notifications = await client.monitorNotifications(reportIDsToMonitor: [HIDReportID.allReports], elementsToMonitor: [])
//                try await setupInputReporting(client: client)
//                do {
//                    print(client)
//                    let modelNumber = await client.modelNumber
//                    let primaryUsage = await client.primaryUsage
//                    let deviceUsages = await client.deviceUsages
//                    let descriptor = await client.descriptor
//                    let transport = await client.transport
//                    let elements = await client.elements
//                    
//                   
//                    
//                    print("modelNumber", modelNumber)
//                    print("primaryUsage", primaryUsage)
//                    print("deviceUsages", deviceUsages)
//                    print("descriptor",  descriptor.map { String(format: "%02x", $0) }.joined(separator: ", "))
//                    print("descriptor",  descriptor)
//                    print("transport", transport)
////                    handleIO(serviceName: productName)
//                    
////                    let report = try await client.dispatchGetReportRequest(type: .input)
//                    
////                    try await setupInputReporting(client: client)
////                    print("Get report data: [\(report.map { String(format: "%02x", $0) }.joined(separator: " "))]")
//                }
//                catch {
//                    print("Could not get device report")
//                }
//

                print("A device was found.", deviceReference, "\n \n")
                    
            case .deviceRemoved(let deviceReference):
                print("A device was removed.", deviceReference)
            
                    
            default:
                print("Nothing turned up in device search")
                continue
            }
        }
    }

    func sendButtonPress(data: Data) async throws {
        guard let pressDevice = self.device else {
            print("Cannot send button press to virtual gamepad")
            return
        }
        try await pressDevice.dispatchInputReport(data: data, timestamp: SuspendingClock.now)
    }
    
    func handleIO(deviceReference: HIDDeviceClient.DeviceReference) async throws -> Int {
        print("Handle IO")

        
        // let dic :CFDictionary = IOServiceMatching("IOHIDResource")
        let dic :CFDictionary = IOServiceMatching("AppleUserHIDDevice")
        
        
        var ret = IOServiceGetMatchingServices(kIOMasterPortDefault, dic, &virtualInterator)
        
        if(ret != KERN_SUCCESS) {
            print("IO Service Error")
            return 1
        }
        
        var found = 0;
        var doLoop = true
        while(doLoop) {
            print("while loop")
            virtgamepadService = IOIteratorNext(virtualInterator)
            print("found service", String(virtgamepadService, radix: 16))
            if virtgamepadService == IO_OBJECT_NULL {
                doLoop = false
                break
            }
            guard let device = IOHIDDeviceCreate(kCFAllocatorDefault, virtgamepadService) else {
                throw MyError.runtimeError("Coult not create IO Device")
            }

                
            let openResult = IOHIDDeviceOpen(device, IOOptionBits(0x00));
            print("openResult", openResult, "\n")
            
            guard let client = HIDDeviceClient(deviceReference: deviceReference) else {
                throw MyError.runtimeError("Could not create client")
            }
            
            try await setupInputReporting(client: client)
            
//            var bufferSize = 1024
//            let buffer = UnsafeMutablePointer<Int8>.allocate(capacity: bufferSize)
//            let reportId: CFIndex = 0
//            let report = IOHIDDeviceGetReport(device, kIOHIDReportTypeInput, reportId, buffer, &bufferSize)
            
//            print("report", report)
//            print("buffer", buffer)
//            print("device", device, "\n")
//            print("openResult", openResult, "\n")
            
//            ret = IOServiceOpen(virtgamepadService, mach_task_self_, 0, &virtgamepadConnect);
//            print("open", ret)
//            
//            buffer.deallocate()
            
          if(ret == KERN_SUCCESS) {
            found = 1;
            break;
          }
          
            let closeResult = IOHIDDeviceClose(device, IOOptionBits(0x00));
//          IOObjectRelease(virtgamepadService)
        }
        
        
        IOObjectRelease(virtualInterator);
        print("found", found)
        if (found != 1) {
            print("Unable to find gamepad service")
            return 1
        }
        
        let deviceName = strdup(VIRTGAMEPAD_NAME)
        virtgamepadInput[0] = UInt64(bitPattern: Int64(Int(bitPattern: deviceName)))
        virtgamepadInput[1] = UInt64(strlen(deviceName!))

        // Store the report descriptor and its length
        // Define the gamepad report descriptor as an array of UInt8
        let gamepad_report_descriptor: [UInt8] = [0x05, 0x01]

        // Convert the array to an unsafe pointer and get its raw pointer (memory address)
        let pointer = UnsafeRawPointer(gamepad_report_descriptor)

        // Convert the pointer to a UInt64 value
        virtgamepadInput[2] = UInt64(bitPattern: Int64(Int(bitPattern: pointer)))
        virtgamepadInput[3] = UInt64(joystickDescriptor.count)

        // Convert the serial number to a C string and store its address
        let serialNumber = strdup(VIRTGAMEPAD_SN)
        virtgamepadInput[4] = UInt64(bitPattern: Int64(Int(bitPattern: serialNumber)))
        virtgamepadInput[5] = UInt64(strlen(serialNumber!))

        // Store the vendor ID and product ID
        virtgamepadInput[6] = 0x054c
        virtgamepadInput[7] = 0x05c4
        print(ret)
        
        let scalarOutput: UnsafeMutablePointer<UInt64>? = nil
        let inputStruct: UnsafeMutablePointer<UInt32>? = nil
        let outputCount: UnsafeMutablePointer<UInt32>? = nil
        
        print("gameservice", virtgamepadConnect)

        let result = IOConnectCallScalarMethod(
            virtgamepadConnect, // Connection to the I/O Kit driver
            UInt32(FOOHID_CREATE), // Selector for the method to call
            virtgamepadInput, // Array of scalar input values
            UInt32(VIRTGAMEPAD_INPUT_COUNT),// Number of scalar input values
            scalarOutput, // Pointer to store scalar output values (NULL in this case)
            outputCount // Pointer to store the number of output values (NULL in this case)
        )

        if result == KERN_SUCCESS {
            print("IOConnectCallScalarMethod succeeded")
        } else {
            print("IOConnectCallScalarMethod failed with error: \(result)")
        }
    
        print(ret)

        print(ret == KERN_SUCCESS)
        print (KERN_SUCCESS)
        print("IO HANDLE OVER")
        return 0
    }
    
    func getGamepadData() -> Data {
        let ABUTTON_PRESS_DATA: Data = Data([02, 00, 08, 80, 67, 80, 80, 00])
        let INITIAL_DATA: Data = Data([00, 00, 08, 80, 67, 80, 80, 00])
        
        return isAButtonPress ? ABUTTON_PRESS_DATA : INITIAL_DATA
    }
    
    func updateGamepad() async throws {
        while (true) {
            let seconds = 0.01
            try await Task.sleep(nanoseconds: UInt64(seconds * Double(NSEC_PER_SEC)))
            try await sendButtonPress(data: getGamepadData())
        }
    }
    
    func setupInputReporting(client: HIDDeviceClient) async throws {
        print("setup reporting", client)
        var previousReportData = Data([00, 00, 00, 00, 00, 00, 00, 00])
        for try await notification in await client.monitorNotifications(reportIDsToMonitor: [HIDReportID.allReports], elementsToMonitor: []) {
            switch notification {


            // Receive device input data.
            case .inputReport(let reportID, let reportData, let timestamp):
                if (reportData != previousReportData) {
                    previousReportData = reportData
                    print("Received input report ID:\(String(describing: reportID)) timestamp:\(timestamp) data:[\(reportData.map { String(format: "%02x", $0) }.joined(separator: " "))]")
                }

            // Receive a notification if another client seizes this device.
            case .deviceSeized:
                print("\(client) seized")


            // Receive a notification if another client releases this device.
            case .deviceUnseized:
                print("\(client) unseized")


            // Receive a notification if a person removes the client.
            case .deviceRemoved:
                print("\(client) removed")


            default:
                continue
            }
        }
    }
    
    func pressAButton() {
        isAButtonPress = true
    }
    
    func reset() {
        isAButtonPress = false
    }
    
}
