/* Header file automatically generated from DataBuffer.idl */
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
#ifndef __DataBuffer_h__
#define __DataBuffer_h__
#ifndef __DataBuffer_p_h__
#define __DataBuffer_p_h__


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
#include "Windows.Storage.h"
#include "Windows.Storage.Streams.h"

#if defined(__cplusplus) && !defined(CINTERFACE)
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IDataBuffer;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer ABI::RealtimeStreaming::Network::IDataBuffer

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IDataBufferFactory;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory ABI::RealtimeStreaming::Network::IDataBufferFactory

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_FWD_DEFINED__



#pragma once
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class DataBuffer;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */



/*
 *
 * Interface RealtimeStreaming.Network.IDataBuffer
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.DataBuffer
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: RealtimeStreaming.Network.IDataBuffer:HRESULT get_CurrentLength(UInt64*);HRESULT put_CurrentLength(UInt64);HRESULT get_Offset(UInt64*);HRESULT put_Offset(UInt64);HRESULT TrimLeft(UInt64,Windows.Foundation.HResult*);HRESULT TrimRight(UInt64,RealtimeStreaming.Network.DataBuffer**);HRESULT Reset(Windows.Foundation.HResult*);
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBuffer[] = L"RealtimeStreaming.Network.IDataBuffer";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [uuid("9c3e8bdb-28c3-5a16-8688-85ea99443847"), version, object, exclusiveto] */
            MIDL_INTERFACE("9c3e8bdb-28c3-5a16-8688-85ea99443847")
            IDataBuffer : public IInspectable
            {
            public:
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_CurrentLength(
                    /* [out, retval] */unsigned __int64 * value
                    ) = 0;
                /* [propput] */virtual HRESULT STDMETHODCALLTYPE put_CurrentLength(
                    /* [in] */unsigned __int64 value
                    ) = 0;
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_Offset(
                    /* [out, retval] */unsigned __int64 * value
                    ) = 0;
                /* [propput] */virtual HRESULT STDMETHODCALLTYPE put_Offset(
                    /* [in] */unsigned __int64 value
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE TrimLeft(
                    /* [in] */unsigned __int64 cbSize,
                    /* [out, retval] */HRESULT * result
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE TrimRight(
                    /* [in] */unsigned __int64 cbSize,
                    /* [out, retval] */ABI::RealtimeStreaming::Network::IDataBuffer * * result
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE Reset(
                    /* [out, retval] */HRESULT * result
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IDataBuffer=_uuidof(IDataBuffer);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IDataBufferFactory
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.DataBuffer
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: RealtimeStreaming.Network.IDataBufferFactory:HRESULT CreateInstance(UInt64,RealtimeStreaming.Network.DataBuffer**);
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBufferFactory[] = L"RealtimeStreaming.Network.IDataBufferFactory";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [uuid("6ac59985-ec6a-5753-aa3f-9e45717bda8b"), version, object, exclusiveto] */
            MIDL_INTERFACE("6ac59985-ec6a-5753-aa3f-9e45717bda8b")
            IDataBufferFactory : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE CreateInstance(
                    /* [in] */unsigned __int64 size,
                    /* [retval, out] */ABI::RealtimeStreaming::Network::IDataBuffer * * value
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IDataBufferFactory=_uuidof(IDataBufferFactory);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.DataBuffer
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IDataBuffer ** Default Interface **
 *    Windows.Storage.Streams.IBuffer
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_DataBuffer_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_DataBuffer_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_DataBuffer[] = L"RealtimeStreaming.Network.DataBuffer";
#endif


#else // !defined(__cplusplus)
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_FWD_DEFINED__


#pragma once


/*
 *
 * Interface RealtimeStreaming.Network.IDataBuffer
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.DataBuffer
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: RealtimeStreaming.Network.IDataBuffer:HRESULT get_CurrentLength(UInt64*);HRESULT put_CurrentLength(UInt64);HRESULT get_Offset(UInt64*);HRESULT put_Offset(UInt64);HRESULT TrimLeft(UInt64,Windows.Foundation.HResult*);HRESULT TrimRight(UInt64,RealtimeStreaming.Network.DataBuffer**);HRESULT Reset(Windows.Foundation.HResult*);
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBuffer[] = L"RealtimeStreaming.Network.IDataBuffer";
/* [uuid("9c3e8bdb-28c3-5a16-8688-85ea99443847"), version, object, exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [propget] */HRESULT ( STDMETHODCALLTYPE *get_CurrentLength )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [out, retval] */unsigned __int64 * value
        );
    /* [propput] */HRESULT ( STDMETHODCALLTYPE *put_CurrentLength )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [in] */unsigned __int64 value
        );
    /* [propget] */HRESULT ( STDMETHODCALLTYPE *get_Offset )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [out, retval] */unsigned __int64 * value
        );
    /* [propput] */HRESULT ( STDMETHODCALLTYPE *put_Offset )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [in] */unsigned __int64 value
        );
    HRESULT ( STDMETHODCALLTYPE *TrimLeft )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [in] */unsigned __int64 cbSize,
        /* [out, retval] */HRESULT * result
        );
    HRESULT ( STDMETHODCALLTYPE *TrimRight )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [in] */unsigned __int64 cbSize,
        /* [out, retval] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * * result
        );
    HRESULT ( STDMETHODCALLTYPE *Reset )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [out, retval] */HRESULT * result
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_get_CurrentLength(This,value) \
    ( (This)->lpVtbl->get_CurrentLength(This,value) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_put_CurrentLength(This,value) \
    ( (This)->lpVtbl->put_CurrentLength(This,value) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_get_Offset(This,value) \
    ( (This)->lpVtbl->get_Offset(This,value) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_put_Offset(This,value) \
    ( (This)->lpVtbl->put_Offset(This,value) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_TrimLeft(This,cbSize,result) \
    ( (This)->lpVtbl->TrimLeft(This,cbSize,result) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_TrimRight(This,cbSize,result) \
    ( (This)->lpVtbl->TrimRight(This,cbSize,result) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_Reset(This,result) \
    ( (This)->lpVtbl->Reset(This,result) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IDataBufferFactory
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.DataBuffer
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: RealtimeStreaming.Network.IDataBufferFactory:HRESULT CreateInstance(UInt64,RealtimeStreaming.Network.DataBuffer**);
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBufferFactory[] = L"RealtimeStreaming.Network.IDataBufferFactory";
/* [uuid("6ac59985-ec6a-5753-aa3f-9e45717bda8b"), version, object, exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactoryVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *CreateInstance )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This,
        /* [in] */unsigned __int64 size,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * * value
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactoryVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactoryVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_CreateInstance(This,size,value) \
    ( (This)->lpVtbl->CreateInstance(This,size,value) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.DataBuffer
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IDataBuffer ** Default Interface **
 *    Windows.Storage.Streams.IBuffer
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_DataBuffer_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_DataBuffer_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_DataBuffer[] = L"RealtimeStreaming.Network.DataBuffer";
#endif


#endif // defined(__cplusplus)
#pragma pop_macro("MIDL_CONST_ID")
#endif // __DataBuffer_p_h__

#endif // __DataBuffer_h__
