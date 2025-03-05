/*
See LICENSE folder for this sample’s licensing information.

Abstract:
A DriverKit null driver implementation that only logs interactions with its client.
*/

#include <os/log.h>

#include <string>
#include <array>
#include <DriverKit/IOLib.h>
#include <DriverKit/IOMemoryMap.h>
#include <DriverKit/IOTimerDispatchSource.h>
#include <DriverKit/IOUserClient.h>
#include <DriverKit/OSData.h>
#include <DriverKit/OSCollections.h>
#include <DriverKit/OSString.h>
#include "IOBufferMemoryDescriptorUtility.hpp"
#include "OpenVirtualHIDDevice.h"


 #include <HIDDriverKit/HIDDriverKit.h>


#include <time.h>

#include "OpenVirtualHIDClient.h"

// This log to makes it easier to parse out individual logs from the driver, since all logs will be prefixed with the same word/phrase.
// DriverKit logging has no logging levels; some developers might want to prefix errors differently than info messages.
// Another option is to #define a "debug" case where some log messages only exist when doing a debug build.
// To search for logs from this driver, use either: `sudo dmesg | grep OpenVirtualHIDClient` or use Console.app search to find messages that start with "OpenVirtualHIDClient -".
#define Log(fmt, ...) os_log(OS_LOG_DEFAULT, "OpenVirtualHID - " fmt "\n", ##__VA_ARGS__)
namespace {
typedef enum
{
    ExternalMethodType_Scalar = 0,
    ExternalMethodType_Struct = 1,
    ExternalMethodType_CheckedScalar = 2,
    ExternalMethodType_CheckedStruct = 3,
    ExternalMethodType_RegisterAsyncCallback = 4,
    ExternalMethodType_AsyncRequest = 5,
    NumberOfExternalMethods = 2// Has to be last
} ExternalMethodType;

#define GAMEPAD_ZEROBTNS 0
#define GAMEPAD_BTN_A (1 << 0)
#define GAMEPAD_BTN_B (1 << 1)
#define GAMEPAD_BTN_X (1 << 2)
#define GAMEPAD_BTN_Y (1 << 3)
#define GAMEPAD_BTN_L (1 << 4)
#define GAMEPAD_BTN_R (1 << 5)
#define GAMEPAD_BTN_LT (1 << 6)
#define GAMEPAD_BTN_RT (1 << 7)
#define GAMEPAD_BTN_BACK (1 << 8)
#define GAMEPAD_BTN_START (1 << 9)
#define GAMEPAD_BTN_LS (1 << 10)
#define GAMEPAD_BTN_RS (1 << 11)
#define GAMEPAD_BTN_UP (1 << 12)
#define GAMEPAD_BTN_DOWN (1 << 13)
#define GAMEPAD_BTN_LEFT (1 << 14)
#define GAMEPAD_BTN_RIGHT (1 << 15)


struct gamepad_report_t {
  uint8_t  reportId;                                 // Report ID = 0x01 (1)
                                                     // Collection: CA:GamePad
  uint8_t  GD_GamePadX;                              // Usage 0x00010030: X, Value = 0 to 255
  uint8_t  GD_GamePadY;                              // Usage 0x00010031: Y, Value = 0 to 255
  uint8_t  GD_GamePadZ;                              // Usage 0x00010032: Z, Value = 0 to 255
  uint8_t  GD_GamePadRz;                             // Usage 0x00010035: Rz, Value = 0 to 255
  uint8_t  GD_GamePadHatSwitch : 4;                  // Usage 0x00010039: Hat switch, Value = 0 to 7, Physical = Value x 45 in degrees
  uint8_t  BTN_GamePadButton1 : 1;                   // Usage 0x00090001: Button 1 Primary/trigger, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton2 : 1;                   // Usage 0x00090002: Button 2 Secondary, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton3 : 1;                   // Usage 0x00090003: Button 3 Tertiary, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton4 : 1;                   // Usage 0x00090004: Button 4, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton5 : 1;                   // Usage 0x00090005: Button 5, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton6 : 1;                   // Usage 0x00090006: Button 6, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton7 : 1;                   // Usage 0x00090007: Button 7, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton8 : 1;                   // Usage 0x00090008: Button 8, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton9 : 1;                   // Usage 0x00090009: Button 9, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton10 : 1;                  // Usage 0x0009000A: Button 10, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton11 : 1;                  // Usage 0x0009000B: Button 11, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton12 : 1;                  // Usage 0x0009000C: Button 12, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton13 : 1;                  // Usage 0x0009000D: Button 13, Value = 0 to 1, Physical = Value x 315
  uint8_t  BTN_GamePadButton14 : 1;                  // Usage 0x0009000E: Button 14, Value = 0 to 1, Physical = Value x 315
  uint8_t  VEN_GamePad0020 : 6;                      // Usage 0xFF000020: , Value = 0 to 127, Physical = Value x 315 / 127
  uint8_t  GD_GamePadRx;                             // Usage 0x00010033: Rx, Value = 0 to 255, Physical = Value x 21 / 17
  uint8_t  GD_GamePadRy;                             // Usage 0x00010034: Ry, Value = 0 to 255, Physical = Value x 21 / 17
  uint8_t  VEN_GamePad0021[54];                      // Usage 0xFF000021: , Value = 0 to 255, Physical = Value x 21 / 1j
};

std::string joinName(std::string baseName, uint64_t id)
{
  std::string idString = std::to_string(id);
  return baseName + " " + idString;
}

kern_return_t createIOMemoryDescriptor(IOUserClientMethodArguments* arguments, IOMemoryDescriptor** memory)
{
  if (!memory) {
    Log("Create Memory Descriptor: Bad Memory Argument");
    return kIOReturnBadArgument;
  }

  *memory = nullptr;

  OSData *structureInput = arguments->structureInput;
  Log("Create Memory Descriptor: %b", !!structureInput);
  
  if (structureInput) {
    Log("Create Memory Descriptor: Struct Argument");
    auto kr = IOBufferMemoryDescriptorUtility::createWithBytes(arguments->structureInput->getBytesNoCopy(),
                                                               arguments->structureInput->getLength(),
                                                               memory);
    if (kr != kIOReturnSuccess) {
      
      return kr;
    }
  } else if (arguments->structureInputDescriptor) {

    *memory = arguments->structureInputDescriptor;
    

    (*memory)->retain();
  }
  

  return kIOReturnSuccess;
}
} // namespace

