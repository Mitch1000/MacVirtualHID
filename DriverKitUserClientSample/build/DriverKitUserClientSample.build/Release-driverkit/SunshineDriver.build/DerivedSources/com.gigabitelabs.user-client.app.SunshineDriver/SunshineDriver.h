/* iig(DriverKit-286) generated from SunshineDriver.iig */

/* SunshineDriver.iig:1-14 */
/*
See LICENSE folder for this sample’s licensing information.

Abstract:
The IOKit Interface Generator file that declares the driver's methods.
*/

#ifndef SunshineDriver_h
#define SunshineDriver_h

#include <Availability.h>
#include <DriverKit/IOUserClient.h>  /* .iig include */
#include <DriverKit/IOTimerDispatchSource.h>  /* .iig include */

/* source class SunshineDriver SunshineDriver.iig:15-64 */

#if __DOCUMENTATION__
#define KERNEL IIG_KERNEL

class SunshineDriver: public IOUserClient
{
protected:
    // These are the standard methods that use an insecure process to respond to external messages.
    kern_return_t HandleExternalScalar(IOUserClientMethodArguments* arguments) LOCALONLY;
    kern_return_t HandleExternalStruct(IOUserClientMethodArguments* arguments) LOCALONLY;
    kern_return_t MethodCreate(char *name,
                             int name_len,
                             char *report_descriptor,
                             int report_descriptor_len,
                             char *serial_number,
                             int serial_number_len,
                             int vendor_id,
                             int product_id
                             ) LOCALONLY;


    // These are the static functions called by our IOUserClientMethodDispatch in our more secure means of checking external messages.
    static kern_return_t StaticHandleExternalCheckedScalar(OSObject* target, void* reference, IOUserClientMethodArguments* arguments) LOCALONLY;
    static kern_return_t StaticHandleExternalCheckedStruct(OSObject* target, void* reference, IOUserClientMethodArguments* arguments) LOCALONLY;

    // There are the local functions called from the static callbacks of the same name.
    kern_return_t HandleExternalCheckedScalar(void* reference, IOUserClientMethodArguments* arguments) LOCALONLY;
    kern_return_t HandleExternalCheckedStruct(void* reference, IOUserClientMethodArguments* arguments) LOCALONLY;

    // Provide a means to register our async callback with the dext.
    static kern_return_t StaticRegisterAsyncCallback(OSObject* target, void* reference, IOUserClientMethodArguments* arguments) LOCALONLY;
    kern_return_t RegisterAsyncCallback(void* reference, IOUserClientMethodArguments* arguments) LOCALONLY;

    // This flow shows what it's like to make a request that fires off an asynchronous action and then makes a callback.
    // While a driver typically uses the callback from the method that assigns the callback, it's also equally viable to retain the callback and re-use it.
    // If appropriate, a single function could even call the callback multiple times.
    static kern_return_t StaticHandleAsyncRequest(OSObject* target, void* reference, IOUserClientMethodArguments* arguments) LOCALONLY;
    kern_return_t HandleAsyncRequest(void* reference, IOUserClientMethodArguments* arguments) LOCALONLY;

    void PrintExtendedErrorInfo(kern_return_t ret) LOCALONLY;


public:
    virtual bool init(void) override;
    virtual kern_return_t Start(IOService* provider) override;
    virtual kern_return_t Stop(IOService* provider) override;
    virtual void free(void) override;

    virtual kern_return_t NewUserClient(uint32_t type, IOUserClient** userClient) override;
    virtual kern_return_t ExternalMethod(uint64_t selector, IOUserClientMethodArguments* arguments, const IOUserClientMethodDispatch* dispatch, OSObject* target, void* reference) override;

    // This function is called to simulate a device taking some time to complete an action and calling back the dext asynchronously.
    virtual void SimulatedAsyncEvent(OSAction* action, uint64_t time) TYPE(IOTimerDispatchSource::TimerOccurred);

};

#undef KERNEL
#else /* __DOCUMENTATION__ */

/* generated class SunshineDriver SunshineDriver.iig:15-64 */

#define SunshineDriver_SimulatedAsyncEvent_ID            0x3900dfb243a57f40ULL

