#include "OpenVirtualHIDDriverRoot.h"
#include "IOBufferMemoryDescriptorUtility.hpp"
#include <HIDDriverKit/IOHIDDeviceKeys.h>
#include <HIDDriverKit/IOHIDUsageTables.h>
#include <os/log.h>

#define Log(fmt, ...) os_log(OS_LOG_DEFAULT, "OpenVirtualHIDDriver - " fmt "\n", ##__VA_ARGS__)

struct OpenVirtualHIDDriverRoot_IVars {
};

bool OpenVirtualHIDDriverRoot::init() {
  Log(" init");

  if (!super::init()) {
    return false;
  }

  ivars = IONewZero(OpenVirtualHIDDriverRoot_IVars, 1);
  if (ivars == nullptr) {
    return false;
  }

  return true;
}

void OpenVirtualHIDDriverRoot::free() {
  Log(" free");

  IOSafeDeleteNULL(ivars, OpenVirtualHIDDriverRoot_IVars, 1);

  super::free();
}

kern_return_t IMPL(OpenVirtualHIDDriverRoot, Start) {
  Log(" Start");

  auto kr = Start(provider, SUPERDISPATCH);
  if (kr != kIOReturnSuccess) {
    Log(" super::Start failed: 0x%x", kr);
    Stop(provider, SUPERDISPATCH);
    return kr;
  }

  RegisterService();

  return kIOReturnSuccess;
}

kern_return_t IMPL(OpenVirtualHIDDriverRoot, Stop) {
  Log(" Stop");

  return Stop(provider, SUPERDISPATCH);
}

kern_return_t IMPL(OpenVirtualHIDDriverRoot, NewUserClient) {
  Log(" NewUserClient type:%d", type);

  IOService* client;

  auto kr = Create(this, "UserClientProperties", &client);
  if (kr != kIOReturnSuccess) {
    Log(" IOService::Create failed: 0x%x", kr);
    return kr;
  }

  Log(" UserClient is created");

  *userClient = OSDynamicCast(IOUserClient, client);
  if (!*userClient) {
    Log(" OSDynamicCast failed");
    client->release();
    return kIOReturnError;
  }

  return kIOReturnSuccess;
}