struct OpenVirtualHIDClient_IVars {
  uint32_t keyboardVendorId;
  uint32_t keyboardProductId;
  OSDictionary *devices;
  OpenVirtualHIDDevice *newDevice;
  std::string baseName = "Dualshock 4";
};

bool OpenVirtualHIDClient::init() {
  Log(" init");

  if (!super::init()) {
    return false;
  }

  ivars = IONewZero(OpenVirtualHIDClient_IVars, 1);
  if (ivars == nullptr) {
    return false;
  }

  ivars->devices = OSDictionary::withCapacity(4);


  if (!ivars->devices) {
      Log("Unable to inizialize HID devices dictionary.");
      return false;
  }

  return true;
}

void OpenVirtualHIDClient::free() {
  Log(" free");
  if (ivars->devices) {
    ivars->devices->release();
  }

  
  OSSafeReleaseNULL(ivars->devices);
  
  OSSafeReleaseNULL(ivars->newDevice);
  
  IOSafeDeleteNULL(ivars, OpenVirtualHIDClient_IVars, 1);

  super::free();
}

kern_return_t IMPL(OpenVirtualHIDClient, Start) {
  Log(" Start");

  {
    auto kr = Start(provider, SUPERDISPATCH);
    if (kr != kIOReturnSuccess) {
      Log(" Start failed");
      return kr;
    }
  }

  return kIOReturnSuccess;
}

kern_return_t IMPL(OpenVirtualHIDClient, Stop) {
  Log(" Stop");

  return Stop(provider, SUPERDISPATCH);
}

using byte = unsigned char ;

template<typename T> std::array<byte, sizeof(T)> to_bytes(const T& object)
{
    std::array< byte, sizeof(T) > bytes ;

    const byte* begin = reinterpret_cast<const byte*>(std::addressof(object));
    const byte* end = begin + sizeof(T) ;
    std::copy( begin, end, std::begin(bytes) ) ;

    return bytes;
}


