/* Header file automatically generated from DataBundleArgs.idl */
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
#ifndef __DataBundleArgs_h__
#define __DataBundleArgs_h__
#ifndef __DataBundleArgs_p_h__
#define __DataBundleArgs_p_h__


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
#if !defined(WINDOWS_APPLICATIONMODEL_ACTIVATION_ACTIVATEDEVENTSCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_ACTIVATION_ACTIVATEDEVENTSCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_ACTIVATION_ACTIVATEDEVENTSCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_ACTIVATION_ACTIVATIONCAMERASETTINGSCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_ACTIVATION_ACTIVATIONCAMERASETTINGSCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_ACTIVATION_ACTIVATIONCAMERASETTINGSCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_ACTIVATION_CONTACTACTIVATEDEVENTSCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_ACTIVATION_CONTACTACTIVATEDEVENTSCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_ACTIVATION_CONTACTACTIVATEDEVENTSCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_ACTIVATION_WEBUISEARCHACTIVATEDEVENTSCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_ACTIVATION_WEBUISEARCHACTIVATEDEVENTSCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_ACTIVATION_WEBUISEARCHACTIVATEDEVENTSCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_BACKGROUND_BACKGROUNDALARMAPPLICATIONCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_BACKGROUND_BACKGROUNDALARMAPPLICATIONCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_BACKGROUND_BACKGROUNDALARMAPPLICATIONCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_CALLS_BACKGROUND_CALLSBACKGROUNDCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_CALLS_BACKGROUND_CALLSBACKGROUNDCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_CALLS_BACKGROUND_CALLSBACKGROUNDCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_CALLS_CALLSPHONECONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_CALLS_CALLSPHONECONTRACT_VERSION 0x40000
#endif // defined(WINDOWS_APPLICATIONMODEL_CALLS_CALLSPHONECONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_CALLS_CALLSVOIPCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_CALLS_CALLSVOIPCONTRACT_VERSION 0x40000
#endif // defined(WINDOWS_APPLICATIONMODEL_CALLS_CALLSVOIPCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_CALLS_LOCKSCREENCALLCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_CALLS_LOCKSCREENCALLCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_CALLS_LOCKSCREENCALLCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_COMMUNICATIONBLOCKING_COMMUNICATIONBLOCKINGCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_COMMUNICATIONBLOCKING_COMMUNICATIONBLOCKINGCONTRACT_VERSION 0x20000
#endif // defined(WINDOWS_APPLICATIONMODEL_COMMUNICATIONBLOCKING_COMMUNICATIONBLOCKINGCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_FULLTRUSTAPPCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_FULLTRUSTAPPCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_FULLTRUSTAPPCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_SEARCH_SEARCHCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_SEARCH_SEARCHCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_SEARCH_SEARCHCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_STARTUPTASKCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_STARTUPTASKCONTRACT_VERSION 0x30000
#endif // defined(WINDOWS_APPLICATIONMODEL_STARTUPTASKCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_WALLET_WALLETCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_WALLET_WALLETCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_WALLET_WALLETCONTRACT_VERSION)

#if !defined(WINDOWS_DEVICES_PRINTERS_EXTENSIONS_EXTENSIONSCONTRACT_VERSION)
#define WINDOWS_DEVICES_PRINTERS_EXTENSIONS_EXTENSIONSCONTRACT_VERSION 0x20000
#endif // defined(WINDOWS_DEVICES_PRINTERS_EXTENSIONS_EXTENSIONSCONTRACT_VERSION)

#if !defined(WINDOWS_DEVICES_SMARTCARDS_SMARTCARDBACKGROUNDTRIGGERCONTRACT_VERSION)
#define WINDOWS_DEVICES_SMARTCARDS_SMARTCARDBACKGROUNDTRIGGERCONTRACT_VERSION 0x30000
#endif // defined(WINDOWS_DEVICES_SMARTCARDS_SMARTCARDBACKGROUNDTRIGGERCONTRACT_VERSION)

