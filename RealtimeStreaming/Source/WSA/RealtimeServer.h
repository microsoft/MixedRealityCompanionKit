/* Header file automatically generated from RealtimeServer.idl */
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
#ifndef __RealtimeServer_h__
#define __RealtimeServer_h__
#ifndef __RealtimeServer_p_h__
#define __RealtimeServer_p_h__


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
#include "Windows.Media.h"
#include "Connection.h"
#include "ModuleManager.h"

#if defined(__cplusplus) && !defined(CINTERFACE)
#if defined(__MIDL_USE_C_ENUM)
#define MIDL_ENUM enum
#else
#define MIDL_ENUM enum class
#endif
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            interface IRealtimeServer;
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer ABI::RealtimeStreaming::Media::IRealtimeServer

#endif // ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            interface IRealtimeServerFactory;
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory ABI::RealtimeStreaming::Media::IRealtimeServerFactory

#endif // ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_FWD_DEFINED__


namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            class RealtimeServer;
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */



/*
 *
 * Interface RealtimeStreaming.Media.IRealtimeServer
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.RealtimeServer
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: RealtimeStreaming.Media.IRealtimeServer:HRESULT WriteFrame(UInt32,UInt32,UInt8*);HRESULT get_VideoProperties(Windows.Media.MediaProperties.VideoEncodingProperties**);
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_IRealtimeServer[] = L"RealtimeStreaming.Media.IRealtimeServer";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            /* [uuid("bdf2a077-3916-5de4-b9ea-74f026b059d5"), version, object, exclusiveto] */
            MIDL_INTERFACE("bdf2a077-3916-5de4-b9ea-74f026b059d5")
            IRealtimeServer : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE WriteFrame(
                    /* [in] */unsigned int bufferSize,
                    /* [in] */unsigned int pBufferLength,
                    /* [size_is(pBufferLength), in] */byte * pBuffer
                    ) = 0;
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_VideoProperties(
                    /* [out, retval] */ABI::Windows::Media::MediaProperties::IVideoEncodingProperties * * value
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IRealtimeServer=_uuidof(IRealtimeServer);
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Media.IRealtimeServerFactory
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.RealtimeServer
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: RealtimeStreaming.Media.IRealtimeServerFactory:HRESULT CreateInstance(RealtimeStreaming.Network.Connection*,__MIDL___MIDL_itf_RealtimeServer2Eidl_0006_0001_0001,Windows.Media.MediaProperties.MediaEncodingProfile*,RealtimeStreaming.Media.RealtimeServer**);
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_IRealtimeServerFactory[] = L"RealtimeStreaming.Media.IRealtimeServerFactory";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            /* [uuid("3baec682-8a74-582b-97a6-e4ec999c4c7f"), version, object, exclusiveto] */
            MIDL_INTERFACE("3baec682-8a74-582b-97a6-e4ec999c4c7f")
            IRealtimeServerFactory : public IInspectable
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE CreateInstance(
                    /* [in] */ABI::RealtimeStreaming::Network::IConnection * connection,
                    /* [in] */GUID inputMediaType,
                    /* [in] */ABI::Windows::Media::MediaProperties::IMediaEncodingProfile * mediaEncodingProperties,
                    /* [retval, out] */ABI::RealtimeStreaming::Media::IRealtimeServer * * value
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IRealtimeServerFactory=_uuidof(IRealtimeServerFactory);
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Media.RealtimeServer
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Media.IRealtimeServer ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Media_RealtimeServer_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Media_RealtimeServer_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Media_RealtimeServer[] = L"RealtimeStreaming.Media.RealtimeServer";
#endif


#else // !defined(__cplusplus)
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer;

#endif // ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory;

#endif // ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_FWD_DEFINED__



/*
 *
 * Interface RealtimeStreaming.Media.IRealtimeServer
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.RealtimeServer
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: RealtimeStreaming.Media.IRealtimeServer:HRESULT WriteFrame(UInt32,UInt32,UInt8*);HRESULT get_VideoProperties(Windows.Media.MediaProperties.VideoEncodingProperties**);
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_IRealtimeServer[] = L"RealtimeStreaming.Media.IRealtimeServer";
/* [uuid("bdf2a077-3916-5de4-b9ea-74f026b059d5"), version, object, exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *WriteFrame )(
        __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
        /* [in] */unsigned int bufferSize,
        /* [in] */unsigned int pBufferLength,
        /* [size_is(pBufferLength), in] */byte * pBuffer
        );
    /* [propget] */HRESULT ( STDMETHODCALLTYPE *get_VideoProperties )(
        __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
        /* [out, retval] */__x_ABI_CWindows_CMedia_CMediaProperties_CIVideoEncodingProperties * * value
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerVtbl;

interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_WriteFrame(This,bufferSize,pBufferLength,pBuffer) \
    ( (This)->lpVtbl->WriteFrame(This,bufferSize,pBufferLength,pBuffer) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_get_VideoProperties(This,value) \
    ( (This)->lpVtbl->get_VideoProperties(This,value) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Media.IRealtimeServerFactory
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.RealtimeServer
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: RealtimeStreaming.Media.IRealtimeServerFactory:HRESULT CreateInstance(RealtimeStreaming.Network.Connection*,__MIDL___MIDL_itf_RealtimeServer2Eidl_0006_0001_0001,Windows.Media.MediaProperties.MediaEncodingProfile*,RealtimeStreaming.Media.RealtimeServer**);
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_IRealtimeServerFactory[] = L"RealtimeStreaming.Media.IRealtimeServerFactory";
/* [uuid("3baec682-8a74-582b-97a6-e4ec999c4c7f"), version, object, exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactoryVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *CreateInstance )(
        __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnection * connection,
        /* [in] */GUID inputMediaType,
        /* [in] */__x_ABI_CWindows_CMedia_CMediaProperties_CIMediaEncodingProfile * mediaEncodingProperties,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * * value
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactoryVtbl;

interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactoryVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_CreateInstance(This,connection,inputMediaType,mediaEncodingProperties,value) \
    ( (This)->lpVtbl->CreateInstance(This,connection,inputMediaType,mediaEncodingProperties,value) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Media.RealtimeServer
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Media.IRealtimeServer ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Media_RealtimeServer_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Media_RealtimeServer_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Media_RealtimeServer[] = L"RealtimeStreaming.Media.RealtimeServer";
#endif


#endif // defined(__cplusplus)
#pragma pop_macro("MIDL_CONST_ID")
#endif // __RealtimeServer_p_h__

#endif // __RealtimeServer_h__
