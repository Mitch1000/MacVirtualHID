/* iig(DriverKit-286 Nov 11 2024 03:06:27) generated from SunshineDriver.iig */

#undef	IIG_IMPLEMENTATION
#define	IIG_IMPLEMENTATION 	SunshineDriver.iig

#if KERNEL
#include <libkern/c++/OSString.h>
#else
#include <DriverKit/DriverKit.h>
#endif /* KERNEL */
#include <DriverKit/IOReturn.h>
#include "SunshineDriver.h"


#if __has_builtin(__builtin_load_member_function_pointer)
#define SimpleMemberFunctionCast(cfnty, self, func) (cfnty)__builtin_load_member_function_pointer(self, func)
#else
#define SimpleMemberFunctionCast(cfnty, self, func) ({ union { typeof(func) memfun; cfnty cfun; } pair; pair.memfun = func; pair.cfun; })
#endif


#if !KERNEL
extern OSMetaClass * gOSContainerMetaClass;
extern OSMetaClass * gOSDataMetaClass;
extern OSMetaClass * gOSNumberMetaClass;
extern OSMetaClass * gOSBooleanMetaClass;
extern OSMetaClass * gOSDictionaryMetaClass;
extern OSMetaClass * gOSArrayMetaClass;
extern OSMetaClass * gOSSetMetaClass;
extern OSMetaClass * gOSOrderedSetMetaClass;
extern OSMetaClass * gOSStringMetaClass;
extern OSMetaClass * gIOServiceStateNotificationDispatchSourceMetaClass;
extern OSMetaClass * gIOMemoryMapMetaClass;
extern OSMetaClass * gOSAction_IOUserClient_KernelCompletionMetaClass;
extern OSMetaClass * gOSAction_SunshineDriver_SimulatedAsyncEventMetaClass;
#endif /* !KERNEL */

#if !KERNEL

#define SunshineDriver_QueueNames  "" \
    "\037IOUserClientQueueExternalMethod"

#define SunshineDriver_MethodNames  "" \
    "\017_ExternalMethod"

#define SunshineDriverMetaClass_MethodNames  ""

struct OSClassDescription_SunshineDriver_t
{
    OSClassDescription base;
    uint64_t           methodOptions[2 * 1];
    uint64_t           metaMethodOptions[2 * 0];
    char               queueNames[sizeof(SunshineDriver_QueueNames)];
    char               methodNames[sizeof(SunshineDriver_MethodNames)];
    char               metaMethodNames[sizeof(SunshineDriverMetaClass_MethodNames)];
};

const struct OSClassDescription_SunshineDriver_t
OSClassDescription_SunshineDriver =
{
    .base =
    {
        .descriptionSize         = sizeof(OSClassDescription_SunshineDriver_t),
        .name                    = "SunshineDriver",
        .superName               = "IOUserClient",
        .methodOptionsSize       = 2 * sizeof(uint64_t) * 1,
        .methodOptionsOffset     = __builtin_offsetof(struct OSClassDescription_SunshineDriver_t, methodOptions),
        .metaMethodOptionsSize   = 2 * sizeof(uint64_t) * 0,
        .metaMethodOptionsOffset = __builtin_offsetof(struct OSClassDescription_SunshineDriver_t, metaMethodOptions),
        .queueNamesSize       = sizeof(SunshineDriver_QueueNames),
        .queueNamesOffset     = __builtin_offsetof(struct OSClassDescription_SunshineDriver_t, queueNames),
        .methodNamesSize         = sizeof(SunshineDriver_MethodNames),
        .methodNamesOffset       = __builtin_offsetof(struct OSClassDescription_SunshineDriver_t, methodNames),
        .metaMethodNamesSize     = sizeof(SunshineDriverMetaClass_MethodNames),
        .metaMethodNamesOffset   = __builtin_offsetof(struct OSClassDescription_SunshineDriver_t, metaMethodNames),
        .flags                   = 0*kOSClassCanRemote,
    },
    .methodOptions =
    {
        IOUserClient__ExternalMethod_ID,
        0x0000000000000000,
    },
    .metaMethodOptions =
    {
    },
    .queueNames      = SunshineDriver_QueueNames,
    .methodNames     = SunshineDriver_MethodNames,
    .metaMethodNames = SunshineDriverMetaClass_MethodNames,
};