#if !defined(WINDOWS_DEVICES_SMARTCARDS_SMARTCARDEMULATORCONTRACT_VERSION)
#define WINDOWS_DEVICES_SMARTCARDS_SMARTCARDEMULATORCONTRACT_VERSION 0x60000
#endif // defined(WINDOWS_DEVICES_SMARTCARDS_SMARTCARDEMULATORCONTRACT_VERSION)

#if !defined(WINDOWS_DEVICES_SMS_LEGACYSMSAPICONTRACT_VERSION)
#define WINDOWS_DEVICES_SMS_LEGACYSMSAPICONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_DEVICES_SMS_LEGACYSMSAPICONTRACT_VERSION)

#if !defined(WINDOWS_FOUNDATION_FOUNDATIONCONTRACT_VERSION)
#define WINDOWS_FOUNDATION_FOUNDATIONCONTRACT_VERSION 0x30000
#endif // defined(WINDOWS_FOUNDATION_FOUNDATIONCONTRACT_VERSION)

#if !defined(WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION)
#define WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION 0x70000
#endif // defined(WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION)

#if !defined(WINDOWS_GAMING_INPUT_GAMINGINPUTPREVIEWCONTRACT_VERSION)
#define WINDOWS_GAMING_INPUT_GAMINGINPUTPREVIEWCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_GAMING_INPUT_GAMINGINPUTPREVIEWCONTRACT_VERSION)

#if !defined(WINDOWS_GLOBALIZATION_GLOBALIZATIONJAPANESEPHONETICANALYZERCONTRACT_VERSION)
#define WINDOWS_GLOBALIZATION_GLOBALIZATIONJAPANESEPHONETICANALYZERCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_GLOBALIZATION_GLOBALIZATIONJAPANESEPHONETICANALYZERCONTRACT_VERSION)

#if !defined(WINDOWS_MEDIA_CAPTURE_APPBROADCASTCONTRACT_VERSION)
#define WINDOWS_MEDIA_CAPTURE_APPBROADCASTCONTRACT_VERSION 0x20000
#endif // defined(WINDOWS_MEDIA_CAPTURE_APPBROADCASTCONTRACT_VERSION)

#if !defined(WINDOWS_MEDIA_CAPTURE_APPCAPTURECONTRACT_VERSION)
#define WINDOWS_MEDIA_CAPTURE_APPCAPTURECONTRACT_VERSION 0x40000
#endif // defined(WINDOWS_MEDIA_CAPTURE_APPCAPTURECONTRACT_VERSION)

#if !defined(WINDOWS_MEDIA_CAPTURE_APPCAPTUREMETADATACONTRACT_VERSION)
#define WINDOWS_MEDIA_CAPTURE_APPCAPTUREMETADATACONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_MEDIA_CAPTURE_APPCAPTUREMETADATACONTRACT_VERSION)

#if !defined(WINDOWS_MEDIA_CAPTURE_CAMERACAPTUREUICONTRACT_VERSION)
#define WINDOWS_MEDIA_CAPTURE_CAMERACAPTUREUICONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_MEDIA_CAPTURE_CAMERACAPTUREUICONTRACT_VERSION)

#if !defined(WINDOWS_MEDIA_CAPTURE_GAMEBARCONTRACT_VERSION)
#define WINDOWS_MEDIA_CAPTURE_GAMEBARCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_MEDIA_CAPTURE_GAMEBARCONTRACT_VERSION)

#if !defined(WINDOWS_MEDIA_DEVICES_CALLCONTROLCONTRACT_VERSION)
#define WINDOWS_MEDIA_DEVICES_CALLCONTROLCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_MEDIA_DEVICES_CALLCONTROLCONTRACT_VERSION)

#if !defined(WINDOWS_MEDIA_MEDIACONTROLCONTRACT_VERSION)
#define WINDOWS_MEDIA_MEDIACONTROLCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_MEDIA_MEDIACONTROLCONTRACT_VERSION)

#if !defined(WINDOWS_MEDIA_PROTECTION_PROTECTIONRENEWALCONTRACT_VERSION)
#define WINDOWS_MEDIA_PROTECTION_PROTECTIONRENEWALCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_MEDIA_PROTECTION_PROTECTIONRENEWALCONTRACT_VERSION)

