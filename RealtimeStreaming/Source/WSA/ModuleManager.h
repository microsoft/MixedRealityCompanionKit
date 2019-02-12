/* Header file automatically generated from ModuleManager.idl */
/*
 * File built with Microsoft(R) MIDLRT Compiler Engine Version 10.00.0223 
 */

#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include <rpc.h>
#include <rpcndr.h>

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include <windows.h>
#include <ole2.h>
#endif /*COM_NO_WINDOWS_H*/
#ifndef __ModuleManager_h__
#define __ModuleManager_h__
#ifndef __ModuleManager_p_h__
#define __ModuleManager_p_h__


#pragma once

// Ensure that the setting of the /ns_prefix command line switch is consistent for all headers.
// If you get an error from the compiler indicating "warning C4005: 'CHECK_NS_PREFIX_STATE': macro redefinition", this
// indicates that you have included two different headers with different settings for the /ns_prefix MIDL command line switch
#if !defined(DISABLE_NS_PREFIX_CHECKS)
#define CHECK_NS_PREFIX_STATE "always"
#endif // !defined(DISABLE_NS_PREFIX_CHECKS)


#pragma push_macro("MIDL_CONST_ID")
#undef MIDL_CONST_ID
#define MIDL_CONST_ID const __declspec(selectany)


//  API Contract Inclusion Definitions
#if !defined(SPECIFIC_API_CONTRACT_DEFINITIONS)
#if !defined(WINDOWS_APPLICATIONMODEL_CALLS_CALLSPHONECONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_CALLS_CALLSPHONECONTRACT_VERSION 0x40000
#endif // defined(WINDOWS_APPLICATIONMODEL_CALLS_CALLSPHONECONTRACT_VERSION)

#if !defined(WINDOWS_FOUNDATION_FOUNDATIONCONTRACT_VERSION)
#define WINDOWS_FOUNDATION_FOUNDATIONCONTRACT_VERSION 0x30000
#endif // defined(WINDOWS_FOUNDATION_FOUNDATIONCONTRACT_VERSION)

#if !defined(WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION)
#define WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION 0x70000
#endif // defined(WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION)

#if !defined(WINDOWS_NETWORKING_SOCKETS_CONTROLCHANNELTRIGGERCONTRACT_VERSION)
#define WINDOWS_NETWORKING_SOCKETS_CONTROLCHANNELTRIGGERCONTRACT_VERSION 0x30000
#endif // defined(WINDOWS_NETWORKING_SOCKETS_CONTROLCHANNELTRIGGERCONTRACT_VERSION)

#if !defined(WINDOWS_PHONE_PHONECONTRACT_VERSION)
#define WINDOWS_PHONE_PHONECONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_PHONE_PHONECONTRACT_VERSION)

#if !defined(WINDOWS_PHONE_PHONEINTERNALCONTRACT_VERSION)
#define WINDOWS_PHONE_PHONEINTERNALCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_PHONE_PHONEINTERNALCONTRACT_VERSION)

#if !defined(WINDOWS_UI_WEBUI_CORE_WEBUICOMMANDBARCONTRACT_VERSION)
#define WINDOWS_UI_WEBUI_CORE_WEBUICOMMANDBARCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_UI_WEBUI_CORE_WEBUICOMMANDBARCONTRACT_VERSION)

#endif // defined(SPECIFIC_API_CONTRACT_DEFINITIONS)


// Header files for imported files
#include "oaidl.h"
#include "ocidl.h"
#include "Windows.Foundation.h"
#include "RealtimeStreaming.h"

#if defined(__cplusplus) && !defined(CINTERFACE)
#if defined(__MIDL_USE_C_ENUM)
#define MIDL_ENUM enum
#else
#define MIDL_ENUM enum class
#endif
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            interface IModule;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule ABI::RealtimeStreaming::Plugin::IModule

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            interface IModuleFactory;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory ABI::RealtimeStreaming::Plugin::IModuleFactory

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            interface IModuleManager;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager ABI::RealtimeStreaming::Plugin::IModuleManager

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__


namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            class Module;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */


namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            class ModuleManager;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */



