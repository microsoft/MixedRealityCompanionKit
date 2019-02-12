/* Header file automatically generated from Connector.idl */
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
#ifndef __Connector_h__
#define __Connector_h__
#ifndef __Connector_p_h__
#define __Connector_p_h__


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
#include "Connection.h"
#include "ModuleManager.h"
// Importing Collections header
#include <windows.foundation.collections.h>

#if defined(__cplusplus) && !defined(CINTERFACE)
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IConnector;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector ABI::RealtimeStreaming::Network::IConnector

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__

// Parameterized interface forward declarations (C++)

// Collection interface definitions
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class Connection;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

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



#ifndef DEF___FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_USE
#define DEF___FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("45c830d1-4827-5175-b3da-fe2695798e03"))
IAsyncOperationCompletedHandler<ABI::RealtimeStreaming::Network::Connection*> : IAsyncOperationCompletedHandler_impl<ABI::Windows::Foundation::Internal::AggregateType<ABI::RealtimeStreaming::Network::Connection*, ABI::RealtimeStreaming::Network::IConnection*>> 
{
    static const wchar_t* z_get_rc_name_impl() 
    {
        return L"Windows.Foundation.AsyncOperationCompletedHandler`1<RealtimeStreaming.Network.Connection>"; 
    }
};
// Define a typedef for the parameterized interface specialization's mangled name.
// This allows code which uses the mangled name for the parameterized interface to access the
// correct parameterized interface specialization.
typedef IAsyncOperationCompletedHandler<ABI::RealtimeStreaming::Network::Connection*> __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_t;
#define __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection ABI::Windows::Foundation::__FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_t
/* ABI */ } /* Windows */ } /* Foundation */ }

////  Define an alias for the C version of the interface for compatibility purposes.
//#define __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::RealtimeStreaming::Network::IConnection*>
//#define __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_t ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::RealtimeStreaming::Network::IConnection*>
#endif // !defined(RO_NO_TEMPLATE_NAME)
#endif /* DEF___FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_USE */





#ifndef DEF___FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_USE
#define DEF___FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("b0827db8-0aab-584a-9d02-9f162504c7a6"))
IAsyncOperation<ABI::RealtimeStreaming::Network::Connection*> : IAsyncOperation_impl<ABI::Windows::Foundation::Internal::AggregateType<ABI::RealtimeStreaming::Network::Connection*, ABI::RealtimeStreaming::Network::IConnection*>> 
{
    static const wchar_t* z_get_rc_name_impl() 
    {
        return L"Windows.Foundation.IAsyncOperation`1<RealtimeStreaming.Network.Connection>"; 
    }
};
// Define a typedef for the parameterized interface specialization's mangled name.
// This allows code which uses the mangled name for the parameterized interface to access the
// correct parameterized interface specialization.
typedef IAsyncOperation<ABI::RealtimeStreaming::Network::Connection*> __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_t;
#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection ABI::Windows::Foundation::__FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_t
/* ABI */ } /* Windows */ } /* Foundation */ }

////  Define an alias for the C version of the interface for compatibility purposes.
//#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection ABI::Windows::Foundation::IAsyncOperation<ABI::RealtimeStreaming::Network::IConnection*>
//#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_t ABI::Windows::Foundation::IAsyncOperation<ABI::RealtimeStreaming::Network::IConnection*>
#endif // !defined(RO_NO_TEMPLATE_NAME)
#endif /* DEF___FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_USE */




#ifndef DEF___FIEventHandler_1_boolean_USE
#define DEF___FIEventHandler_1_boolean_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("6ea71ff9-100d-54c3-bb4a-6922523cf9a9"))
IEventHandler<bool> : IEventHandler_impl<ABI::Windows::Foundation::Internal::AggregateType<bool, boolean>> 
{
    static const wchar_t* z_get_rc_name_impl() 
    {
        return L"Windows.Foundation.EventHandler`1<Boolean>"; 
    }
};
// Define a typedef for the parameterized interface specialization's mangled name.
// This allows code which uses the mangled name for the parameterized interface to access the
// correct parameterized interface specialization.
typedef IEventHandler<bool> __FIEventHandler_1_boolean_t;
#define __FIEventHandler_1_boolean ABI::Windows::Foundation::__FIEventHandler_1_boolean_t
/* ABI */ } /* Windows */ } /* Foundation */ }