OSMetaClass * gSunshineDriverMetaClass;

static kern_return_t
SunshineDriver_New(OSMetaClass * instance);

const OSClassLoadInformation
SunshineDriver_Class = 
{
    .description       = &OSClassDescription_SunshineDriver.base,
    .metaPointer       = &gSunshineDriverMetaClass,
    .version           = 1,
    .instanceSize      = sizeof(SunshineDriver),

    .New               = &SunshineDriver_New,
};

extern const void * const
gSunshineDriver_Declaration;
const void * const
gSunshineDriver_Declaration
__attribute__((visibility("hidden"),section("__DATA_CONST,__osclassinfo,regular,no_dead_strip"),no_sanitize("address")))
    = &SunshineDriver_Class;

static kern_return_t
SunshineDriver_New(OSMetaClass * instance)
{
    if (!new(instance) SunshineDriverMetaClass) return (kIOReturnNoMemory);
    return (kIOReturnSuccess);
}

kern_return_t
SunshineDriverMetaClass::New(OSObject * instance)
{
    if (!new(instance) SunshineDriver) return (kIOReturnNoMemory);
    return (kIOReturnSuccess);
}

#endif /* !KERNEL */

kern_return_t
SunshineDriver::Dispatch(const IORPC rpc)
{
    return _Dispatch(this, rpc);
}

kern_return_t
SunshineDriver::_Dispatch(SunshineDriver * self, const IORPC rpc)
{
    kern_return_t ret = kIOReturnUnsupported;
    IORPCMessage * msg = IORPCMessageFromMach(rpc.message, false);

    switch (msg->msgid)
    {
        case IOService_Start_ID:
        {
            ret = IOService::Start_Invoke(rpc, self, SimpleMemberFunctionCast(IOService::Start_Handler, *self, &SunshineDriver::Start_Impl));
            break;
        }
        case IOService_Stop_ID:
        {
            ret = IOService::Stop_Invoke(rpc, self, SimpleMemberFunctionCast(IOService::Stop_Handler, *self, &SunshineDriver::Stop_Impl));
            break;
        }
        case IOService_NewUserClient_ID:
#if !KERNEL
        if (self->IsRemote())
        {
            ret = self->OSMetaClassBase::Dispatch(rpc);
            break;
        }
        else
#endif /* !KERNEL */
        {
            ret = IOService::NewUserClient_Invoke(rpc, self, SimpleMemberFunctionCast(IOService::NewUserClient_Handler, *self, &SunshineDriver::NewUserClient_Impl));
            break;
        }
        case SunshineDriver_SimulatedAsyncEvent_ID:
#if !KERNEL
        if (self->IsRemote())
        {
            ret = self->OSMetaClassBase::Dispatch(rpc);
            break;
        }
        else
#endif /* !KERNEL */
        {
            ret = IOTimerDispatchSource::TimerOccurred_Invoke(rpc, self, SimpleMemberFunctionCast(IOTimerDispatchSource::TimerOccurred_Handler, *self, &SunshineDriver::SimulatedAsyncEvent_Impl), OSTypeID(OSAction_SunshineDriver_SimulatedAsyncEvent));
            break;
        }

        default:
            ret = IOUserClient::_Dispatch(self, rpc);
            break;
    }

    return (ret);
}

