/* Header file automatically generated from DataBundle.idl */
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
#ifndef __DataBundle_h__
#define __DataBundle_h__
#ifndef __DataBundle_p_h__
#define __DataBundle_p_h__


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
#include "DataBuffer.h"

#if defined(__cplusplus) && !defined(CINTERFACE)
#if defined(__MIDL_USE_C_ENUM)
#define MIDL_ENUM enum
#else
#define MIDL_ENUM enum class
#endif
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IDataBundle;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle ABI::RealtimeStreaming::Network::IDataBundle

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IDataBundleFactory;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory ABI::RealtimeStreaming::Network::IDataBundleFactory

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_FWD_DEFINED__


namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class DataBundle;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */



/*
 *
 * Interface RealtimeStreaming.Network.IDataBundle
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.DataBundle
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBundle[] = L"RealtimeStreaming.Network.IDataBundle";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, uuid("01458ce6-2e5f-11e9-b210-d663bd873d93"), exclusiveto, version] */
            MIDL_INTERFACE("01458ce6-2e5f-11e9-b210-d663bd873d93")
            IDataBundle : public IInspectable
            {
            public:
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_BufferCount(
                    /* [out, retval] */unsigned int * value
                    ) = 0;
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_TotalSize(
                    /* [out, retval] */unsigned __int64 * value
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE AddBuffer(
                    /* [in] */ABI::RealtimeStreaming::Network::IDataBuffer * dataBuffer,
                    /* [out, retval] */HRESULT * result
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE InsertBuffer(
                    /* [in] */unsigned int index,
                    /* [in] */ABI::RealtimeStreaming::Network::IDataBuffer * dataBuffer,
                    /* [out, retval] */HRESULT * result
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE RemoveBuffer(
                    /* [in] */ABI::RealtimeStreaming::Network::IDataBuffer * dataBuffer,
                    /* [out, retval] */HRESULT * result
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE RemoveBufferByIndex(
                    /* [in] */unsigned int index,
                    /* [out, retval] */ABI::RealtimeStreaming::Network::IDataBuffer * * result
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE Reset(
                    /* [out, retval] */HRESULT * result
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IDataBundle=_uuidof(IDataBundle);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IDataBundleFactory
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.DataBundle
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBundleFactory[] = L"RealtimeStreaming.Network.IDataBundleFactory";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, uuid("042b0fbc-2e5f-11e9-b210-d663bd873d93"), exclusiveto, version] */
            MIDL_INTERFACE("042b0fbc-2e5f-11e9-b210-d663bd873d93")
            IDataBundleFactory : public IInspectable
            {
            public:
                
            };

            extern MIDL_CONST_ID IID & IID_IDataBundleFactory=_uuidof(IDataBundleFactory);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.DataBundle
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IDataBundle ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_DataBundle_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_DataBundle_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_DataBundle[] = L"RealtimeStreaming.Network.DataBundle";
#endif


#else // !defined(__cplusplus)
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_FWD_DEFINED__



/*
 *
 * Interface RealtimeStreaming.Network.IDataBundle
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.DataBundle
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBundle[] = L"RealtimeStreaming.Network.IDataBundle";
/* [object, uuid("01458ce6-2e5f-11e9-b210-d663bd873d93"), exclusiveto, version] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [propget] */HRESULT ( STDMETHODCALLTYPE *get_BufferCount )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [out, retval] */unsigned int * value
        );
    /* [propget] */HRESULT ( STDMETHODCALLTYPE *get_TotalSize )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [out, retval] */unsigned __int64 * value
        );
    HRESULT ( STDMETHODCALLTYPE *AddBuffer )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * dataBuffer,
        /* [out, retval] */HRESULT * result
        );
    HRESULT ( STDMETHODCALLTYPE *InsertBuffer )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [in] */unsigned int index,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * dataBuffer,
        /* [out, retval] */HRESULT * result
        );
    HRESULT ( STDMETHODCALLTYPE *RemoveBuffer )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * dataBuffer,
        /* [out, retval] */HRESULT * result
        );
    HRESULT ( STDMETHODCALLTYPE *RemoveBufferByIndex )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [in] */unsigned int index,
        /* [out, retval] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * * result
        );
    HRESULT ( STDMETHODCALLTYPE *Reset )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [out, retval] */HRESULT * result
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_get_BufferCount(This,value) \
    ( (This)->lpVtbl->get_BufferCount(This,value) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_get_TotalSize(This,value) \
    ( (This)->lpVtbl->get_TotalSize(This,value) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_AddBuffer(This,dataBuffer,result) \
    ( (This)->lpVtbl->AddBuffer(This,dataBuffer,result) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_InsertBuffer(This,index,dataBuffer,result) \
    ( (This)->lpVtbl->InsertBuffer(This,index,dataBuffer,result) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_RemoveBuffer(This,dataBuffer,result) \
    ( (This)->lpVtbl->RemoveBuffer(This,dataBuffer,result) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_RemoveBufferByIndex(This,index,result) \
    ( (This)->lpVtbl->RemoveBufferByIndex(This,index,result) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_Reset(This,result) \
    ( (This)->lpVtbl->Reset(This,result) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IDataBundleFactory
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.DataBundle
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBundleFactory[] = L"RealtimeStreaming.Network.IDataBundleFactory";
/* [object, uuid("042b0fbc-2e5f-11e9-b210-d663bd873d93"), exclusiveto, version] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactoryVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactoryVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactoryVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.DataBundle
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IDataBundle ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_DataBundle_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_DataBundle_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_DataBundle[] = L"RealtimeStreaming.Network.DataBundle";
#endif


#endif // defined(__cplusplus)
#pragma pop_macro("MIDL_CONST_ID")
#endif // __DataBundle_p_h__

#endif // __DataBundle_h__