#define SunshineDriver_Start_Args \
        IOService * provider

#define SunshineDriver_Stop_Args \
        IOService * provider

#define SunshineDriver_NewUserClient_Args \
        uint32_t type, \
        IOUserClient ** userClient

#define SunshineDriver_SimulatedAsyncEvent_Args \
        OSAction * action, \
        uint64_t time

#define SunshineDriver_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(SunshineDriver * self, const IORPC rpc);\
\
    kern_return_t\
    HandleExternalScalar(\
        IOUserClientMethodArguments * arguments);\
\
    kern_return_t\
    HandleExternalStruct(\
        IOUserClientMethodArguments * arguments);\
\
    kern_return_t\
    MethodCreate(\
        char * name,\
        int name_len,\
        char * report_descriptor,\
        int report_descriptor_len,\
        char * serial_number,\
        int serial_number_len,\
        int vendor_id,\
        int product_id);\
\
    static kern_return_t\
    StaticHandleExternalCheckedScalar(\
        OSObject * target,\
        void * reference,\
        IOUserClientMethodArguments * arguments);\
\
    static kern_return_t\
    StaticHandleExternalCheckedStruct(\
        OSObject * target,\
        void * reference,\
        IOUserClientMethodArguments * arguments);\
\
    kern_return_t\
    HandleExternalCheckedScalar(\
        void * reference,\
        IOUserClientMethodArguments * arguments);\
\
    kern_return_t\
    HandleExternalCheckedStruct(\
        void * reference,\
        IOUserClientMethodArguments * arguments);\
\
    static kern_return_t\
    StaticRegisterAsyncCallback(\
        OSObject * target,\
        void * reference,\
        IOUserClientMethodArguments * arguments);\
\
    kern_return_t\
    RegisterAsyncCallback(\
        void * reference,\
        IOUserClientMethodArguments * arguments);\
\
    static kern_return_t\
    StaticHandleAsyncRequest(\
        OSObject * target,\
        void * reference,\
        IOUserClientMethodArguments * arguments);\
\
    kern_return_t\
    HandleAsyncRequest(\
        void * reference,\
        IOUserClientMethodArguments * arguments);\
\
    void\
    PrintExtendedErrorInfo(\
        kern_return_t ret);\
\
    kern_return_t\
    CreateActionSimulatedAsyncEvent(size_t referenceSize, OSAction ** action);\
\
\
protected:\
    /* _Impl methods */\
\
    kern_return_t\
    Start_Impl(IOService_Start_Args);\
\
    kern_return_t\
    Stop_Impl(IOService_Stop_Args);\
\
    kern_return_t\
    NewUserClient_Impl(IOService_NewUserClient_Args);\
\
    void\
    SimulatedAsyncEvent_Impl(SunshineDriver_SimulatedAsyncEvent_Args);\
\
\
public:\
    /* _Invoke methods */\
\


#define SunshineDriver_KernelMethods \
\
protected:\
    /* _Impl methods */\
\