/*
 *
 * Interface RealtimeStreaming.Plugin.IModule
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.Module
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IModule[] = L"RealtimeStreaming.Plugin.IModule";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            /* [object, uuid("2edeb3fb-8008-4a13-b8d3-7081fd11443f"), exclusiveto, version] */
            MIDL_INTERFACE("2edeb3fb-8008-4a13-b8d3-7081fd11443f")
            IModule : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE Shutdown(void) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IModule=_uuidof(IModule);
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModule;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Plugin.IModuleFactory
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.Module
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IModuleFactory[] = L"RealtimeStreaming.Plugin.IModuleFactory";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            /* [object, uuid("1d81174e-2dc0-11e9-b210-d663bd873d93"), exclusiveto, version] */
            MIDL_INTERFACE("1d81174e-2dc0-11e9-b210-d663bd873d93")
            IModuleFactory : public IInspectable
            {
            public:
                
            };

            extern MIDL_CONST_ID IID & IID_IModuleFactory=_uuidof(IModuleFactory);
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Plugin.Module
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Plugin.IModule ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Plugin_Module_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Plugin_Module_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Plugin_Module[] = L"RealtimeStreaming.Plugin.Module";
#endif


/*
 *
 * Interface RealtimeStreaming.Plugin.IModuleManager
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.ModuleManager
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IModuleManager[] = L"RealtimeStreaming.Plugin.IModuleManager";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            /* [object, uuid("ed7ca192-2e9d-11e9-b210-d663bd873d93"), exclusiveto, version] */
            MIDL_INTERFACE("ed7ca192-2e9d-11e9-b210-d663bd873d93")
            IModuleManager : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE AddModule(
                    /* [in] */ABI::RealtimeStreaming::Plugin::IModule * pluginModule,
                    /* [out, retval] */unsigned int * result
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE GetModule(
                    /* [in] */unsigned int moduleHandle,
                    /* [out, retval] */ABI::RealtimeStreaming::Plugin::IModule * * result
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE ReleaseModule(
                    /* [in] */unsigned int moduleHandle
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IModuleManager=_uuidof(IModuleManager);
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Plugin.ModuleManager
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Plugin.IModuleManager ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Plugin_ModuleManager_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Plugin_ModuleManager_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Plugin_ModuleManager[] = L"RealtimeStreaming.Plugin.ModuleManager";
#endif


#else // !defined(__cplusplus)
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIModule __x_ABI_CRealtimeStreaming_CPlugin_CIModule;

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory;

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager;

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__




/*
 *
 * Interface RealtimeStreaming.Plugin.IModule
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.Module
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IModule[] = L"RealtimeStreaming.Plugin.IModule";
/* [object, uuid("2edeb3fb-8008-4a13-b8d3-7081fd11443f"), exclusiveto, version] */
typedef struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *Shutdown )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CPlugin_CIModuleVtbl;

interface __x_ABI_CRealtimeStreaming_CPlugin_CIModule
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_Shutdown(This) \
    ( (This)->lpVtbl->Shutdown(This) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModule;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Plugin.IModuleFactory
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.Module
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IModuleFactory[] = L"RealtimeStreaming.Plugin.IModuleFactory";
/* [object, uuid("1d81174e-2dc0-11e9-b210-d663bd873d93"), exclusiveto, version] */
typedef struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactoryVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactoryVtbl;

interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactoryVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Plugin.Module
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Plugin.IModule ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Plugin_Module_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Plugin_Module_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Plugin_Module[] = L"RealtimeStreaming.Plugin.Module";
#endif


/*
 *
 * Interface RealtimeStreaming.Plugin.IModuleManager
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.ModuleManager
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IModuleManager[] = L"RealtimeStreaming.Plugin.IModuleManager";
/* [object, uuid("ed7ca192-2e9d-11e9-b210-d663bd873d93"), exclusiveto, version] */
typedef struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *AddModule )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CPlugin_CIModule * pluginModule,
        /* [out, retval] */unsigned int * result
        );
    HRESULT ( STDMETHODCALLTYPE *GetModule )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
        /* [in] */unsigned int moduleHandle,
        /* [out, retval] */__x_ABI_CRealtimeStreaming_CPlugin_CIModule * * result
        );
    HRESULT ( STDMETHODCALLTYPE *ReleaseModule )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
        /* [in] */unsigned int moduleHandle
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerVtbl;

interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_AddModule(This,pluginModule,result) \
    ( (This)->lpVtbl->AddModule(This,pluginModule,result) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_GetModule(This,moduleHandle,result) \
    ( (This)->lpVtbl->GetModule(This,moduleHandle,result) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_ReleaseModule(This,moduleHandle) \
    ( (This)->lpVtbl->ReleaseModule(This,moduleHandle) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Plugin.ModuleManager
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Plugin.IModuleManager ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Plugin_ModuleManager_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Plugin_ModuleManager_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Plugin_ModuleManager[] = L"RealtimeStreaming.Plugin.ModuleManager";
#endif


#endif // defined(__cplusplus)
#pragma pop_macro("MIDL_CONST_ID")
#endif // __ModuleManager_p_h__

#endif // __ModuleManager_h__