kern_return_t OpenVirtualHIDClient::ExternalMethod(uint64_t selector,
                                                    IOUserClientMethodArguments* arguments,
                                                    const IOUserClientMethodDispatch* dispatch,
                                                    OSObject* target,
                                                    void* reference)
{
    kern_return_t ret = kIOReturnSuccess;
    Log("Switch Selector Value %i", int(selector));



    switch (selector)
    {
        case 0: {
          return CreateDevice(arguments);
        }
            
        case 1: {
          return DeviceReadyCheck(arguments);
        }
       

        case 2: {
          return UpdateDeviceInputs(arguments);
        }

        default:
        {
            ret = kIOReturnBadArgument;
        } break;
    }

    return ret;
  

  return kIOReturnBadArgument;
}

kern_return_t OpenVirtualHIDClient::UpdateDeviceInputs(IOUserClientMethodArguments* arguments)
{
  Log("Switch Selector at 2 -> Updating Device");
  uint64_t deviceId = arguments->scalarInput[0];
  std::string name = joinName(ivars->baseName, deviceId);
  OSString *key = nullptr;
  key = OSString::withCString(name.c_str());

  Log("Switch Selector at 2 -> Updating Device: '%{public}s'", name.c_str());
  
  OpenVirtualHIDDevice *device = nullptr;
  device = (OpenVirtualHIDDevice *)ivars->devices->getObject(key);
  
  if (device) {
    IOMemoryDescriptor* memory = nullptr;
    struct gamepad_report_t gamepad;
    int buttons = int(arguments->scalarInput[2]);
    gamepad.reportId = 0x01;
    
    gamepad.GD_GamePadRx = 0;
    gamepad.GD_GamePadRy = 0;
    
    
    gamepad.BTN_GamePadButton1 = (buttons & GAMEPAD_BTN_Y) ? 1 : 0; // A
    gamepad.BTN_GamePadButton2 = (buttons & GAMEPAD_BTN_B) ? 1 : 0;
    gamepad.BTN_GamePadButton3 = (buttons & GAMEPAD_BTN_A) ? 1 : 0; // X
    gamepad.BTN_GamePadButton4 = (buttons & GAMEPAD_BTN_X) ? 1 : 0; // Y
    gamepad.BTN_GamePadButton5 = (buttons & GAMEPAD_BTN_L) ? 1 : 0;
    gamepad.BTN_GamePadButton6 = (buttons & GAMEPAD_BTN_R) ? 1 : 0;
    gamepad.BTN_GamePadButton7 = (buttons & GAMEPAD_BTN_LT) ? 1 : 0;
    gamepad.BTN_GamePadButton8 = (buttons & GAMEPAD_BTN_RT) ? 1 : 0;
    gamepad.BTN_GamePadButton9 = (buttons & GAMEPAD_BTN_BACK) ? 1 : 0;
    gamepad.BTN_GamePadButton10 = (buttons & GAMEPAD_BTN_START) ? 1 : 0;
    gamepad.BTN_GamePadButton11 = (buttons & GAMEPAD_BTN_LS) ? 1 : 0;
    gamepad.BTN_GamePadButton12 = (buttons & GAMEPAD_BTN_RS) ? 1 : 0;
    
    gamepad.GD_GamePadHatSwitch = 0x0F;
    
    gamepad.GD_GamePadHatSwitch = buttons == GAMEPAD_BTN_UP ? 0 : gamepad.GD_GamePadHatSwitch;
    gamepad.GD_GamePadHatSwitch = buttons == GAMEPAD_BTN_DOWN ? 4 : gamepad.GD_GamePadHatSwitch;
    gamepad.GD_GamePadHatSwitch = buttons == GAMEPAD_BTN_LEFT ? 6 : gamepad.GD_GamePadHatSwitch;
    gamepad.GD_GamePadHatSwitch = buttons == GAMEPAD_BTN_RIGHT ? 2 : gamepad.GD_GamePadHatSwitch;
    
    
    gamepad.GD_GamePadZ = int(arguments->scalarInput[3]);
    gamepad.GD_GamePadRz = int(arguments->scalarInput[4]);
    gamepad.GD_GamePadX = int(arguments->scalarInput[5]);
    gamepad.GD_GamePadY = int(arguments->scalarInput[6]);
    gamepad.GD_GamePadRx = int(arguments->scalarInput[7]);
    gamepad.GD_GamePadRy = int(arguments->scalarInput[8]);
    
    
    
    size_t dataSize = sizeof(struct gamepad_report_t);
    
    
    unsigned char* myBytes = reinterpret_cast<unsigned char*>(&gamepad);
    
    
    OSData* data = OSData::withBytes(myBytes, dataSize);
    
    
    
    auto kr = IOBufferMemoryDescriptorUtility::createWithBytes(data->getBytesNoCopy(0, dataSize),
                                                               dataSize,
                                                               &memory);
    
    
    if (kr == kIOReturnSuccess) {
      Log("Memory Created Successfuly");
      
      kr = device->postReport(memory);
      if (kr == kIOReturnSuccess) {
        Log("Post report succeeded");
      }
      OSSafeReleaseNULL(memory);
    }
    
    OSSafeReleaseNULL(data);
    OSSafeReleaseNULL(key);
    return kr;
  }
  OSSafeReleaseNULL(key);
  return kIOReturnError;;
}