#define SunshineDriver_VirtualMethods \
\
public:\
\
    virtual bool\
    init(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual void\
    free(\
) APPLE_KEXT_OVERRIDE;\
\
    virtual kern_return_t\
    ExternalMethod(\
        uint64_t selector,\
        IOUserClientMethodArguments * arguments,\
        const IOUserClientMethodDispatch * dispatch,\
        OSObject * target,\
        void * reference) APPLE_KEXT_OVERRIDE;\
\


#if !KERNEL

extern OSMetaClass          * gSunshineDriverMetaClass;
extern const OSClassLoadInformation SunshineDriver_Class;

class SunshineDriverMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

#if !KERNEL

class SunshineDriverInterface : public OSInterface
{
public:
};

struct SunshineDriver_IVars;
struct SunshineDriver_LocalIVars;

class SunshineDriver : public IOUserClient, public SunshineDriverInterface
{
#if !KERNEL
    friend class SunshineDriverMetaClass;
#endif /* !KERNEL */

#if !KERNEL
public:
#ifdef SunshineDriver_DECLARE_IVARS
SunshineDriver_DECLARE_IVARS
#else /* SunshineDriver_DECLARE_IVARS */
    union
    {
        SunshineDriver_IVars * ivars;
        SunshineDriver_LocalIVars * lvars;
    };
#endif /* SunshineDriver_DECLARE_IVARS */
#endif /* !KERNEL */

#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gSunshineDriverMetaClass; };
#endif /* KERNEL */

    using super = IOUserClient;

#if !KERNEL
    SunshineDriver_Methods
    SunshineDriver_VirtualMethods
#endif /* !KERNEL */

};
#endif /* !KERNEL */


#define OSAction_SunshineDriver_SimulatedAsyncEvent_Methods \
\
public:\
\
    virtual kern_return_t\
    Dispatch(const IORPC rpc) APPLE_KEXT_OVERRIDE;\
\
    static kern_return_t\
    _Dispatch(OSAction_SunshineDriver_SimulatedAsyncEvent * self, const IORPC rpc);\
\
\
protected:\
    /* _Impl methods */\
\
\
public:\
    /* _Invoke methods */\
\


#define OSAction_SunshineDriver_SimulatedAsyncEvent_KernelMethods \
\
protected:\
    /* _Impl methods */\
\


#define OSAction_SunshineDriver_SimulatedAsyncEvent_VirtualMethods \
\
public:\
\


#if !KERNEL

extern OSMetaClass          * gOSAction_SunshineDriver_SimulatedAsyncEventMetaClass;
extern const OSClassLoadInformation OSAction_SunshineDriver_SimulatedAsyncEvent_Class;

class OSAction_SunshineDriver_SimulatedAsyncEventMetaClass : public OSMetaClass
{
public:
    virtual kern_return_t
    New(OSObject * instance) override;
    virtual kern_return_t
    Dispatch(const IORPC rpc) override;
};

#endif /* !KERNEL */

class OSAction_SunshineDriver_SimulatedAsyncEventInterface : public OSInterface
{
public:
};

struct OSAction_SunshineDriver_SimulatedAsyncEvent_IVars;
struct OSAction_SunshineDriver_SimulatedAsyncEvent_LocalIVars;

class __attribute__((availability(driverkit,introduced=20,message="Type-safe OSAction factory methods are available in DriverKit 20 and newer"))) OSAction_SunshineDriver_SimulatedAsyncEvent : public OSAction, public OSAction_SunshineDriver_SimulatedAsyncEventInterface
{
#if KERNEL
    OSDeclareDefaultStructorsWithDispatch(OSAction_SunshineDriver_SimulatedAsyncEvent);
#endif /* KERNEL */

#if !KERNEL
    friend class OSAction_SunshineDriver_SimulatedAsyncEventMetaClass;
#endif /* !KERNEL */

public:
#ifdef OSAction_SunshineDriver_SimulatedAsyncEvent_DECLARE_IVARS
OSAction_SunshineDriver_SimulatedAsyncEvent_DECLARE_IVARS
#else /* OSAction_SunshineDriver_SimulatedAsyncEvent_DECLARE_IVARS */
    union
    {
        OSAction_SunshineDriver_SimulatedAsyncEvent_IVars * ivars;
        OSAction_SunshineDriver_SimulatedAsyncEvent_LocalIVars * lvars;
    };
#endif /* OSAction_SunshineDriver_SimulatedAsyncEvent_DECLARE_IVARS */
#if !KERNEL
    static OSMetaClass *
    sGetMetaClass() { return gOSAction_SunshineDriver_SimulatedAsyncEventMetaClass; };
    virtual const OSMetaClass *
    getMetaClass() const APPLE_KEXT_OVERRIDE { return gOSAction_SunshineDriver_SimulatedAsyncEventMetaClass; };
#endif /* KERNEL */

    using super = OSAction;

#if !KERNEL
    OSAction_SunshineDriver_SimulatedAsyncEvent_Methods
#endif /* !KERNEL */

    OSAction_SunshineDriver_SimulatedAsyncEvent_VirtualMethods
};

#endif /* !__DOCUMENTATION__ */

/* SunshineDriver.iig:66- */
#endif /* SunshineDriver_h */