////  Define an alias for the C version of the interface for compatibility purposes.
//#define __FIEventHandler_1_boolean ABI::Windows::Foundation::IEventHandler<boolean>
//#define __FIEventHandler_1_boolean_t ABI::Windows::Foundation::IEventHandler<boolean>
#endif // !defined(RO_NO_TEMPLATE_NAME)
#endif /* DEF___FIEventHandler_1_boolean_USE */






/*
 *
 * Interface RealtimeStreaming.Network.IConnector
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connector
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnector[] = L"RealtimeStreaming.Network.IConnector";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, uuid("6229e89c-2e85-11e9-b210-d663bd873d93"), exclusiveto, version] */
            MIDL_INTERFACE("6229e89c-2e85-11e9-b210-d663bd873d93")
            IConnector : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE ConnectAsync(
                    /* [out, retval] */__FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * * operation
                    ) = 0;
                /* [eventadd] */virtual HRESULT STDMETHODCALLTYPE add_Closed(
                    /* [in] */__FIEventHandler_1_boolean * handler,
                    /* [retval, out] */EventRegistrationToken * token
                    ) = 0;
                /* [eventremove] */virtual HRESULT STDMETHODCALLTYPE remove_Closed(
                    /* [in] */EventRegistrationToken token
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IConnector=_uuidof(IConnector);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnector;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.Connector
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IConnector ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_Connector_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_Connector_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_Connector[] = L"RealtimeStreaming.Network.Connector";
#endif


#else // !defined(__cplusplus)
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnector __x_ABI_CRealtimeStreaming_CNetwork_CIConnector;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__

// Parameterized interface forward declarations (C)

// Collection interface definitions
#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnection __x_ABI_CRealtimeStreaming_CNetwork_CIConnection;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__


#if !defined(____FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__)
#define ____FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__

typedef interface __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection;

//  Declare the parameterized interface IID.
EXTERN_C const IID IID___FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection;

// Forward declare the async operation.
typedef interface __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection;

typedef struct __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnectionVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(__RPC__in __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection * This,
        /* [in] */ __RPC__in REFIID riid,
        /* [annotation][iid_is][out] */ 
        _COM_Outptr_  void **ppvObject);
    ULONG ( STDMETHODCALLTYPE *AddRef )(__RPC__in __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection * This);
    ULONG ( STDMETHODCALLTYPE *Release )(__RPC__in __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection * This);

    HRESULT ( STDMETHODCALLTYPE *Invoke )(__RPC__in __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection * This,/* [in] */ __RPC__in_opt __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection *asyncInfo, /* [in] */ AsyncStatus status);
    END_INTERFACE
} __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnectionVtbl;

interface __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection
{
    CONST_VTBL struct __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnectionVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 
#define __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 
#define __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 

#define __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_Invoke(This,asyncInfo,status)	\
    ( (This)->lpVtbl -> Invoke(This,asyncInfo,status) ) 
#endif /* COBJMACROS */


#endif // ____FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__



#if !defined(____FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__)
#define ____FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__

typedef interface __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection;

//  Declare the parameterized interface IID.
EXTERN_C const IID IID___FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection;

typedef struct __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnectionVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(__RPC__in __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This,
        /* [in] */ __RPC__in REFIID riid,
        /* [annotation][iid_is][out] */ 
        _COM_Outptr_  void **ppvObject);
    ULONG ( STDMETHODCALLTYPE *AddRef )(__RPC__in __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This);
    ULONG ( STDMETHODCALLTYPE *Release )(__RPC__in __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This);

    HRESULT ( STDMETHODCALLTYPE *GetIids )(__RPC__in __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This,
        /* [out] */ __RPC__out ULONG *iidCount,
        /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids);
    HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(__RPC__in __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This, /* [out] */ __RPC__deref_out_opt HSTRING *className);
    HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(__RPC__in __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This, /* [out] */ __RPC__out TrustLevel *trustLevel);