#if !defined(WINDOWS_NETWORKING_CONNECTIVITY_WWANCONTRACT_VERSION)
#define WINDOWS_NETWORKING_CONNECTIVITY_WWANCONTRACT_VERSION 0x20000
#endif // defined(WINDOWS_NETWORKING_CONNECTIVITY_WWANCONTRACT_VERSION)

#if !defined(WINDOWS_NETWORKING_SOCKETS_CONTROLCHANNELTRIGGERCONTRACT_VERSION)
#define WINDOWS_NETWORKING_SOCKETS_CONTROLCHANNELTRIGGERCONTRACT_VERSION 0x30000
#endif // defined(WINDOWS_NETWORKING_SOCKETS_CONTROLCHANNELTRIGGERCONTRACT_VERSION)

#if !defined(WINDOWS_PHONE_PHONECONTRACT_VERSION)
#define WINDOWS_PHONE_PHONECONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_PHONE_PHONECONTRACT_VERSION)

#if !defined(WINDOWS_PHONE_PHONEINTERNALCONTRACT_VERSION)
#define WINDOWS_PHONE_PHONEINTERNALCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_PHONE_PHONEINTERNALCONTRACT_VERSION)

#if !defined(WINDOWS_SECURITY_ENTERPRISEDATA_ENTERPRISEDATACONTRACT_VERSION)
#define WINDOWS_SECURITY_ENTERPRISEDATA_ENTERPRISEDATACONTRACT_VERSION 0x50000
#endif // defined(WINDOWS_SECURITY_ENTERPRISEDATA_ENTERPRISEDATACONTRACT_VERSION)

#if !defined(WINDOWS_STORAGE_PROVIDER_CLOUDFILESCONTRACT_VERSION)
#define WINDOWS_STORAGE_PROVIDER_CLOUDFILESCONTRACT_VERSION 0x30000
#endif // defined(WINDOWS_STORAGE_PROVIDER_CLOUDFILESCONTRACT_VERSION)

#if !defined(WINDOWS_SYSTEM_ANDROMEDAPLACEHOLDERCONTRACT_VERSION)
#define WINDOWS_SYSTEM_ANDROMEDAPLACEHOLDERCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_SYSTEM_ANDROMEDAPLACEHOLDERCONTRACT_VERSION)

#if !defined(WINDOWS_SYSTEM_SYSTEMMANAGEMENTCONTRACT_VERSION)
#define WINDOWS_SYSTEM_SYSTEMMANAGEMENTCONTRACT_VERSION 0x60000
#endif // defined(WINDOWS_SYSTEM_SYSTEMMANAGEMENTCONTRACT_VERSION)

#if !defined(WINDOWS_UI_CORE_COREWINDOWDIALOGSCONTRACT_VERSION)
#define WINDOWS_UI_CORE_COREWINDOWDIALOGSCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_UI_CORE_COREWINDOWDIALOGSCONTRACT_VERSION)

#if !defined(WINDOWS_UI_VIEWMANAGEMENT_VIEWMANAGEMENTVIEWSCALINGCONTRACT_VERSION)
#define WINDOWS_UI_VIEWMANAGEMENT_VIEWMANAGEMENTVIEWSCALINGCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_UI_VIEWMANAGEMENT_VIEWMANAGEMENTVIEWSCALINGCONTRACT_VERSION)

#if !defined(WINDOWS_UI_WEBUI_CORE_WEBUICOMMANDBARCONTRACT_VERSION)
#define WINDOWS_UI_WEBUI_CORE_WEBUICOMMANDBARCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_UI_WEBUI_CORE_WEBUICOMMANDBARCONTRACT_VERSION)

#endif // defined(SPECIFIC_API_CONTRACT_DEFINITIONS)


// Header files for imported files
#include "oaidl.h"
#include "ocidl.h"
#include "RealtimeStreaming.h"
#include "Connection.h"
#include "DataBundle.h"

#if defined(__cplusplus) && !defined(CINTERFACE)
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IBundleReceivedArgs;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs ABI::RealtimeStreaming::Network::IBundleReceivedArgs

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IBundleReceivedArgsFactory;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory ABI::RealtimeStreaming::Network::IBundleReceivedArgsFactory

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_FWD_DEFINED__