#if KERNEL
kern_return_t
SunshineDriver::MetaClass::Dispatch(const IORPC rpc)
{
#else /* KERNEL */
kern_return_t
SunshineDriverMetaClass::Dispatch(const IORPC rpc)
{
#endif /* !KERNEL */

    kern_return_t ret = kIOReturnUnsupported;
    IORPCMessage * msg = IORPCMessageFromMach(rpc.message, false);

    switch (msg->msgid)
    {

        default:
            ret = OSMetaClassBase::Dispatch(rpc);
            break;
    }

    return (ret);
}

kern_return_t
SunshineDriver::CreateActionSimulatedAsyncEvent(size_t referenceSize, OSAction ** action)
{
    kern_return_t ret;

#if defined(IOKIT_ENABLE_SHARED_PTR)
    OSSharedPtr<OSString>
#else /* defined(IOKIT_ENABLE_SHARED_PTR) */
    OSString *
#endif /* !defined(IOKIT_ENABLE_SHARED_PTR) */
    typeName = OSString::withCString("OSAction_SunshineDriver_SimulatedAsyncEvent");
    if (!typeName) {
        return kIOReturnNoMemory;
    }
    ret = OSAction_SunshineDriver_SimulatedAsyncEvent::CreateWithTypeName(this,
                           SunshineDriver_SimulatedAsyncEvent_ID,
                           IOTimerDispatchSource_TimerOccurred_ID,
                           referenceSize,
#if defined(IOKIT_ENABLE_SHARED_PTR)
                           typeName.get(),
#else /* defined(IOKIT_ENABLE_SHARED_PTR) */
                           typeName,
#endif /* !defined(IOKIT_ENABLE_SHARED_PTR) */
                           action);

#if !defined(IOKIT_ENABLE_SHARED_PTR)
    typeName->release();
#endif /* !defined(IOKIT_ENABLE_SHARED_PTR) */
    return (ret);
}

#if KERNEL
OSDefineMetaClassAndStructors(OSAction_SunshineDriver_SimulatedAsyncEvent, OSAction);
#endif /* KERNEL */

#if !KERNEL

#define OSAction_SunshineDriver_SimulatedAsyncEvent_QueueNames  ""

#define OSAction_SunshineDriver_SimulatedAsyncEvent_MethodNames  ""

#define OSAction_SunshineDriver_SimulatedAsyncEventMetaClass_MethodNames  ""

struct OSClassDescription_OSAction_SunshineDriver_SimulatedAsyncEvent_t
{
    OSClassDescription base;
    uint64_t           methodOptions[2 * 0];
    uint64_t           metaMethodOptions[2 * 0];
    char               queueNames[sizeof(OSAction_SunshineDriver_SimulatedAsyncEvent_QueueNames)];
    char               methodNames[sizeof(OSAction_SunshineDriver_SimulatedAsyncEvent_MethodNames)];
    char               metaMethodNames[sizeof(OSAction_SunshineDriver_SimulatedAsyncEventMetaClass_MethodNames)];
};

 __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer")))
const struct OSClassDescription_OSAction_SunshineDriver_SimulatedAsyncEvent_t
OSClassDescription_OSAction_SunshineDriver_SimulatedAsyncEvent =
{
    .base =
    {
        .descriptionSize         = sizeof(OSClassDescription_OSAction_SunshineDriver_SimulatedAsyncEvent_t),
        .name                    = "OSAction_SunshineDriver_SimulatedAsyncEvent",
        .superName               = "OSAction",
        .methodOptionsSize       = 2 * sizeof(uint64_t) * 0,
        .methodOptionsOffset     = __builtin_offsetof(struct OSClassDescription_OSAction_SunshineDriver_SimulatedAsyncEvent_t, methodOptions),
        .metaMethodOptionsSize   = 2 * sizeof(uint64_t) * 0,
        .metaMethodOptionsOffset = __builtin_offsetof(struct OSClassDescription_OSAction_SunshineDriver_SimulatedAsyncEvent_t, metaMethodOptions),
        .queueNamesSize       = sizeof(OSAction_SunshineDriver_SimulatedAsyncEvent_QueueNames),
        .queueNamesOffset     = __builtin_offsetof(struct OSClassDescription_OSAction_SunshineDriver_SimulatedAsyncEvent_t, queueNames),
        .methodNamesSize         = sizeof(OSAction_SunshineDriver_SimulatedAsyncEvent_MethodNames),
        .methodNamesOffset       = __builtin_offsetof(struct OSClassDescription_OSAction_SunshineDriver_SimulatedAsyncEvent_t, methodNames),
        .metaMethodNamesSize     = sizeof(OSAction_SunshineDriver_SimulatedAsyncEventMetaClass_MethodNames),
        .metaMethodNamesOffset   = __builtin_offsetof(struct OSClassDescription_OSAction_SunshineDriver_SimulatedAsyncEvent_t, metaMethodNames),
        .flags                   = 0*kOSClassCanRemote,
    },
    .methodOptions =
    {
    },
    .metaMethodOptions =
    {
    },
    .queueNames      = OSAction_SunshineDriver_SimulatedAsyncEvent_QueueNames,
    .methodNames     = OSAction_SunshineDriver_SimulatedAsyncEvent_MethodNames,
    .metaMethodNames = OSAction_SunshineDriver_SimulatedAsyncEventMetaClass_MethodNames,
};

 __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer")))
OSMetaClass * gOSAction_SunshineDriver_SimulatedAsyncEventMetaClass;

 __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer")))
static kern_return_t
OSAction_SunshineDriver_SimulatedAsyncEvent_New(OSMetaClass * instance);

 __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer")))
const OSClassLoadInformation
OSAction_SunshineDriver_SimulatedAsyncEvent_Class = 
{
    .description       = &OSClassDescription_OSAction_SunshineDriver_SimulatedAsyncEvent.base,
    .metaPointer       = &gOSAction_SunshineDriver_SimulatedAsyncEventMetaClass,
    .version           = 1,
    .instanceSize      = sizeof(OSAction_SunshineDriver_SimulatedAsyncEvent),

    .New               = &OSAction_SunshineDriver_SimulatedAsyncEvent_New,
};

 __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer")))
extern const void * const
gOSAction_SunshineDriver_SimulatedAsyncEvent_Declaration;
 __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer")))
const void * const
gOSAction_SunshineDriver_SimulatedAsyncEvent_Declaration
 __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer")))
__attribute__((visibility("hidden"),section("__DATA_CONST,__osclassinfo,regular,no_dead_strip"),no_sanitize("address")))
    = &OSAction_SunshineDriver_SimulatedAsyncEvent_Class;

 __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer")))
static kern_return_t
OSAction_SunshineDriver_SimulatedAsyncEvent_New(OSMetaClass * instance)
{
    if (!new(instance) OSAction_SunshineDriver_SimulatedAsyncEventMetaClass) return (kIOReturnNoMemory);
    return (kIOReturnSuccess);
}

 __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer")))
kern_return_t
OSAction_SunshineDriver_SimulatedAsyncEventMetaClass::New(OSObject * instance)
{
    if (!new(instance) OSAction_SunshineDriver_SimulatedAsyncEvent) return (kIOReturnNoMemory);
    return (kIOReturnSuccess);
}

#endif /* !KERNEL */

kern_return_t
OSAction_SunshineDriver_SimulatedAsyncEvent::Dispatch(const IORPC rpc)
{
    return _Dispatch(this, rpc);
}

kern_return_t
OSAction_SunshineDriver_SimulatedAsyncEvent::_Dispatch(OSAction_SunshineDriver_SimulatedAsyncEvent * self, const IORPC rpc)
{
    kern_return_t ret = kIOReturnUnsupported;
    IORPCMessage * msg = IORPCMessageFromMach(rpc.message, false);

    switch (msg->msgid)
    {

        default:
            ret = OSAction::_Dispatch(self, rpc);
            break;
    }

    return (ret);
}

#if KERNEL
kern_return_t
OSAction_SunshineDriver_SimulatedAsyncEvent::MetaClass::Dispatch(const IORPC rpc)
{
#else /* KERNEL */
kern_return_t
OSAction_SunshineDriver_SimulatedAsyncEventMetaClass::Dispatch(const IORPC rpc)
{
#endif /* !KERNEL */

    kern_return_t ret = kIOReturnUnsupported;
    IORPCMessage * msg = IORPCMessageFromMach(rpc.message, false);

    switch (msg->msgid)
    {

        default:
            ret = OSMetaClassBase::Dispatch(rpc);
            break;
    }

    return (ret);
}