    /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Completed )(__RPC__in __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This, /* [in] */ __RPC__in_opt __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection *handler);
    /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Completed )(__RPC__in __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This, /* [retval][out] */ __RPC__deref_out_opt __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection **handler);
    HRESULT ( STDMETHODCALLTYPE *GetResults )(__RPC__in __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This, /* [retval][out] */ __RPC__out __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * *results);
    END_INTERFACE
} __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnectionVtbl;

interface __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection
{
    CONST_VTBL struct __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnectionVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 
#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 
#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 

#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 
#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 
#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 

#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_put_Completed(This,handler)	\
    ( (This)->lpVtbl -> put_Completed(This,handler) ) 
#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_get_Completed(This,handler)	\
    ( (This)->lpVtbl -> get_Completed(This,handler) ) 
#define __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_GetResults(This,results)	\
    ( (This)->lpVtbl -> GetResults(This,results) ) 
#endif /* COBJMACROS */


#endif // ____FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__


#if !defined(____FIEventHandler_1_boolean_INTERFACE_DEFINED__)
#define ____FIEventHandler_1_boolean_INTERFACE_DEFINED__

typedef interface __FIEventHandler_1_boolean __FIEventHandler_1_boolean;

//  Declare the parameterized interface IID.
EXTERN_C const IID IID___FIEventHandler_1_boolean;

typedef struct __FIEventHandler_1_booleanVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(__RPC__in __FIEventHandler_1_boolean * This,
        /* [in] */ __RPC__in REFIID riid,
        /* [annotation][iid_is][out] */ 
        _COM_Outptr_  void **ppvObject);
    ULONG ( STDMETHODCALLTYPE *AddRef )(__RPC__in __FIEventHandler_1_boolean * This);
    ULONG ( STDMETHODCALLTYPE *Release )(__RPC__in __FIEventHandler_1_boolean * This);

    HRESULT ( STDMETHODCALLTYPE *Invoke )(__RPC__in __FIEventHandler_1_boolean * This,/* [in] */ __RPC__in_opt IInspectable *sender,/* [in] */ __RPC__in_opt boolean *e);
    END_INTERFACE
} __FIEventHandler_1_booleanVtbl;

interface __FIEventHandler_1_boolean
{
    CONST_VTBL struct __FIEventHandler_1_booleanVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __FIEventHandler_1_boolean_QueryInterface(This,riid,ppvObject)	\
        ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 
#define __FIEventHandler_1_boolean_AddRef(This)	\
        ( (This)->lpVtbl -> AddRef(This) ) 
#define __FIEventHandler_1_boolean_Release(This)	\
        ( (This)->lpVtbl -> Release(This) ) 

#define __FIEventHandler_1_boolean_Invoke(This,sender,e)	\
        ( (This)->lpVtbl -> Invoke(This,sender,e) ) 
#endif /* COBJMACROS */


#endif // ____FIEventHandler_1_boolean_INTERFACE_DEFINED__




/*
 *
 * Interface RealtimeStreaming.Network.IConnector
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connector
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnector[] = L"RealtimeStreaming.Network.IConnector";
/* [object, uuid("6229e89c-2e85-11e9-b210-d663bd873d93"), exclusiveto, version] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *ConnectAsync )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
        /* [out, retval] */__FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * * operation
        );
    /* [eventadd] */HRESULT ( STDMETHODCALLTYPE *add_Closed )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
        /* [in] */__FIEventHandler_1_boolean * handler,
        /* [retval, out] */EventRegistrationToken * token
        );
    /* [eventremove] */HRESULT ( STDMETHODCALLTYPE *remove_Closed )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
        /* [in] */EventRegistrationToken token
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnector
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_ConnectAsync(This,operation) \
    ( (This)->lpVtbl->ConnectAsync(This,operation) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_add_Closed(This,handler,token) \
    ( (This)->lpVtbl->add_Closed(This,handler,token) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_remove_Closed(This,token) \
    ( (This)->lpVtbl->remove_Closed(This,token) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnector;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.Connector
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IConnector ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_Connector_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_Connector_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_Connector[] = L"RealtimeStreaming.Network.Connector";
#endif


#endif // defined(__cplusplus)
#pragma pop_macro("MIDL_CONST_ID")
#endif // __Connector_p_h__

#endif // __Connector_h__