#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IConnection;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection ABI::RealtimeStreaming::Network::IConnection

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__


namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class BundleReceivedArgs;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */



/*
 *
 * Interface RealtimeStreaming.Network.IBundleReceivedArgs
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.BundleReceivedArgs
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IBundleReceivedArgs[] = L"RealtimeStreaming.Network.IBundleReceivedArgs";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, uuid("26a177fe-2e53-11e9-b210-d663bd873d93"), exclusiveto, version] */
            MIDL_INTERFACE("26a177fe-2e53-11e9-b210-d663bd873d93")
            IBundleReceivedArgs : public IInspectable
            {
            public:
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_PayloadType(
                    /* [out, retval] */ABI::RealtimeStreaming::PayloadType * value
                    ) = 0;
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_DataConnection(
                    /* [out, retval] */ABI::RealtimeStreaming::Network::IConnection * * value
                    ) = 0;
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_Bundle(
                    /* [out, retval] */ABI::RealtimeStreaming::Network::IDataBundle * * value
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IBundleReceivedArgs=_uuidof(IBundleReceivedArgs);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IBundleReceivedArgsFactory
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.BundleReceivedArgs
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IBundleReceivedArgsFactory[] = L"RealtimeStreaming.Network.IBundleReceivedArgsFactory";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, uuid("9a172738-2e53-11e9-b210-d663bd873d93"), exclusiveto, version] */
            MIDL_INTERFACE("9a172738-2e53-11e9-b210-d663bd873d93")
            IBundleReceivedArgsFactory : public IInspectable
            {
            public:
                
            };

            extern MIDL_CONST_ID IID & IID_IBundleReceivedArgsFactory=_uuidof(IBundleReceivedArgsFactory);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.BundleReceivedArgs
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IBundleReceivedArgs ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_BundleReceivedArgs_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_BundleReceivedArgs_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_BundleReceivedArgs[] = L"RealtimeStreaming.Network.BundleReceivedArgs";
#endif


#else // !defined(__cplusplus)
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnection __x_ABI_CRealtimeStreaming_CNetwork_CIConnection;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__




/*
 *
 * Interface RealtimeStreaming.Network.IBundleReceivedArgs
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.BundleReceivedArgs
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IBundleReceivedArgs[] = L"RealtimeStreaming.Network.IBundleReceivedArgs";
/* [object, uuid("26a177fe-2e53-11e9-b210-d663bd873d93"), exclusiveto, version] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [propget] */HRESULT ( STDMETHODCALLTYPE *get_PayloadType )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
        /* [out, retval] */enum __x_ABI_CRealtimeStreaming_CPayloadType * value
        );
    /* [propget] */HRESULT ( STDMETHODCALLTYPE *get_DataConnection )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
        /* [out, retval] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnection * * value
        );
    /* [propget] */HRESULT ( STDMETHODCALLTYPE *get_Bundle )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
        /* [out, retval] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * * value
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_get_PayloadType(This,value) \
    ( (This)->lpVtbl->get_PayloadType(This,value) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_get_DataConnection(This,value) \
    ( (This)->lpVtbl->get_DataConnection(This,value) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_get_Bundle(This,value) \
    ( (This)->lpVtbl->get_Bundle(This,value) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IBundleReceivedArgsFactory
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.BundleReceivedArgs
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IBundleReceivedArgsFactory[] = L"RealtimeStreaming.Network.IBundleReceivedArgsFactory";
/* [object, uuid("9a172738-2e53-11e9-b210-d663bd873d93"), exclusiveto, version] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactoryVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactoryVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactoryVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.BundleReceivedArgs
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IBundleReceivedArgs ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_BundleReceivedArgs_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_BundleReceivedArgs_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_BundleReceivedArgs[] = L"RealtimeStreaming.Network.BundleReceivedArgs";
#endif


#endif // defined(__cplusplus)
#pragma pop_macro("MIDL_CONST_ID")
#endif // __DataBundleArgs_p_h__

#endif // __DataBundleArgs_h__