kern_return_t OpenVirtualHIDClient::DeviceReadyCheck(IOUserClientMethodArguments* arguments)
{
  OSString *key = nullptr;
  uint64_t deviceId = arguments->scalarInput[0];
  std::string name = joinName(ivars->baseName, deviceId);
  key = OSString::withCString(name.c_str());

  Log("Switch Selector at 1 -> Device Ready: '%{public}s'", name.c_str());
  Log("Device Ready -> Output Count %i", arguments->scalarOutputCount);
  Log("Device Ready -> Output at 0 %i", int(arguments->scalarOutput[0]));

  Log("Device Ready -> Devices Count: %i", ivars->devices->getCount());

  OpenVirtualHIDDevice *device = nullptr;
  device = (OpenVirtualHIDDevice *)ivars->devices->getObject(key);

  Log("Device Ready -> Devices Exists %i", !device);

  if (arguments->scalarOutput && arguments->scalarOutputCount > 0) {
    arguments->scalarOutput[0] = (device && device->getReady());
    Log("Device Ready Success");
    OSSafeReleaseNULL(key);
    return kIOReturnSuccess;
  }
  OSSafeReleaseNULL(key);
  Log("Device Ready Fail");
  return kIOReturnError;
}

kern_return_t OpenVirtualHIDClient::CreateDevice(IOUserClientMethodArguments* arguments)
{
  uint64_t deviceId = arguments->scalarInput[0];
  std::string name = joinName(ivars->baseName, deviceId);

  Log("Switch Selector at 0 -> Creating Device '%{public}s'", name.c_str());
  OSString *key = nullptr;
  key = OSString::withCString(name.c_str());

  Log("Device Create -> Devices Count: %i", ivars->devices->getCount());

  OpenVirtualHIDDevice *device = nullptr;

  device = (OpenVirtualHIDDevice *)ivars->devices->getObject(key);

  Log("Device Create -> Device Exists %i", !device);

  if (!device) {
    if (arguments->scalarInputCount > 6) {
      ivars->keyboardVendorId = static_cast<uint32_t>(arguments->scalarInput[4]);
      ivars->keyboardProductId = static_cast<uint32_t>(arguments->scalarInput[5]);
    }
    

  
    IOService* client;
  
    auto kr = Create(this, "VirtualHIDDeviceProperties", &client);
    if (kr != kIOReturnSuccess) {
      OSSafeReleaseNULL(key);
      Log(" IOService::Create failed: 0x%x", kr);
      return kr;
    }
  
    ivars->newDevice = OSDynamicCast(OpenVirtualHIDDevice, client);
    
    if (!ivars->newDevice) {
      Log(" OSDynamicCast failed");
      OSSafeReleaseNULL(client);
      OSSafeReleaseNULL(key);
      return kIOReturnError;
    }
  
  
    ivars->devices->setObject(key, ivars->newDevice);
  }
  OSSafeReleaseNULL(key);
  OSSafeReleaseNULL(key);
  return kIOReturnSuccess;
}

uint32_t IMPL(OpenVirtualHIDClient, getKeyboardVendorId) {
  return ivars->keyboardVendorId;
}

uint32_t IMPL(OpenVirtualHIDClient, getKeyboardProductId) {
  return ivars->keyboardProductId;
}

//}
