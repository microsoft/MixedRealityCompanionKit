/* Header file automatically generated from RealtimeStreaming.idl */
/*
 * File built with Microsoft(R) MIDLRT Compiler Engine Version 10.00.0206 
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
#ifndef __RealtimeStreaming_h__
#define __RealtimeStreaming_h__
#ifndef __RealtimeStreaming_p_h__
#define __RealtimeStreaming_p_h__


#pragma once

#pragma push_macro("MIDL_CONST_ID")
#if !defined(_MSC_VER) || (_MSC_VER >= 1910)
#define MIDL_CONST_ID constexpr const
#else
#define MIDL_CONST_ID const __declspec(selectany)
#endif


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
#define WINDOWS_APPLICATIONMODEL_CALLS_CALLSPHONECONTRACT_VERSION 0x30000
#endif // defined(WINDOWS_APPLICATIONMODEL_CALLS_CALLSPHONECONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_CALLS_CALLSVOIPCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_CALLS_CALLSVOIPCONTRACT_VERSION 0x20000
#endif // defined(WINDOWS_APPLICATIONMODEL_CALLS_CALLSVOIPCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_CALLS_LOCKSCREENCALLCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_CALLS_LOCKSCREENCALLCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_CALLS_LOCKSCREENCALLCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_FULLTRUSTAPPCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_FULLTRUSTAPPCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_FULLTRUSTAPPCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_SEARCH_SEARCHCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_SEARCH_SEARCHCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_APPLICATIONMODEL_SEARCH_SEARCHCONTRACT_VERSION)

#if !defined(WINDOWS_APPLICATIONMODEL_STARTUPTASKCONTRACT_VERSION)
#define WINDOWS_APPLICATIONMODEL_STARTUPTASKCONTRACT_VERSION 0x20000
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
#define WINDOWS_DEVICES_SMARTCARDS_SMARTCARDEMULATORCONTRACT_VERSION 0x50000
#endif // defined(WINDOWS_DEVICES_SMARTCARDS_SMARTCARDEMULATORCONTRACT_VERSION)

#if !defined(WINDOWS_DEVICES_SMS_LEGACYSMSAPICONTRACT_VERSION)
#define WINDOWS_DEVICES_SMS_LEGACYSMSAPICONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_DEVICES_SMS_LEGACYSMSAPICONTRACT_VERSION)

#if !defined(WINDOWS_FOUNDATION_FOUNDATIONCONTRACT_VERSION)
#define WINDOWS_FOUNDATION_FOUNDATIONCONTRACT_VERSION 0x30000
#endif // defined(WINDOWS_FOUNDATION_FOUNDATIONCONTRACT_VERSION)

#if !defined(WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION)
#define WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION 0x50000
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
#define WINDOWS_NETWORKING_CONNECTIVITY_WWANCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_NETWORKING_CONNECTIVITY_WWANCONTRACT_VERSION)

#if !defined(WINDOWS_NETWORKING_SOCKETS_CONTROLCHANNELTRIGGERCONTRACT_VERSION)
#define WINDOWS_NETWORKING_SOCKETS_CONTROLCHANNELTRIGGERCONTRACT_VERSION 0x20000
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
#define WINDOWS_STORAGE_PROVIDER_CLOUDFILESCONTRACT_VERSION 0x10000
#endif // defined(WINDOWS_STORAGE_PROVIDER_CLOUDFILESCONTRACT_VERSION)

#if !defined(WINDOWS_SYSTEM_SYSTEMMANAGEMENTCONTRACT_VERSION)
#define WINDOWS_SYSTEM_SYSTEMMANAGEMENTCONTRACT_VERSION 0x40000
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
#include "inspectable.h"
#include "AsyncInfo.h"
#include "EventToken.h"
#include "Windows.Foundation.h"
#include "Windows.Foundation.Numerics.h"
#include "Windows.Media.h"
#include "Windows.Media.Capture.h"
#include "Windows.Media.Capture.Core.h"
#include "Windows.Networking.Sockets.h"
#include "Windows.Perception.Spatial.h"
#include "Windows.Storage.h"
#include "Windows.Storage.Streams.h"
// Importing Collections header
#include <windows.foundation.collections.h>

#if defined(__cplusplus) && !defined(CINTERFACE)
#if defined(__MIDL_USE_C_ENUM)
#define MIDL_ENUM enum
#else
#define MIDL_ENUM enum class
#endif
/* Forward Declarations */
#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            interface IClosedEventHandler;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler ABI::RealtimeStreaming::Plugin::IClosedEventHandler

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_FWD_DEFINED__

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

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            interface IModuleManagerStatics;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics ABI::RealtimeStreaming::Plugin::IModuleManagerStatics

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            interface IPluginManager;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager ABI::RealtimeStreaming::Plugin::IPluginManager

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            interface IPluginManagerStatics;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics ABI::RealtimeStreaming::Plugin::IPluginManagerStatics

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            interface IDirectXManager;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager ABI::RealtimeStreaming::Plugin::IDirectXManager

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IDisconnectedEventHandler;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler ABI::RealtimeStreaming::Network::IDisconnectedEventHandler

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IBundleReceivedEventHandler;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler ABI::RealtimeStreaming::Network::IBundleReceivedEventHandler

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IConnectedEventHandler;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler ABI::RealtimeStreaming::Network::IConnectedEventHandler

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IDataPacket;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket ABI::RealtimeStreaming::Network::IDataPacket

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_FWD_DEFINED__

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

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IDataBufferStatics;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics ABI::RealtimeStreaming::Network::IDataBufferStatics

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IDataBufferLock;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock ABI::RealtimeStreaming::Network::IDataBufferLock

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_FWD_DEFINED__

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

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IDataBundleStatics;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics ABI::RealtimeStreaming::Network::IDataBundleStatics

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_FWD_DEFINED__

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

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IBundleReceivedArgsStatics;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics ABI::RealtimeStreaming::Network::IBundleReceivedArgsStatics

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_FWD_DEFINED__

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

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IConnectionInternal;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal ABI::RealtimeStreaming::Network::IConnectionInternal

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IConnectionStatics;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics ABI::RealtimeStreaming::Network::IConnectionStatics

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIListener_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIListener_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IListener;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIListener ABI::RealtimeStreaming::Network::IListener

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIListener_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IListenerStatics;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics ABI::RealtimeStreaming::Network::IListenerStatics

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_FWD_DEFINED__

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

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            interface IConnectorStatics;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics ABI::RealtimeStreaming::Network::IConnectorStatics

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            interface IMrvcSchemeHandler;
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler ABI::RealtimeStreaming::Media::IMrvcSchemeHandler

#endif // ____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            interface INetworkMediaSink;
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink ABI::RealtimeStreaming::Media::INetworkMediaSink

#endif // ____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_FWD_DEFINED__

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

#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_FWD_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            interface IRealtimeServerStatics;
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */
#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics ABI::RealtimeStreaming::Media::IRealtimeServerStatics

#endif // ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_FWD_DEFINED__

// Parameterized interface forward declarations (C++)

// Collection interface definitions
namespace ABI {
    namespace Windows {
        namespace Devices {
            namespace Enumeration {
                class DeviceInformation;
            } /* Windows */
        } /* Devices */
    } /* Enumeration */} /* ABI */

#ifndef ____x_ABI_CWindows_CDevices_CEnumeration_CIDeviceInformation_FWD_DEFINED__
#define ____x_ABI_CWindows_CDevices_CEnumeration_CIDeviceInformation_FWD_DEFINED__
namespace ABI {
    namespace Windows {
        namespace Devices {
            namespace Enumeration {
                interface IDeviceInformation;
            } /* Windows */
        } /* Devices */
    } /* Enumeration */} /* ABI */
#define __x_ABI_CWindows_CDevices_CEnumeration_CIDeviceInformation ABI::Windows::Devices::Enumeration::IDeviceInformation

#endif // ____x_ABI_CWindows_CDevices_CEnumeration_CIDeviceInformation_FWD_DEFINED__


#if WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION >= 0x10000

#ifndef DEF___FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_USE
#define DEF___FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("bb483df2-7bb6-5923-a28d-8342ec30046b"))
IAsyncOperationCompletedHandler<ABI::Windows::Devices::Enumeration::DeviceInformation*> : IAsyncOperationCompletedHandler_impl<ABI::Windows::Foundation::Internal::AggregateType<ABI::Windows::Devices::Enumeration::DeviceInformation*, ABI::Windows::Devices::Enumeration::IDeviceInformation*>> 
{
    static const wchar_t* z_get_rc_name_impl() 
    {
        return L"Windows.Foundation.AsyncOperationCompletedHandler`1<Windows.Devices.Enumeration.DeviceInformation>"; 
    }
};
// Define a typedef for the parameterized interface specialization's mangled name.
// This allows code which uses the mangled name for the parameterized interface to access the
// correct parameterized interface specialization.
typedef IAsyncOperationCompletedHandler<ABI::Windows::Devices::Enumeration::DeviceInformation*> __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_t;
#define __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation ABI::Windows::Foundation::__FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_t
/* ABI */ } /* Windows */ } /* Foundation */ }

////  Define an alias for the C version of the interface for compatibility purposes.
//#define __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::Windows::Devices::Enumeration::IDeviceInformation*>
//#define __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_t ABI::Windows::Foundation::IAsyncOperationCompletedHandler<ABI::Windows::Devices::Enumeration::IDeviceInformation*>
#endif // !defined(RO_NO_TEMPLATE_NAME)
#endif /* DEF___FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_USE */


#endif // WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION >= 0x10000


#if WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION >= 0x10000

#ifndef DEF___FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_USE
#define DEF___FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("07faa053-eb2f-5cba-b25b-d9d57be6715f"))
IAsyncOperation<ABI::Windows::Devices::Enumeration::DeviceInformation*> : IAsyncOperation_impl<ABI::Windows::Foundation::Internal::AggregateType<ABI::Windows::Devices::Enumeration::DeviceInformation*, ABI::Windows::Devices::Enumeration::IDeviceInformation*>> 
{
    static const wchar_t* z_get_rc_name_impl() 
    {
        return L"Windows.Foundation.IAsyncOperation`1<Windows.Devices.Enumeration.DeviceInformation>"; 
    }
};
// Define a typedef for the parameterized interface specialization's mangled name.
// This allows code which uses the mangled name for the parameterized interface to access the
// correct parameterized interface specialization.
typedef IAsyncOperation<ABI::Windows::Devices::Enumeration::DeviceInformation*> __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_t;
#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation ABI::Windows::Foundation::__FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_t
/* ABI */ } /* Windows */ } /* Foundation */ }

////  Define an alias for the C version of the interface for compatibility purposes.
//#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Devices::Enumeration::IDeviceInformation*>
//#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_t ABI::Windows::Foundation::IAsyncOperation<ABI::Windows::Devices::Enumeration::IDeviceInformation*>
#endif // !defined(RO_NO_TEMPLATE_NAME)
#endif /* DEF___FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_USE */


#endif // WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION >= 0x10000

namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class DataBuffer;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */



#ifndef DEF___FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_USE
#define DEF___FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation { namespace Collections {
template <>
struct __declspec(uuid("02efb54c-9903-5c1a-a1ec-7120567d76e7"))
IIterator<ABI::RealtimeStreaming::Network::DataBuffer*> : IIterator_impl<ABI::Windows::Foundation::Internal::AggregateType<ABI::RealtimeStreaming::Network::DataBuffer*, ABI::RealtimeStreaming::Network::IDataBuffer*>> 
{
    static const wchar_t* z_get_rc_name_impl() 
    {
        return L"Windows.Foundation.Collections.IIterator`1<RealtimeStreaming.Network.DataBuffer>"; 
    }
};
// Define a typedef for the parameterized interface specialization's mangled name.
// This allows code which uses the mangled name for the parameterized interface to access the
// correct parameterized interface specialization.
typedef IIterator<ABI::RealtimeStreaming::Network::DataBuffer*> __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_t;
#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer ABI::Windows::Foundation::Collections::__FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_t
/* ABI */ } /* Windows */ } /* Foundation */ } /* Collections */ }

////  Define an alias for the C version of the interface for compatibility purposes.
//#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer ABI::Windows::Foundation::Collections::IIterator<ABI::RealtimeStreaming::Network::IDataBuffer*>
//#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_t ABI::Windows::Foundation::Collections::IIterator<ABI::RealtimeStreaming::Network::IDataBuffer*>
#endif // !defined(RO_NO_TEMPLATE_NAME)
#endif /* DEF___FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_USE */





#ifndef DEF___FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_USE
#define DEF___FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation { namespace Collections {
template <>
struct __declspec(uuid("4f439324-d96a-583b-8e44-fb8213a947d6"))
IIterable<ABI::RealtimeStreaming::Network::DataBuffer*> : IIterable_impl<ABI::Windows::Foundation::Internal::AggregateType<ABI::RealtimeStreaming::Network::DataBuffer*, ABI::RealtimeStreaming::Network::IDataBuffer*>> 
{
    static const wchar_t* z_get_rc_name_impl() 
    {
        return L"Windows.Foundation.Collections.IIterable`1<RealtimeStreaming.Network.DataBuffer>"; 
    }
};
// Define a typedef for the parameterized interface specialization's mangled name.
// This allows code which uses the mangled name for the parameterized interface to access the
// correct parameterized interface specialization.
typedef IIterable<ABI::RealtimeStreaming::Network::DataBuffer*> __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_t;
#define __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer ABI::Windows::Foundation::Collections::__FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_t
/* ABI */ } /* Windows */ } /* Foundation */ } /* Collections */ }

////  Define an alias for the C version of the interface for compatibility purposes.
//#define __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer ABI::Windows::Foundation::Collections::IIterable<ABI::RealtimeStreaming::Network::IDataBuffer*>
//#define __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_t ABI::Windows::Foundation::Collections::IIterable<ABI::RealtimeStreaming::Network::IDataBuffer*>
#endif // !defined(RO_NO_TEMPLATE_NAME)
#endif /* DEF___FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_USE */





#ifndef DEF___FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_USE
#define DEF___FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation { namespace Collections {
template <>
struct __declspec(uuid("24f72ee9-a071-5f09-8215-e73b9083cb55"))
IVectorView<ABI::RealtimeStreaming::Network::DataBuffer*> : IVectorView_impl<ABI::Windows::Foundation::Internal::AggregateType<ABI::RealtimeStreaming::Network::DataBuffer*, ABI::RealtimeStreaming::Network::IDataBuffer*>> 
{
    static const wchar_t* z_get_rc_name_impl() 
    {
        return L"Windows.Foundation.Collections.IVectorView`1<RealtimeStreaming.Network.DataBuffer>"; 
    }
};
// Define a typedef for the parameterized interface specialization's mangled name.
// This allows code which uses the mangled name for the parameterized interface to access the
// correct parameterized interface specialization.
typedef IVectorView<ABI::RealtimeStreaming::Network::DataBuffer*> __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_t;
#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer ABI::Windows::Foundation::Collections::__FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_t
/* ABI */ } /* Windows */ } /* Foundation */ } /* Collections */ }

////  Define an alias for the C version of the interface for compatibility purposes.
//#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer ABI::Windows::Foundation::Collections::IVectorView<ABI::RealtimeStreaming::Network::IDataBuffer*>
//#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_t ABI::Windows::Foundation::Collections::IVectorView<ABI::RealtimeStreaming::Network::IDataBuffer*>
#endif // !defined(RO_NO_TEMPLATE_NAME)
#endif /* DEF___FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_USE */





#ifndef DEF___FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_USE
#define DEF___FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation { namespace Collections {
template <>
struct __declspec(uuid("05fbf735-6444-5217-bc03-6b4495e9d8ce"))
IVector<ABI::RealtimeStreaming::Network::DataBuffer*> : IVector_impl<ABI::Windows::Foundation::Internal::AggregateType<ABI::RealtimeStreaming::Network::DataBuffer*, ABI::RealtimeStreaming::Network::IDataBuffer*>> 
{
    static const wchar_t* z_get_rc_name_impl() 
    {
        return L"Windows.Foundation.Collections.IVector`1<RealtimeStreaming.Network.DataBuffer>"; 
    }
};
// Define a typedef for the parameterized interface specialization's mangled name.
// This allows code which uses the mangled name for the parameterized interface to access the
// correct parameterized interface specialization.
typedef IVector<ABI::RealtimeStreaming::Network::DataBuffer*> __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_t;
#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer ABI::Windows::Foundation::Collections::__FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_t
/* ABI */ } /* Windows */ } /* Foundation */ } /* Collections */ }

////  Define an alias for the C version of the interface for compatibility purposes.
//#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer ABI::Windows::Foundation::Collections::IVector<ABI::RealtimeStreaming::Network::IDataBuffer*>
//#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_t ABI::Windows::Foundation::Collections::IVector<ABI::RealtimeStreaming::Network::IDataBuffer*>
#endif // !defined(RO_NO_TEMPLATE_NAME)
#endif /* DEF___FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_USE */



namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class Connection;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */



#ifndef DEF___FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_USE
#define DEF___FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("ae3bb708-f38a-56e2-8880-278c8627dd05"))
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
struct __declspec(uuid("31717ca0-2cab-56fa-9875-2312ef716900"))
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





namespace ABI {
    namespace RealtimeStreaming {
        
        typedef UINT32 ModuleHandle;
        
    } /* RealtimeStreaming */} /* ABI */


#ifndef MODULE_HANDLE

typedef UINT32 MODULE_HANDLE;

#define MODULE_HANDLE_INVALID (UINT)0x0bad

#define MODULE_HANDLE_START (UINT)0x0bae

#endif // MODULE_HANDLE

const ULONG c_cbReceiveBufferSize = 2 * 1024;

const ULONG c_cbMaxBundleSize = 1024 * 1024;

const UINT16 c_cbMaxBufferFailures = 7;

const UINT16 c_cbMaxBundleFailures = 3;

extern wchar_t const __declspec(selectany)c_szNetworkScheme[] = L"mrvc";

extern wchar_t const __declspec(selectany)c_szNetworkSchemeWithColon[] = L"mrvc:";


#ifdef __cplusplus
namespace RealtimeStreaming { namespace Network {
typedef ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::Networking::Sockets::StreamSocketListener*, ABI::Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs*> IConnectionReceivedEventHandler;
}}
#endif //__cplusplus
namespace ABI {
    namespace RealtimeStreaming {
        
        typedef MIDL_ENUM PayloadType : int PayloadType;
        
    } /* RealtimeStreaming */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        
        typedef MIDL_ENUM SampleFlags : int SampleFlags;
        
    } /* RealtimeStreaming */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        
        typedef struct MF_FLOAT2 MF_FLOAT2;
        
    } /* RealtimeStreaming */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        
        typedef struct PayloadHeader PayloadHeader;
        
    } /* RealtimeStreaming */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        
        typedef struct MediaTypeDescription MediaTypeDescription;
        
    } /* RealtimeStreaming */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        
        typedef struct MediaSampleHeader MediaSampleHeader;
        
    } /* RealtimeStreaming */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        
        typedef struct MediaSampleTransforms MediaSampleTransforms;
        
    } /* RealtimeStreaming */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        
        typedef struct MediaStreamTick MediaStreamTick;
        
    } /* RealtimeStreaming */} /* ABI */


namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            
            typedef MIDL_ENUM DeviceType : int DeviceType;
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            
            typedef MIDL_ENUM PluginEventType : int PluginEventType;
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */





namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            class ModuleManager;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */




namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            class PluginManager;
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */













namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class DataBundle;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */




namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class BundleReceivedArgs;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */












namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class Connector;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */




namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class Listener;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */




namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            
            typedef MIDL_ENUM SourceStreamState : int SourceStreamState;
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            
            typedef MIDL_ENUM SinkStreamState : int SinkStreamState;
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            
            typedef MIDL_ENUM SinkStreamOperation : int SinkStreamOperation;
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            
            typedef __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation IDeviceEnumerationOperation;
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            
            typedef __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation IDeviceEnumerationCompletedHandler;
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */








namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            class MrvcSchemeHandler;
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */

#ifndef ____x_ABI_CWindows_CMedia_CIMediaExtension_FWD_DEFINED__
#define ____x_ABI_CWindows_CMedia_CIMediaExtension_FWD_DEFINED__
namespace ABI {
    namespace Windows {
        namespace Media {
            interface IMediaExtension;
        } /* Windows */
    } /* Media */} /* ABI */
#define __x_ABI_CWindows_CMedia_CIMediaExtension ABI::Windows::Media::IMediaExtension

#endif // ____x_ABI_CWindows_CMedia_CIMediaExtension_FWD_DEFINED__




namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            class NetworkMediaSink;
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */




namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            class RealtimeServer;
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */





/*
 *
 * Struct RealtimeStreaming.PayloadType
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        /* [v1_enum, version] */
        MIDL_ENUM PayloadType : int
        {
            Unknown,
            State_Scene,
            State_Input,
            State_CaptureReady,
            State_CaptureStarted,
            State_CaptureStopped,
            RequestMediaDescription,
            RequestMediaStart,
            RequestMediaStop,
            RequestMediaCaptureStart,
            RequestMediaCaptureRecord,
            RequestMediaCaptureStop,
            RequestMediaEncodeStart,
            RequestMediaEncodeStop,
            SendMediaDescription,
            SendMediaSample,
            SendMediaStreamTick,
            SendFormatChange,
            ENDOFLIST,
        };
        const PayloadType PayloadType_Unknown = PayloadType::Unknown;
        const PayloadType PayloadType_State_Scene = PayloadType::State_Scene;
        const PayloadType PayloadType_State_Input = PayloadType::State_Input;
        const PayloadType PayloadType_State_CaptureReady = PayloadType::State_CaptureReady;
        const PayloadType PayloadType_State_CaptureStarted = PayloadType::State_CaptureStarted;
        const PayloadType PayloadType_State_CaptureStopped = PayloadType::State_CaptureStopped;
        const PayloadType PayloadType_RequestMediaDescription = PayloadType::RequestMediaDescription;
        const PayloadType PayloadType_RequestMediaStart = PayloadType::RequestMediaStart;
        const PayloadType PayloadType_RequestMediaStop = PayloadType::RequestMediaStop;
        const PayloadType PayloadType_RequestMediaCaptureStart = PayloadType::RequestMediaCaptureStart;
        const PayloadType PayloadType_RequestMediaCaptureRecord = PayloadType::RequestMediaCaptureRecord;
        const PayloadType PayloadType_RequestMediaCaptureStop = PayloadType::RequestMediaCaptureStop;
        const PayloadType PayloadType_RequestMediaEncodeStart = PayloadType::RequestMediaEncodeStart;
        const PayloadType PayloadType_RequestMediaEncodeStop = PayloadType::RequestMediaEncodeStop;
        const PayloadType PayloadType_SendMediaDescription = PayloadType::SendMediaDescription;
        const PayloadType PayloadType_SendMediaSample = PayloadType::SendMediaSample;
        const PayloadType PayloadType_SendMediaStreamTick = PayloadType::SendMediaStreamTick;
        const PayloadType PayloadType_SendFormatChange = PayloadType::SendFormatChange;
        const PayloadType PayloadType_ENDOFLIST = PayloadType::ENDOFLIST;
        
    } /* RealtimeStreaming */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.SampleFlags
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        /* [v1_enum, version] */
        MIDL_ENUM SampleFlags : int
        {
            SampleFlag_BottomFieldFirst = 0,
            SampleFlag_CleanPoint = 1,
            SampleFlag_DerivedFromTopField = 2,
            SampleFlag_Discontinuity = 3,
            SampleFlag_Interlaced = 4,
            SampleFlag_RepeatFirstField = 5,
            SampleFlag_SingleField = 6,
        };
        const SampleFlags SampleFlags_SampleFlag_BottomFieldFirst = SampleFlags::SampleFlag_BottomFieldFirst;
        const SampleFlags SampleFlags_SampleFlag_CleanPoint = SampleFlags::SampleFlag_CleanPoint;
        const SampleFlags SampleFlags_SampleFlag_DerivedFromTopField = SampleFlags::SampleFlag_DerivedFromTopField;
        const SampleFlags SampleFlags_SampleFlag_Discontinuity = SampleFlags::SampleFlag_Discontinuity;
        const SampleFlags SampleFlags_SampleFlag_Interlaced = SampleFlags::SampleFlag_Interlaced;
        const SampleFlags SampleFlags_SampleFlag_RepeatFirstField = SampleFlags::SampleFlag_RepeatFirstField;
        const SampleFlags SampleFlags_SampleFlag_SingleField = SampleFlags::SampleFlag_SingleField;
        
    } /* RealtimeStreaming */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.PayloadHeader
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        /* [version] */
        struct PayloadHeader
        {
            ABI::RealtimeStreaming::PayloadType ePayloadType;
            DWORD cbPayloadSize;
        };
        
    } /* RealtimeStreaming */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.MediaDescription
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        /* [version] */
        struct MediaDescription
        {
            DWORD StreamCount;
            DWORD StreamTypeHeaderSize;
        };
        
    } /* RealtimeStreaming */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.MediaTypeDescription
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        /* [version] */
        struct MediaTypeDescription
        {
            GUID guiMajorType;
            GUID guiSubType;
            DWORD dwStreamId;
            UINT32 AttributesBlobSize;
        };
        
    } /* RealtimeStreaming */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.MF_FLOAT2
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        /* [version] */
        struct MF_FLOAT2
        {
            FLOAT x;
            FLOAT y;
        };
        
    } /* RealtimeStreaming */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.MFCameraIntrinsic_DistortionModel
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        /* [version] */
        struct MFCameraIntrinsic_DistortionModel
        {
            FLOAT Radial_k1;
            FLOAT Radial_k2;
            FLOAT Radial_k3;
            FLOAT Tangential_p1;
            FLOAT Tangential_p2;
        };
        
    } /* RealtimeStreaming */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.MediaSampleHeader
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        /* [version] */
        struct MediaSampleHeader
        {
            DWORD dwStreamId;
            LONGLONG hnsTimestamp;
            LONGLONG hnsDuration;
            DWORD dwFlags;
            DWORD dwFlagMasks;
            DWORD cbCameraDataSize;
            ABI::Windows::Foundation::Numerics::Matrix4x4 worldToCameraMatrix;
            ABI::Windows::Foundation::Numerics::Matrix4x4 cameraProjectionTransform;
            ABI::Windows::Foundation::Numerics::Matrix4x4 cameraViewTransform;
        };
        
    } /* RealtimeStreaming */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.MediaSampleTransforms
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        /* [version] */
        struct MediaSampleTransforms
        {
            ABI::Windows::Foundation::Numerics::Matrix4x4 worldToCameraMatrix;
            ABI::Windows::Foundation::Numerics::Matrix4x4 cameraProjectionTransform;
            ABI::Windows::Foundation::Numerics::Matrix4x4 cameraViewTransform;
        };
        
    } /* RealtimeStreaming */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.MediaStreamTick
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        /* [version] */
        struct MediaStreamTick
        {
            DWORD dwStreamId;
            LONGLONG hnsTimestamp;
            UINT32 cbAttributesSize;
        };
        
    } /* RealtimeStreaming */} /* ABI */



/*
 *
 * Struct RealtimeStreaming.Plugin.DeviceType
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            /* [v1_enum, version] */
            MIDL_ENUM DeviceType : int
            {
                Null = 0,
                DX11 = 1,
                Warp = 2,
            };
            const DeviceType DeviceType_Null = DeviceType::Null;
            const DeviceType DeviceType_DX11 = DeviceType::DX11;
            const DeviceType DeviceType_Warp = DeviceType::Warp;
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.Plugin.PluginEventType
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            /* [v1_enum, version] */
            MIDL_ENUM PluginEventType : int
            {
                Update = 1,
                Render = 2,
                Flush = 3,
            };
            const PluginEventType PluginEventType_Update = PluginEventType::Update;
            const PluginEventType PluginEventType_Render = PluginEventType::Render;
            const PluginEventType PluginEventType_Flush = PluginEventType::Flush;
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */


/*
 *
 * Delegate RealtimeStreaming.Plugin.ClosedEventHandler
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_INTERFACE_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            /* [object, version, uuid("742c3239-1cf3-40bb-98c8-73270e0e337f")] */
            MIDL_INTERFACE("742c3239-1cf3-40bb-98c8-73270e0e337f")
            IClosedEventHandler : IUnknown
            {
                virtual HRESULT STDMETHODCALLTYPE Invoke(
                    /* [in] */IInspectable * sender
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IClosedEventHandler=_uuidof(IClosedEventHandler);
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Plugin.IModule
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Foundation.IClosable
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IModule[] = L"RealtimeStreaming.Plugin.IModule";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            /* [object, version, uuid("2edeb3fb-8008-4a13-b8d3-7081fd11443f")] */
            MIDL_INTERFACE("2edeb3fb-8008-4a13-b8d3-7081fd11443f")
            IModule : IInspectable
            {
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_IsInitialized(
                    /* [retval, out] */boolean * isInitialized
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE Uninitialize(void) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IModule=_uuidof(IModule);
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModule;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__) */


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
            /* [object, version, uuid("ee26ebc8-2eba-4476-be2a-4bf9389590bc"), exclusiveto] */
            MIDL_INTERFACE("ee26ebc8-2eba-4476-be2a-4bf9389590bc")
            IModuleManager : IInspectable
            {
                virtual HRESULT STDMETHODCALLTYPE AddModule(
                    /* [in] */ABI::RealtimeStreaming::Plugin::IModule * pluginModule,
                    /* [retval, out] */ABI::RealtimeStreaming::ModuleHandle * moduleHandle
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE GetModule(
                    /* [in] */ABI::RealtimeStreaming::ModuleHandle moduleHandle,
                    /* [retval, out] */ABI::RealtimeStreaming::Plugin::IModule * * pluginModule
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE ReleaseModule(
                    /* [in] */ABI::RealtimeStreaming::ModuleHandle moduleHandle
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE Uninitialize(void) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IModuleManager=_uuidof(IModuleManager);
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Plugin.IModuleManagerStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.ModuleManager
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IModuleManagerStatics[] = L"RealtimeStreaming.Plugin.IModuleManagerStatics";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            /* [object, version, uuid("5feb052c-d722-4f2a-8bf5-7d03ba808794"), exclusiveto] */
            MIDL_INTERFACE("5feb052c-d722-4f2a-8bf5-7d03ba808794")
            IModuleManagerStatics : IInspectable
            {
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_InvalidModuleHandle(
                    /* [retval, out] */ABI::RealtimeStreaming::ModuleHandle * moduleHandle
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IModuleManagerStatics=_uuidof(IModuleManagerStatics);
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Plugin.ModuleManager
 *
 * RuntimeClass can be activated.
 *
 * RuntimeClass contains static methods.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Plugin.IModuleManager ** Default Interface **
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Plugin_ModuleManager_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Plugin_ModuleManager_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Plugin_ModuleManager[] = L"RealtimeStreaming.Plugin.ModuleManager";
#endif


/*
 *
 * Interface RealtimeStreaming.Plugin.IPluginManager
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.PluginManager
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IPluginManager[] = L"RealtimeStreaming.Plugin.IPluginManager";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            /* [object, version, uuid("c9630182-dd7a-4fe8-b797-14aa4daf297a"), exclusiveto] */
            MIDL_INTERFACE("c9630182-dd7a-4fe8-b797-14aa4daf297a")
            IPluginManager : IInspectable
            {
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_ModuleManager(
                    /* [retval, out] */ABI::RealtimeStreaming::Plugin::IModuleManager * * moduleManager
                    ) = 0;
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_DirectXManager(
                    /* [retval, out] */ABI::RealtimeStreaming::Plugin::IDirectXManager * * dxManager
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IPluginManager=_uuidof(IPluginManager);
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Plugin.IPluginManagerStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.PluginManager
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IPluginManagerStatics[] = L"RealtimeStreaming.Plugin.IPluginManagerStatics";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            /* [object, version, uuid("172d17ad-464c-4da4-ad26-34d2a70d4f07"), exclusiveto] */
            MIDL_INTERFACE("172d17ad-464c-4da4-ad26-34d2a70d4f07")
            IPluginManagerStatics : IInspectable
            {
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_Instance(
                    /* [retval, out] */ABI::RealtimeStreaming::Plugin::IPluginManager * * pluginManager
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IPluginManagerStatics=_uuidof(IPluginManagerStatics);
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Plugin.PluginManager
 *
 * RuntimeClass contains static methods.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Plugin.IPluginManager ** Default Interface **
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Plugin_PluginManager_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Plugin_PluginManager_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Plugin_PluginManager[] = L"RealtimeStreaming.Plugin.PluginManager";
#endif


/*
 *
 * Interface RealtimeStreaming.Plugin.IDirectXManager
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IDirectXManager[] = L"RealtimeStreaming.Plugin.IDirectXManager";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Plugin {
            /* [object, version, uuid("712c3f26-ea19-44a2-b947-0313c7d41a2b")] */
            MIDL_INTERFACE("712c3f26-ea19-44a2-b947-0313c7d41a2b")
            IDirectXManager : IInspectable
            {
                
            };

            extern MIDL_CONST_ID IID & IID_IDirectXManager=_uuidof(IDirectXManager);
            
        } /* RealtimeStreaming */
    } /* Plugin */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_INTERFACE_DEFINED__) */





/*
 *
 * Delegate RealtimeStreaming.Network.DisconnectedEventHandler
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_INTERFACE_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("7f6533cb-58a5-4634-a0ca-1af12e82f508")] */
            MIDL_INTERFACE("7f6533cb-58a5-4634-a0ca-1af12e82f508")
            IDisconnectedEventHandler : IUnknown
            {
                virtual HRESULT STDMETHODCALLTYPE Invoke(
                    /* [in] */ABI::RealtimeStreaming::Network::IConnection * sender
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IDisconnectedEventHandler=_uuidof(IDisconnectedEventHandler);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_INTERFACE_DEFINED__) */


/*
 *
 * Delegate RealtimeStreaming.Network.BundleReceivedEventHandler
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_INTERFACE_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("351aade9-5b42-4ae8-9d72-dd5b765d3420")] */
            MIDL_INTERFACE("351aade9-5b42-4ae8-9d72-dd5b765d3420")
            IBundleReceivedEventHandler : IUnknown
            {
                virtual HRESULT STDMETHODCALLTYPE Invoke(
                    /* [in] */ABI::RealtimeStreaming::Network::IConnection * sender,
                    /* [in] */ABI::RealtimeStreaming::Network::IBundleReceivedArgs * args
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IBundleReceivedEventHandler=_uuidof(IBundleReceivedEventHandler);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_INTERFACE_DEFINED__) */


/*
 *
 * Delegate RealtimeStreaming.Network.ConnectedEventHandler
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_INTERFACE_DEFINED__
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("6f17b4ae-f506-4d6e-a666-893638581b19")] */
            MIDL_INTERFACE("6f17b4ae-f506-4d6e-a666-893638581b19")
            IConnectedEventHandler : IUnknown
            {
                virtual HRESULT STDMETHODCALLTYPE Invoke(
                    /* [in] */ABI::RealtimeStreaming::Network::IConnection * connection
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IConnectedEventHandler=_uuidof(IConnectedEventHandler);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IDataPacket
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Storage.Streams.IBuffer
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataPacket[] = L"RealtimeStreaming.Network.IDataPacket";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("3fcabce1-d85d-4990-93cc-64b26b8b9505")] */
            MIDL_INTERFACE("3fcabce1-d85d-4990-93cc-64b26b8b9505")
            IDataPacket : IInspectable
            {
                
            };

            extern MIDL_CONST_ID IID & IID_IDataPacket=_uuidof(IDataPacket);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_INTERFACE_DEFINED__) */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class DataPacket;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */



/*
 *
 * Class RealtimeStreaming.Network.DataPacket
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IDataPacket ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_DataPacket_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_DataPacket_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_DataPacket[] = L"RealtimeStreaming.Network.DataPacket";
#endif


/*
 *
 * Interface RealtimeStreaming.Network.IDataBuffer
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBuffer[] = L"RealtimeStreaming.Network.IDataBuffer";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("2fffa1dc-aa9b-4f0f-a67c-414da47613b8")] */
            MIDL_INTERFACE("2fffa1dc-aa9b-4f0f-a67c-414da47613b8")
            IDataBuffer : IInspectable
            {
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_Offset(
                    /* [retval, out] */DWORD * offset
                    ) = 0;
                /* [propput] */virtual HRESULT STDMETHODCALLTYPE put_Offset(
                    /* [in] */DWORD offset
                    ) = 0;
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_CurrentLength(
                    /* [retval, out] */DWORD * currentLength
                    ) = 0;
                /* [propput] */virtual HRESULT STDMETHODCALLTYPE put_CurrentLength(
                    /* [in] */DWORD currentLength
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE TrimLeft(
                    /* [in] */ULONG cbSize
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE TrimRight(
                    /* [in] */ULONG cbSize,
                    /* [out] */ABI::RealtimeStreaming::Network::IDataBuffer * * dataBuffer
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IDataBuffer=_uuidof(IDataBuffer);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IDataBufferStatics
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBufferStatics[] = L"RealtimeStreaming.Network.IDataBufferStatics";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("eb453df3-e098-4fcb-b76c-ef3838583033")] */
            MIDL_INTERFACE("eb453df3-e098-4fcb-b76c-ef3838583033")
            IDataBufferStatics : IInspectable
            {
                /* [overload] */virtual HRESULT STDMETHODCALLTYPE Create(
                    /* [in] */ULONG size
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IDataBufferStatics=_uuidof(IDataBufferStatics);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.DataBuffer
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IDataBuffer ** Default Interface **
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


/*
 *
 * Interface RealtimeStreaming.Network.IDataBufferLock
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBufferLock[] = L"RealtimeStreaming.Network.IDataBufferLock";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("4375ee9d-0302-416b-af1a-2b8ff632a06f")] */
            MIDL_INTERFACE("4375ee9d-0302-416b-af1a-2b8ff632a06f")
            IDataBufferLock : IInspectable
            {
                
            };

            extern MIDL_CONST_ID IID & IID_IDataBufferLock=_uuidof(IDataBufferLock);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_INTERFACE_DEFINED__) */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            class DataBufferLock;
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */



/*
 *
 * Class RealtimeStreaming.Network.DataBufferLock
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IDataBufferLock ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_DataBufferLock_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_DataBufferLock_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_DataBufferLock[] = L"RealtimeStreaming.Network.DataBufferLock";
#endif


/*
 *
 * Interface RealtimeStreaming.Network.IDataBundle
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBundle[] = L"RealtimeStreaming.Network.IDataBundle";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("049cd02e-12ec-45dc-8f58-931c342087ed")] */
            MIDL_INTERFACE("049cd02e-12ec-45dc-8f58-931c342087ed")
            IDataBundle : IInspectable
            {
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_BufferCount(
                    /* [retval, out] */UINT32 * count
                    ) = 0;
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_TotalSize(
                    /* [retval, out] */ULONG * size
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE AddBuffer(
                    /* [in] */ABI::RealtimeStreaming::Network::IDataBuffer * dataBuffer
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE InsertBuffer(
                    /* [in] */UINT32 index,
                    /* [in] */ABI::RealtimeStreaming::Network::IDataBuffer * dataBuffer
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE RemoveBuffer(
                    /* [in] */ABI::RealtimeStreaming::Network::IDataBuffer * dataBuffer
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE RemoveBufferByIndex(
                    /* [in] */UINT32 index,
                    /* [retval, out] */ABI::RealtimeStreaming::Network::IDataBuffer * * dataBuffer
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE Reset(void) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IDataBundle=_uuidof(IDataBundle);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IDataBundleStatics
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBundleStatics[] = L"RealtimeStreaming.Network.IDataBundleStatics";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("bac3c09b-76ca-44d9-ad40-080f3101884a")] */
            MIDL_INTERFACE("bac3c09b-76ca-44d9-ad40-080f3101884a")
            IDataBundleStatics : IInspectable
            {
                /* [overload] */virtual HRESULT STDMETHODCALLTYPE Create(
                    /* [in] */ABI::RealtimeStreaming::Network::IDataBuffer * dataBuffer
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IDataBundleStatics=_uuidof(IDataBundleStatics);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_INTERFACE_DEFINED__) */


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


/*
 *
 * Interface RealtimeStreaming.Network.IBundleReceivedArgs
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IBundleReceivedArgs[] = L"RealtimeStreaming.Network.IBundleReceivedArgs";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("c0d9ad8e-f586-4314-af4d-69b80513b470")] */
            MIDL_INTERFACE("c0d9ad8e-f586-4314-af4d-69b80513b470")
            IBundleReceivedArgs : IInspectable
            {
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_PayloadType(
                    /* [retval, out] */ABI::RealtimeStreaming::PayloadType * payloadType
                    ) = 0;
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_Connection(
                    /* [retval, out] */ABI::RealtimeStreaming::Network::IConnection * * connection
                    ) = 0;
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_DataBundle(
                    /* [retval, out] */ABI::RealtimeStreaming::Network::IDataBundle * * dataBundle
                    ) = 0;
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_RemoteUri(
                    /* [retval, out] */ABI::Windows::Foundation::IUriRuntimeClass * * uri
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IBundleReceivedArgs=_uuidof(IBundleReceivedArgs);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IBundleReceivedArgsStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.BundleReceivedArgs
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IBundleReceivedArgsStatics[] = L"RealtimeStreaming.Network.IBundleReceivedArgsStatics";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("3fa67b47-6b91-42ad-912c-ddd97ce02d50"), exclusiveto] */
            MIDL_INTERFACE("3fa67b47-6b91-42ad-912c-ddd97ce02d50")
            IBundleReceivedArgsStatics : IInspectable
            {
                /* [overload] */virtual HRESULT STDMETHODCALLTYPE Create(
                    /* [in] */ABI::RealtimeStreaming::PayloadType type,
                    /* [in] */ABI::RealtimeStreaming::Network::IConnection * connection,
                    /* [in] */ABI::RealtimeStreaming::Network::IDataBundle * dataBundle,
                    /* [in] */ABI::Windows::Foundation::IUriRuntimeClass * uri,
                    /* [retval, out] */ABI::RealtimeStreaming::Network::IBundleReceivedArgs * * args
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IBundleReceivedArgsStatics=_uuidof(IBundleReceivedArgsStatics);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.BundleReceivedArgs
 *
 * RuntimeClass can be activated.
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


/*
 *
 * Interface RealtimeStreaming.Network.IConnection
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connection
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     RealtimeStreaming.Network.IConnectionInternal
 *     RealtimeStreaming.Plugin.IModule
 *     Windows.Foundation.IClosable
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnection[] = L"RealtimeStreaming.Network.IConnection";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("027d0a6d-3ffb-480e-9e68-255cba264d40"), exclusiveto] */
            MIDL_INTERFACE("027d0a6d-3ffb-480e-9e68-255cba264d40")
            IConnection : IInspectable
            {
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_IsConnected(
                    /* [retval, out] */boolean * connected
                    ) = 0;
                /* [eventadd] */virtual HRESULT STDMETHODCALLTYPE add_Disconnected(
                    /* [in] */ABI::RealtimeStreaming::Network::IDisconnectedEventHandler  * eventHandler,
                    /* [retval, out] */EventRegistrationToken * token
                    ) = 0;
                /* [eventremove] */virtual HRESULT STDMETHODCALLTYPE remove_Disconnected(
                    /* [in] */EventRegistrationToken token
                    ) = 0;
                /* [eventadd] */virtual HRESULT STDMETHODCALLTYPE add_Received(
                    /* [in] */ABI::RealtimeStreaming::Network::IBundleReceivedEventHandler  * eventHandler,
                    /* [retval, out] */EventRegistrationToken * token
                    ) = 0;
                /* [eventremove] */virtual HRESULT STDMETHODCALLTYPE remove_Received(
                    /* [in] */EventRegistrationToken token
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE SendPayloadType(
                    /* [in] */ABI::RealtimeStreaming::PayloadType type
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE SendBundle(
                    /* [in] */ABI::RealtimeStreaming::Network::IDataBundle * dataBundle
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE SendBundleAsync(
                    /* [in] */ABI::RealtimeStreaming::Network::IDataBundle * dataBundle,
                    /* [retval, out] */ABI::Windows::Foundation::IAsyncAction * * asyncAction
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IConnection=_uuidof(IConnection);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnection;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IConnectionInternal
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connection
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnectionInternal[] = L"RealtimeStreaming.Network.IConnectionInternal";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("7e41360e-7307-4ee3-8f5d-b644e7e5c55a"), exclusiveto] */
            MIDL_INTERFACE("7e41360e-7307-4ee3-8f5d-b644e7e5c55a")
            IConnectionInternal : IInspectable
            {
                virtual HRESULT STDMETHODCALLTYPE CheckClosed(void) = 0;
                virtual HRESULT STDMETHODCALLTYPE WaitForHeader(void) = 0;
                virtual HRESULT STDMETHODCALLTYPE WaitForPayload(void) = 0;
                virtual HRESULT STDMETHODCALLTYPE ResetBundle(void) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IConnectionInternal=_uuidof(IConnectionInternal);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IConnectionStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connection
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnectionStatics[] = L"RealtimeStreaming.Network.IConnectionStatics";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("b3c719f9-b1c6-425a-9b21-8a36eaac7be4"), exclusiveto] */
            MIDL_INTERFACE("b3c719f9-b1c6-425a-9b21-8a36eaac7be4")
            IConnectionStatics : IInspectable
            {
                virtual HRESULT STDMETHODCALLTYPE Create(
                    /* [in] */ABI::Windows::Networking::Sockets::IStreamSocket * streamSocket,
                    /* [retval, out] */ABI::RealtimeStreaming::Network::IConnection * * connection
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IConnectionStatics=_uuidof(IConnectionStatics);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.Connection
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IConnection ** Default Interface **
 *    RealtimeStreaming.Network.IConnectionInternal
 *    RealtimeStreaming.Plugin.IModule
 *    Windows.Foundation.IClosable
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_Connection_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_Connection_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_Connection[] = L"RealtimeStreaming.Network.Connection";
#endif


/*
 *
 * Interface RealtimeStreaming.Network.IListener
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Listener
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Foundation.IClosable
 *     RealtimeStreaming.Plugin.IModule
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIListener_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIListener_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IListener[] = L"RealtimeStreaming.Network.IListener";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("887397c7-ce0f-4ced-86f1-54f4ee079d0b"), exclusiveto] */
            MIDL_INTERFACE("887397c7-ce0f-4ced-86f1-54f4ee079d0b")
            IListener : IInspectable
            {
                /* [eventadd] */virtual HRESULT STDMETHODCALLTYPE add_Closed(
                    /* [in] */ABI::RealtimeStreaming::Plugin::IClosedEventHandler  * eventHandler,
                    /* [retval, out] */EventRegistrationToken * token
                    ) = 0;
                /* [eventremove] */virtual HRESULT STDMETHODCALLTYPE remove_Closed(
                    /* [in] */EventRegistrationToken token
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE ListenAsync(
                    /* [retval, out] */__FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * * operation
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IListener=_uuidof(IListener);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIListener;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIListener_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IListenerStatics
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IListenerStatics[] = L"RealtimeStreaming.Network.IListenerStatics";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("0cfcc430-07f5-49c1-b86a-123da17030b5")] */
            MIDL_INTERFACE("0cfcc430-07f5-49c1-b86a-123da17030b5")
            IListenerStatics : IInspectable
            {
                /* [overload] */virtual HRESULT STDMETHODCALLTYPE Create(
                    /* [in] */UINT16 port,
                    /* [retval, out] */ABI::RealtimeStreaming::Network::IListener * * listener
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IListenerStatics=_uuidof(IListenerStatics);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.Listener
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IListener ** Default Interface **
 *    RealtimeStreaming.Plugin.IModule
 *    Windows.Foundation.IClosable
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_Listener_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_Listener_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_Listener[] = L"RealtimeStreaming.Network.Listener";
#endif


/*
 *
 * Interface RealtimeStreaming.Network.IConnector
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connector
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Foundation.IClosable
 *     RealtimeStreaming.Plugin.IModule
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnector[] = L"RealtimeStreaming.Network.IConnector";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("4d3288f4-b509-4ffe-848d-200218029096"), exclusiveto] */
            MIDL_INTERFACE("4d3288f4-b509-4ffe-848d-200218029096")
            IConnector : IInspectable
            {
                /* [eventadd] */virtual HRESULT STDMETHODCALLTYPE add_Closed(
                    /* [in] */ABI::RealtimeStreaming::Plugin::IClosedEventHandler  * eventHandler,
                    /* [retval, out] */EventRegistrationToken * token
                    ) = 0;
                /* [eventremove] */virtual HRESULT STDMETHODCALLTYPE remove_Closed(
                    /* [in] */EventRegistrationToken token
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE ConnectAsync(
                    /* [retval, out] */__FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * * operation
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IConnector=_uuidof(IConnector);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnector;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IConnectorStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connector
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnectorStatics[] = L"RealtimeStreaming.Network.IConnectorStatics";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Network {
            /* [object, version, uuid("5af2e9f2-5052-46f5-a118-85d281b11314"), exclusiveto] */
            MIDL_INTERFACE("5af2e9f2-5052-46f5-a118-85d281b11314")
            IConnectorStatics : IInspectable
            {
                virtual HRESULT STDMETHODCALLTYPE Create(
                    /* [in] */HSTRING hostAddress,
                    /* [in] */UINT16 port,
                    /* [retval, out] */ABI::RealtimeStreaming::Network::IConnector * * connector
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IConnectorStatics=_uuidof(IConnectorStatics);
            
        } /* RealtimeStreaming */
    } /* Network */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.Connector
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IConnector ** Default Interface **
 *    RealtimeStreaming.Plugin.IModule
 *    Windows.Foundation.IClosable
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




/*
 *
 * Struct RealtimeStreaming.Media.SourceStreamState
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            /* [v1_enum, version] */
            MIDL_ENUM SourceStreamState : int
            {
                Invalid,
                Opening,
                Starting,
                Started,
                Stopped,
                Shutdown,
                Count,
            };
            const SourceStreamState SourceStreamState_Invalid = SourceStreamState::Invalid;
            const SourceStreamState SourceStreamState_Opening = SourceStreamState::Opening;
            const SourceStreamState SourceStreamState_Starting = SourceStreamState::Starting;
            const SourceStreamState SourceStreamState_Started = SourceStreamState::Started;
            const SourceStreamState SourceStreamState_Stopped = SourceStreamState::Stopped;
            const SourceStreamState SourceStreamState_Shutdown = SourceStreamState::Shutdown;
            const SourceStreamState SourceStreamState_Count = SourceStreamState::Count;
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.Media.SinkStreamState
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            /* [v1_enum, version] */
            MIDL_ENUM SinkStreamState : int
            {
                NotSet = 0,
                Ready,
                Started,
                Stopped,
                Paused,
                Count,
            };
            const SinkStreamState SinkStreamState_NotSet = SinkStreamState::NotSet;
            const SinkStreamState SinkStreamState_Ready = SinkStreamState::Ready;
            const SinkStreamState SinkStreamState_Started = SinkStreamState::Started;
            const SinkStreamState SinkStreamState_Stopped = SinkStreamState::Stopped;
            const SinkStreamState SinkStreamState_Paused = SinkStreamState::Paused;
            const SinkStreamState SinkStreamState_Count = SinkStreamState::Count;
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.Media.SinkStreamOperation
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            /* [v1_enum, version] */
            MIDL_ENUM SinkStreamOperation : int
            {
                SetMediaType = 0,
                Start,
                Restart,
                Pause,
                Stop,
                ProcessSample,
                PlaceMarker,
                Count,
            };
            const SinkStreamOperation SinkStreamOperation_SetMediaType = SinkStreamOperation::SetMediaType;
            const SinkStreamOperation SinkStreamOperation_Start = SinkStreamOperation::Start;
            const SinkStreamOperation SinkStreamOperation_Restart = SinkStreamOperation::Restart;
            const SinkStreamOperation SinkStreamOperation_Pause = SinkStreamOperation::Pause;
            const SinkStreamOperation SinkStreamOperation_Stop = SinkStreamOperation::Stop;
            const SinkStreamOperation SinkStreamOperation_ProcessSample = SinkStreamOperation::ProcessSample;
            const SinkStreamOperation SinkStreamOperation_PlaceMarker = SinkStreamOperation::PlaceMarker;
            const SinkStreamOperation SinkStreamOperation_Count = SinkStreamOperation::Count;
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */


/*
 *
 * Struct RealtimeStreaming.Media.AudioMixerMode
 *
 */

namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            /* [v1_enum, version] */
            MIDL_ENUM AudioMixerMode : int
            {
                Mic = 0,
                Loopback = 1,
                MicAndLoopback = 2,
            };
            const AudioMixerMode AudioMixerMode_Mic = AudioMixerMode::Mic;
            const AudioMixerMode AudioMixerMode_Loopback = AudioMixerMode::Loopback;
            const AudioMixerMode AudioMixerMode_MicAndLoopback = AudioMixerMode::MicAndLoopback;
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */


/*
 *
 * Interface RealtimeStreaming.Media.IMrvcSchemeHandler
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.MrvcSchemeHandler
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Media.IMediaExtension
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_IMrvcSchemeHandler[] = L"RealtimeStreaming.Media.IMrvcSchemeHandler";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            /* [object, version, uuid("5a4a9427-1f66-40c6-ad6a-b3df73370112"), exclusiveto] */
            MIDL_INTERFACE("5a4a9427-1f66-40c6-ad6a-b3df73370112")
            IMrvcSchemeHandler : IInspectable
            {
                /* [propget] */virtual HRESULT STDMETHODCALLTYPE get_Connection(
                    /* [retval, out] */ABI::RealtimeStreaming::Network::IConnection * * connection
                    ) = 0;
                /* [propput] */virtual HRESULT STDMETHODCALLTYPE put_Connection(
                    /* [in] */ABI::RealtimeStreaming::Network::IConnection * connection
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IMrvcSchemeHandler=_uuidof(IMrvcSchemeHandler);
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Media.MrvcSchemeHandler
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    Windows.Media.IMediaExtension ** Default Interface **
 *    RealtimeStreaming.Media.IMrvcSchemeHandler
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Media_MrvcSchemeHandler_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Media_MrvcSchemeHandler_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Media_MrvcSchemeHandler[] = L"RealtimeStreaming.Media.MrvcSchemeHandler";
#endif


/*
 *
 * Interface RealtimeStreaming.Media.INetworkMediaSink
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.NetworkMediaSink
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Media.IMediaExtension
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_INetworkMediaSink[] = L"RealtimeStreaming.Media.INetworkMediaSink";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            /* [object, version, uuid("c9fff32c-31f3-452f-b126-3c5c6da57206"), exclusiveto] */
            MIDL_INTERFACE("c9fff32c-31f3-452f-b126-3c5c6da57206")
            INetworkMediaSink : IInspectable
            {
                /* [eventadd] */virtual HRESULT STDMETHODCALLTYPE add_Closed(
                    /* [in] */ABI::RealtimeStreaming::Plugin::IClosedEventHandler  * eventHandler,
                    /* [retval, out] */EventRegistrationToken * token
                    ) = 0;
                /* [eventremove] */virtual HRESULT STDMETHODCALLTYPE remove_Closed(
                    /* [in] */EventRegistrationToken token
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE OnEndOfStream(
                    /* [in] */DWORD streamId
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE HandleError(
                    /* [in] */HRESULT hr
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE CheckShutdown(void) = 0;
                virtual HRESULT STDMETHODCALLTYPE SendDescription(void) = 0;
                virtual HRESULT STDMETHODCALLTYPE SetMediaStreamProperties(
                    /* [in] */ABI::Windows::Media::Capture::MediaStreamType MediaStreamType,
                    /* [in] */ABI::Windows::Media::MediaProperties::IMediaEncodingProperties * mediaEncodingProperties
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_INetworkMediaSink=_uuidof(INetworkMediaSink);
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Media.NetworkMediaSink
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    Windows.Media.IMediaExtension ** Default Interface **
 *    RealtimeStreaming.Media.INetworkMediaSink
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Media_NetworkMediaSink_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Media_NetworkMediaSink_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Media_NetworkMediaSink[] = L"RealtimeStreaming.Media.NetworkMediaSink";
#endif


/*
 *
 * Interface RealtimeStreaming.Media.IRealtimeServer
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.RealtimeServer
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     RealtimeStreaming.Plugin.IModule
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_IRealtimeServer[] = L"RealtimeStreaming.Media.IRealtimeServer";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            /* [object, version, uuid("5e9858a9-5ef5-4b84-9645-eb3c11610a17"), exclusiveto] */
            MIDL_INTERFACE("5e9858a9-5ef5-4b84-9645-eb3c11610a17")
            IRealtimeServer : IInspectable
            {
                virtual HRESULT STDMETHODCALLTYPE Shutdown(void) = 0;
                virtual HRESULT STDMETHODCALLTYPE WriteFrame(
                    /* [in] */UINT32 bufferSize,
                    /* [size_is(bufferSize), in] */byte * buffer
                    ) = 0;
                virtual HRESULT STDMETHODCALLTYPE GetCurrentResolution(
                    /* [out] */UINT32 * pWidth,
                    /* [out] */UINT32 * pHeight
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IRealtimeServer=_uuidof(IRealtimeServer);
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Media.IRealtimeServerStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.RealtimeServer
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_IRealtimeServerStatics[] = L"RealtimeStreaming.Media.IRealtimeServerStatics";
namespace ABI {
    namespace RealtimeStreaming {
        namespace Media {
            /* [object, version, uuid("da706722-7bf4-4527-ad4c-2222cb81a7ab"), exclusiveto] */
            MIDL_INTERFACE("da706722-7bf4-4527-ad4c-2222cb81a7ab")
            IRealtimeServerStatics : IInspectable
            {
                virtual HRESULT STDMETHODCALLTYPE Create(
                    /* [in] */ABI::RealtimeStreaming::Network::IConnection * connection,
                    /* [in] */GUID inputMediaType,
                    /* [in] */ABI::Windows::Media::MediaProperties::IMediaEncodingProfile * mediaEncodingProfile,
                    /* [retval, out] */ABI::RealtimeStreaming::Media::IRealtimeServer * * RealtimeServer
                    ) = 0;
                
            };

            extern MIDL_CONST_ID IID & IID_IRealtimeServerStatics=_uuidof(IRealtimeServerStatics);
            
        } /* RealtimeStreaming */
    } /* Media */} /* ABI */

EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Media.RealtimeServer
 *
 * RuntimeClass contains static methods.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Media.IRealtimeServer ** Default Interface **
 *    RealtimeStreaming.Plugin.IModule
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
#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler;

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIModule __x_ABI_CRealtimeStreaming_CPlugin_CIModule;

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager;

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics;

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager;

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics;

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager;

#endif // ____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnection __x_ABI_CRealtimeStreaming_CNetwork_CIConnection;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIListener_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIListener_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIListener __x_ABI_CRealtimeStreaming_CNetwork_CIListener;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIListener_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnector __x_ABI_CRealtimeStreaming_CNetwork_CIConnector;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics;

#endif // ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler;

#endif // ____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink;

#endif // ____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer;

#endif // ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__

#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics;

#endif // ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_FWD_DEFINED__

// Parameterized interface forward declarations (C)

// Collection interface definitions
#ifndef ____x_ABI_CWindows_CDevices_CEnumeration_CIDeviceInformation_FWD_DEFINED__
#define ____x_ABI_CWindows_CDevices_CEnumeration_CIDeviceInformation_FWD_DEFINED__
typedef interface __x_ABI_CWindows_CDevices_CEnumeration_CIDeviceInformation __x_ABI_CWindows_CDevices_CEnumeration_CIDeviceInformation;

#endif // ____x_ABI_CWindows_CDevices_CEnumeration_CIDeviceInformation_FWD_DEFINED__


#if WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION >= 0x10000
#if !defined(____FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_INTERFACE_DEFINED__)
#define ____FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_INTERFACE_DEFINED__

typedef interface __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation;

//  Declare the parameterized interface IID.
EXTERN_C const IID IID___FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation;

// Forward declare the async operation.
typedef interface __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation;

typedef struct __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformationVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(__RPC__in __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation * This,
        /* [in] */ __RPC__in REFIID riid,
        /* [annotation][iid_is][out] */ 
        _COM_Outptr_  void **ppvObject);
    ULONG ( STDMETHODCALLTYPE *AddRef )(__RPC__in __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation * This);
    ULONG ( STDMETHODCALLTYPE *Release )(__RPC__in __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation * This);

    HRESULT ( STDMETHODCALLTYPE *Invoke )(__RPC__in __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation * This,/* [in] */ __RPC__in_opt __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation *asyncInfo, /* [in] */ AsyncStatus status);
    END_INTERFACE
} __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformationVtbl;

interface __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation
{
    CONST_VTBL struct __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformationVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 
#define __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 
#define __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 

#define __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_Invoke(This,asyncInfo,status)	\
    ( (This)->lpVtbl -> Invoke(This,asyncInfo,status) ) 
#endif /* COBJMACROS */


#endif // ____FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation_INTERFACE_DEFINED__

#endif // WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION >= 0x10000


#if WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION >= 0x10000
#if !defined(____FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_INTERFACE_DEFINED__)
#define ____FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_INTERFACE_DEFINED__

typedef interface __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation;

//  Declare the parameterized interface IID.
EXTERN_C const IID IID___FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation;

typedef struct __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformationVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(__RPC__in __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation * This,
        /* [in] */ __RPC__in REFIID riid,
        /* [annotation][iid_is][out] */ 
        _COM_Outptr_  void **ppvObject);
    ULONG ( STDMETHODCALLTYPE *AddRef )(__RPC__in __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation * This);
    ULONG ( STDMETHODCALLTYPE *Release )(__RPC__in __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation * This);

    HRESULT ( STDMETHODCALLTYPE *GetIids )(__RPC__in __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation * This,
        /* [out] */ __RPC__out ULONG *iidCount,
        /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids);
    HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(__RPC__in __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation * This, /* [out] */ __RPC__deref_out_opt HSTRING *className);
    HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(__RPC__in __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation * This, /* [out] */ __RPC__out TrustLevel *trustLevel);

    /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Completed )(__RPC__in __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation * This, /* [in] */ __RPC__in_opt __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation *handler);
    /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Completed )(__RPC__in __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation * This, /* [retval][out] */ __RPC__deref_out_opt __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation **handler);
    HRESULT ( STDMETHODCALLTYPE *GetResults )(__RPC__in __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation * This, /* [retval][out] */ __RPC__out __x_ABI_CWindows_CDevices_CEnumeration_CIDeviceInformation * *results);
    END_INTERFACE
} __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformationVtbl;

interface __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation
{
    CONST_VTBL struct __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformationVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 
#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 
#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 

#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 
#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 
#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 

#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_put_Completed(This,handler)	\
    ( (This)->lpVtbl -> put_Completed(This,handler) ) 
#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_get_Completed(This,handler)	\
    ( (This)->lpVtbl -> get_Completed(This,handler) ) 
#define __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_GetResults(This,results)	\
    ( (This)->lpVtbl -> GetResults(This,results) ) 
#endif /* COBJMACROS */


#endif // ____FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation_INTERFACE_DEFINED__

#endif // WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION >= 0x10000


#if !defined(____FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__)
#define ____FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__

typedef interface __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer;

//  Declare the parameterized interface IID.
EXTERN_C const IID IID___FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer;

typedef struct __FIIterator_1_RealtimeStreaming__CNetwork__CDataBufferVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
        __RPC__in __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [in] */ __RPC__in REFIID riid,
        /* [annotation][iid_is][out] */ 
        _COM_Outptr_  void **ppvObject);
    ULONG ( STDMETHODCALLTYPE *AddRef )(__RPC__in __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer * This);
    ULONG ( STDMETHODCALLTYPE *Release )(__RPC__in __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer * This);
    HRESULT ( STDMETHODCALLTYPE *GetIids )(__RPC__in __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [out] */ __RPC__out ULONG *iidCount,
        /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids);

    HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(__RPC__in __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [out] */ __RPC__deref_out_opt HSTRING *className);
    HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(__RPC__in __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [out] */ __RPC__out TrustLevel *trustLevel);

    /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Current )(__RPC__in __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [retval][out] */ __RPC__out __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * *current);
    /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HasCurrent )(__RPC__in __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [retval][out] */ __RPC__out boolean *hasCurrent);
    HRESULT ( STDMETHODCALLTYPE *MoveNext )(__RPC__in __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [retval][out] */ __RPC__out boolean *hasCurrent);
    HRESULT ( STDMETHODCALLTYPE *GetMany )(__RPC__in __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [in] */ unsigned int capacity,
        /* [size_is][length_is][out] */ __RPC__out_ecount_part(capacity, *actual) __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * *items,
        /* [retval][out] */ __RPC__out unsigned int *actual);

    END_INTERFACE
} __FIIterator_1_RealtimeStreaming__CNetwork__CDataBufferVtbl;

interface __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer
{
    CONST_VTBL struct __FIIterator_1_RealtimeStreaming__CNetwork__CDataBufferVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_get_Current(This,current)	\
    ( (This)->lpVtbl -> get_Current(This,current) ) 

#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_get_HasCurrent(This,hasCurrent)	\
    ( (This)->lpVtbl -> get_HasCurrent(This,hasCurrent) ) 

#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_MoveNext(This,hasCurrent)	\
    ( (This)->lpVtbl -> MoveNext(This,hasCurrent) ) 

#define __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_GetMany(This,capacity,items,actual)	\
    ( (This)->lpVtbl -> GetMany(This,capacity,items,actual) ) 

#endif /* COBJMACROS */


#endif // ____FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__



#if !defined(____FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__)
#define ____FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__

typedef interface __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer;

//  Declare the parameterized interface IID.
EXTERN_C const IID IID___FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer;

typedef  struct __FIIterable_1_RealtimeStreaming__CNetwork__CDataBufferVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
        __RPC__in __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [in] */ __RPC__in REFIID riid,
        /* [annotation][iid_is][out] */ 
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(__RPC__in __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer * This);

    ULONG ( STDMETHODCALLTYPE *Release )(__RPC__in __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer * This);

    HRESULT ( STDMETHODCALLTYPE *GetIids )(__RPC__in __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
                                           /* [out] */ __RPC__out ULONG *iidCount,
                                           /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids);

    HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(__RPC__in __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [out] */ __RPC__deref_out_opt HSTRING *className);

    HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(__RPC__in __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [out] */ __RPC__out TrustLevel *trustLevel);

    HRESULT ( STDMETHODCALLTYPE *First )(__RPC__in __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [retval][out] */ __RPC__deref_out_opt __FIIterator_1_RealtimeStreaming__CNetwork__CDataBuffer **first);

    END_INTERFACE
} __FIIterable_1_RealtimeStreaming__CNetwork__CDataBufferVtbl;

interface __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer
{
    CONST_VTBL struct __FIIterable_1_RealtimeStreaming__CNetwork__CDataBufferVtbl *lpVtbl;
};

#ifdef COBJMACROS

#define __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_First(This,first)	\
    ( (This)->lpVtbl -> First(This,first) ) 

#endif /* COBJMACROS */


#endif // ____FIIterable_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__



#if !defined(____FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__)
#define ____FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__

typedef interface __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer;

//  Declare the parameterized interface IID.
EXTERN_C const IID IID___FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer;

typedef struct __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBufferVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
        __RPC__in __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [in] */ __RPC__in REFIID riid,
        /* [annotation][iid_is][out] */ 
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )( __RPC__in __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer * This);

    ULONG ( STDMETHODCALLTYPE *Release )( __RPC__in __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer * This);

    HRESULT ( STDMETHODCALLTYPE *GetIids )( __RPC__in __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
                                            /* [out] */ __RPC__out ULONG *iidCount,
                                            /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids);

    HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
        __RPC__in __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
            /* [out] */ __RPC__deref_out_opt HSTRING *className);

    HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
        __RPC__in __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
            /* [out] */ __RPC__out TrustLevel *trustLevel);

    HRESULT ( STDMETHODCALLTYPE *GetAt )( 
                                         __RPC__in __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
                                         /* [in] */ unsigned int index,
                                         /* [retval][out] */ __RPC__out __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * *item);

        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Size )( 
            __RPC__in __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
            /* [retval][out] */ __RPC__out unsigned int *size);

        HRESULT ( STDMETHODCALLTYPE *IndexOf )( 
                                               __RPC__in __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * item,
            /* [out] */ __RPC__out unsigned int *index,
            /* [retval][out] */ __RPC__out boolean *found);

        HRESULT ( STDMETHODCALLTYPE *GetMany )( 
                                               __RPC__in __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
            /* [in] */ unsigned int startIndex,
            /* [in] */ unsigned int capacity,
            /* [size_is][length_is][out] */ __RPC__out_ecount_part(capacity, *actual) __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * *items,
            /* [retval][out] */ __RPC__out unsigned int *actual);

        END_INTERFACE
} __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBufferVtbl;

interface __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer
{
    CONST_VTBL struct __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBufferVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_GetAt(This,index,item)	\
    ( (This)->lpVtbl -> GetAt(This,index,item) ) 

#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_get_Size(This,size)	\
    ( (This)->lpVtbl -> get_Size(This,size) ) 

#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_IndexOf(This,item,index,found)	\
    ( (This)->lpVtbl -> IndexOf(This,item,index,found) ) 

#define __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_GetMany(This,startIndex,capacity,items,actual)	\
    ( (This)->lpVtbl -> GetMany(This,startIndex,capacity,items,actual) ) 

#endif /* COBJMACROS */



#endif // ____FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__



#if !defined(____FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__)
#define ____FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__

typedef interface __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer;

//  Declare the parameterized interface IID.
EXTERN_C const IID IID___FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer;

typedef struct __FIVector_1_RealtimeStreaming__CNetwork__CDataBufferVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
        __RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [annotation][iid_is][out] */ 
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This);
    ULONG ( STDMETHODCALLTYPE *Release )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This);
    HRESULT ( STDMETHODCALLTYPE *GetIids )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [out] */ __RPC__out ULONG *iidCount,
        /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids);

    HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [out] */ __RPC__deref_out_opt __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * *className);
    HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [out] */ __RPC__out TrustLevel *trustLevel);

    HRESULT ( STDMETHODCALLTYPE *GetAt )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [in] */ unsigned int index,
        /* [retval][out] */ __RPC__deref_out_opt __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * *item);

    /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Size )( 
        __RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [retval][out] */ __RPC__out unsigned int *size);

    HRESULT ( STDMETHODCALLTYPE *GetView )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [retval][out] */ __RPC__deref_out_opt __FIVectorView_1_RealtimeStreaming__CNetwork__CDataBuffer **view);

    HRESULT ( STDMETHODCALLTYPE *IndexOf )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [in] */ __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * item,
        /* [out] */ __RPC__out unsigned int *index,
        /* [retval][out] */ __RPC__out boolean *found);

    HRESULT ( STDMETHODCALLTYPE *SetAt )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [in] */ unsigned int index,
        /* [in] */ __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * item);

    HRESULT ( STDMETHODCALLTYPE *InsertAt )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [in] */ unsigned int index,
        /* [in] */ __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * item);

    HRESULT ( STDMETHODCALLTYPE *RemoveAt )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [in] */ unsigned int index);
    HRESULT ( STDMETHODCALLTYPE *Append )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This, /* [in] */ __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * item);
    HRESULT ( STDMETHODCALLTYPE *RemoveAtEnd )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This);
    HRESULT ( STDMETHODCALLTYPE *Clear )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This);
    HRESULT ( STDMETHODCALLTYPE *GetMany )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [in] */ unsigned int startIndex,
        /* [in] */ unsigned int capacity,
        /* [size_is][length_is][out] */ __RPC__out_ecount_part(capacity, *actual) __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * *items,
        /* [retval][out] */ __RPC__out unsigned int *actual);

    HRESULT ( STDMETHODCALLTYPE *ReplaceAll )(__RPC__in __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer * This,
        /* [in] */ unsigned int count,
        /* [size_is][in] */ __RPC__in_ecount_full(count) __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * *value);

    END_INTERFACE
} __FIVector_1_RealtimeStreaming__CNetwork__CDataBufferVtbl;

interface __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer
{
    CONST_VTBL struct __FIVector_1_RealtimeStreaming__CNetwork__CDataBufferVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_GetAt(This,index,item)	\
    ( (This)->lpVtbl -> GetAt(This,index,item) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_get_Size(This,size)	\
    ( (This)->lpVtbl -> get_Size(This,size) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_GetView(This,view)	\
    ( (This)->lpVtbl -> GetView(This,view) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_IndexOf(This,item,index,found)	\
    ( (This)->lpVtbl -> IndexOf(This,item,index,found) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_SetAt(This,index,item)	\
    ( (This)->lpVtbl -> SetAt(This,index,item) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_InsertAt(This,index,item)	\
    ( (This)->lpVtbl -> InsertAt(This,index,item) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_RemoveAt(This,index)	\
    ( (This)->lpVtbl -> RemoveAt(This,index) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_Append(This,item)	\
    ( (This)->lpVtbl -> Append(This,item) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_RemoveAtEnd(This)	\
    ( (This)->lpVtbl -> RemoveAtEnd(This) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_GetMany(This,startIndex,capacity,items,actual)	\
    ( (This)->lpVtbl -> GetMany(This,startIndex,capacity,items,actual) ) 

#define __FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_ReplaceAll(This,count,value)	\
    ( (This)->lpVtbl -> ReplaceAll(This,count,value) ) 

#endif /* COBJMACROS */



#endif // ____FIVector_1_RealtimeStreaming__CNetwork__CDataBuffer_INTERFACE_DEFINED__



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




typedef UINT32 __x_ABI_CRealtimeStreaming_CModuleHandle;


#ifndef MODULE_HANDLE

typedef UINT32 MODULE_HANDLE;

#define MODULE_HANDLE_INVALID (UINT)0x0bad

#define MODULE_HANDLE_START (UINT)0x0bae

#endif // MODULE_HANDLE

const ULONG c_cbReceiveBufferSize = 2 * 1024;

const ULONG c_cbMaxBundleSize = 1024 * 1024;

const UINT16 c_cbMaxBufferFailures = 7;

const UINT16 c_cbMaxBundleFailures = 3;

extern wchar_t const __declspec(selectany)c_szNetworkScheme[] = L"mrvc";

extern wchar_t const __declspec(selectany)c_szNetworkSchemeWithColon[] = L"mrvc:";


#ifdef __cplusplus
namespace RealtimeStreaming { namespace Network {
typedef ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::Networking::Sockets::StreamSocketListener*, ABI::Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs*> IConnectionReceivedEventHandler;
}}
#endif //__cplusplus

typedef enum __x_ABI_CRealtimeStreaming_CPayloadType __x_ABI_CRealtimeStreaming_CPayloadType;


typedef enum __x_ABI_CRealtimeStreaming_CSampleFlags __x_ABI_CRealtimeStreaming_CSampleFlags;


typedef struct __x_ABI_CRealtimeStreaming_CMF__FLOAT2 __x_ABI_CRealtimeStreaming_CMF__FLOAT2;


typedef struct __x_ABI_CRealtimeStreaming_CPayloadHeader __x_ABI_CRealtimeStreaming_CPayloadHeader;


typedef struct __x_ABI_CRealtimeStreaming_CMediaTypeDescription __x_ABI_CRealtimeStreaming_CMediaTypeDescription;


typedef struct __x_ABI_CRealtimeStreaming_CMediaSampleHeader __x_ABI_CRealtimeStreaming_CMediaSampleHeader;


typedef struct __x_ABI_CRealtimeStreaming_CMediaSampleTransforms __x_ABI_CRealtimeStreaming_CMediaSampleTransforms;


typedef struct __x_ABI_CRealtimeStreaming_CMediaStreamTick __x_ABI_CRealtimeStreaming_CMediaStreamTick;



typedef enum __x_ABI_CRealtimeStreaming_CPlugin_CDeviceType __x_ABI_CRealtimeStreaming_CPlugin_CDeviceType;


typedef enum __x_ABI_CRealtimeStreaming_CPlugin_CPluginEventType __x_ABI_CRealtimeStreaming_CPlugin_CPluginEventType;









































typedef enum __x_ABI_CRealtimeStreaming_CMedia_CSourceStreamState __x_ABI_CRealtimeStreaming_CMedia_CSourceStreamState;


typedef enum __x_ABI_CRealtimeStreaming_CMedia_CSinkStreamState __x_ABI_CRealtimeStreaming_CMedia_CSinkStreamState;


typedef enum __x_ABI_CRealtimeStreaming_CMedia_CSinkStreamOperation __x_ABI_CRealtimeStreaming_CMedia_CSinkStreamOperation;


typedef __FIAsyncOperation_1_Windows__CDevices__CEnumeration__CDeviceInformation __x_ABI_CRealtimeStreaming_CMedia_CIDeviceEnumerationOperation;


typedef __FIAsyncOperationCompletedHandler_1_Windows__CDevices__CEnumeration__CDeviceInformation __x_ABI_CRealtimeStreaming_CMedia_CIDeviceEnumerationCompletedHandler;








#ifndef ____x_ABI_CWindows_CMedia_CIMediaExtension_FWD_DEFINED__
#define ____x_ABI_CWindows_CMedia_CIMediaExtension_FWD_DEFINED__
typedef interface __x_ABI_CWindows_CMedia_CIMediaExtension __x_ABI_CWindows_CMedia_CIMediaExtension;

#endif // ____x_ABI_CWindows_CMedia_CIMediaExtension_FWD_DEFINED__











/*
 *
 * Struct RealtimeStreaming.PayloadType
 *
 */

/* [v1_enum, version] */
enum __x_ABI_CRealtimeStreaming_CPayloadType
{
    PayloadType_Unknown,
    PayloadType_State_Scene,
    PayloadType_State_Input,
    PayloadType_State_CaptureReady,
    PayloadType_State_CaptureStarted,
    PayloadType_State_CaptureStopped,
    PayloadType_RequestMediaDescription,
    PayloadType_RequestMediaStart,
    PayloadType_RequestMediaStop,
    PayloadType_RequestMediaCaptureStart,
    PayloadType_RequestMediaCaptureRecord,
    PayloadType_RequestMediaCaptureStop,
    PayloadType_RequestMediaEncodeStart,
    PayloadType_RequestMediaEncodeStop,
    PayloadType_SendMediaDescription,
    PayloadType_SendMediaSample,
    PayloadType_SendMediaStreamTick,
    PayloadType_SendFormatChange,
    PayloadType_ENDOFLIST,
};


/*
 *
 * Struct RealtimeStreaming.SampleFlags
 *
 */

/* [v1_enum, version] */
enum __x_ABI_CRealtimeStreaming_CSampleFlags
{
    SampleFlags_SampleFlag_BottomFieldFirst = 0,
    SampleFlags_SampleFlag_CleanPoint = 1,
    SampleFlags_SampleFlag_DerivedFromTopField = 2,
    SampleFlags_SampleFlag_Discontinuity = 3,
    SampleFlags_SampleFlag_Interlaced = 4,
    SampleFlags_SampleFlag_RepeatFirstField = 5,
    SampleFlags_SampleFlag_SingleField = 6,
};


/*
 *
 * Struct RealtimeStreaming.PayloadHeader
 *
 */

/* [version] */
struct __x_ABI_CRealtimeStreaming_CPayloadHeader
{
    __x_ABI_CRealtimeStreaming_CPayloadType ePayloadType;
    DWORD cbPayloadSize;
};


/*
 *
 * Struct RealtimeStreaming.MediaDescription
 *
 */

/* [version] */
struct __x_ABI_CRealtimeStreaming_CMediaDescription
{
    DWORD StreamCount;
    DWORD StreamTypeHeaderSize;
};


/*
 *
 * Struct RealtimeStreaming.MediaTypeDescription
 *
 */

/* [version] */
struct __x_ABI_CRealtimeStreaming_CMediaTypeDescription
{
    GUID guiMajorType;
    GUID guiSubType;
    DWORD dwStreamId;
    UINT32 AttributesBlobSize;
};


/*
 *
 * Struct RealtimeStreaming.MF_FLOAT2
 *
 */

/* [version] */
struct __x_ABI_CRealtimeStreaming_CMF__FLOAT2
{
    FLOAT x;
    FLOAT y;
};


/*
 *
 * Struct RealtimeStreaming.MFCameraIntrinsic_DistortionModel
 *
 */

/* [version] */
struct __x_ABI_CRealtimeStreaming_CMFCameraIntrinsic__DistortionModel
{
    FLOAT Radial_k1;
    FLOAT Radial_k2;
    FLOAT Radial_k3;
    FLOAT Tangential_p1;
    FLOAT Tangential_p2;
};


/*
 *
 * Struct RealtimeStreaming.MediaSampleHeader
 *
 */

/* [version] */
struct __x_ABI_CRealtimeStreaming_CMediaSampleHeader
{
    DWORD dwStreamId;
    LONGLONG hnsTimestamp;
    LONGLONG hnsDuration;
    DWORD dwFlags;
    DWORD dwFlagMasks;
    DWORD cbCameraDataSize;
    __x_ABI_CWindows_CFoundation_CNumerics_CMatrix4x4 worldToCameraMatrix;
    __x_ABI_CWindows_CFoundation_CNumerics_CMatrix4x4 cameraProjectionTransform;
    __x_ABI_CWindows_CFoundation_CNumerics_CMatrix4x4 cameraViewTransform;
};


/*
 *
 * Struct RealtimeStreaming.MediaSampleTransforms
 *
 */

/* [version] */
struct __x_ABI_CRealtimeStreaming_CMediaSampleTransforms
{
    __x_ABI_CWindows_CFoundation_CNumerics_CMatrix4x4 worldToCameraMatrix;
    __x_ABI_CWindows_CFoundation_CNumerics_CMatrix4x4 cameraProjectionTransform;
    __x_ABI_CWindows_CFoundation_CNumerics_CMatrix4x4 cameraViewTransform;
};


/*
 *
 * Struct RealtimeStreaming.MediaStreamTick
 *
 */

/* [version] */
struct __x_ABI_CRealtimeStreaming_CMediaStreamTick
{
    DWORD dwStreamId;
    LONGLONG hnsTimestamp;
    UINT32 cbAttributesSize;
};



/*
 *
 * Struct RealtimeStreaming.Plugin.DeviceType
 *
 */

/* [v1_enum, version] */
enum __x_ABI_CRealtimeStreaming_CPlugin_CDeviceType
{
    DeviceType_Null = 0,
    DeviceType_DX11 = 1,
    DeviceType_Warp = 2,
};


/*
 *
 * Struct RealtimeStreaming.Plugin.PluginEventType
 *
 */

/* [v1_enum, version] */
enum __x_ABI_CRealtimeStreaming_CPlugin_CPluginEventType
{
    PluginEventType_Update = 1,
    PluginEventType_Render = 2,
    PluginEventType_Flush = 3,
};


/*
 *
 * Delegate RealtimeStreaming.Plugin.ClosedEventHandler
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_INTERFACE_DEFINED__
/* [object, version, uuid("742c3239-1cf3-40bb-98c8-73270e0e337f")] */
typedef struct __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandlerVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject);

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler * This);

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler * This);
HRESULT ( STDMETHODCALLTYPE *Invoke )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler * This,
        /* [in] */IInspectable * sender
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandlerVtbl;

interface __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandlerVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_Invoke(This,sender) \
    ( (This)->lpVtbl->Invoke(This,sender) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Plugin.IModule
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Foundation.IClosable
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IModule[] = L"RealtimeStreaming.Plugin.IModule";
/* [object, version, uuid("2edeb3fb-8008-4a13-b8d3-7081fd11443f")] */
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
/* [propget] */HRESULT ( STDMETHODCALLTYPE *get_IsInitialized )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This,
        /* [retval, out] */boolean * isInitialized
        );
    HRESULT ( STDMETHODCALLTYPE *Uninitialize )(
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

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_get_IsInitialized(This,isInitialized) \
    ( (This)->lpVtbl->get_IsInitialized(This,isInitialized) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_Uninitialize(This) \
    ( (This)->lpVtbl->Uninitialize(This) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModule;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__) */


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
/* [object, version, uuid("ee26ebc8-2eba-4476-be2a-4bf9389590bc"), exclusiveto] */
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
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CModuleHandle * moduleHandle
        );
    HRESULT ( STDMETHODCALLTYPE *GetModule )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CModuleHandle moduleHandle,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CPlugin_CIModule * * pluginModule
        );
    HRESULT ( STDMETHODCALLTYPE *ReleaseModule )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CModuleHandle moduleHandle
        );
    HRESULT ( STDMETHODCALLTYPE *Uninitialize )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This
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

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_AddModule(This,pluginModule,moduleHandle) \
    ( (This)->lpVtbl->AddModule(This,pluginModule,moduleHandle) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_GetModule(This,moduleHandle,pluginModule) \
    ( (This)->lpVtbl->GetModule(This,moduleHandle,pluginModule) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_ReleaseModule(This,moduleHandle) \
    ( (This)->lpVtbl->ReleaseModule(This,moduleHandle) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_Uninitialize(This) \
    ( (This)->lpVtbl->Uninitialize(This) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Plugin.IModuleManagerStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.ModuleManager
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IModuleManagerStatics[] = L"RealtimeStreaming.Plugin.IModuleManagerStatics";
/* [object, version, uuid("5feb052c-d722-4f2a-8bf5-7d03ba808794"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStaticsVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [propget] */HRESULT ( STDMETHODCALLTYPE *get_InvalidModuleHandle )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics * This,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CModuleHandle * moduleHandle
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStaticsVtbl;

interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStaticsVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_get_InvalidModuleHandle(This,moduleHandle) \
    ( (This)->lpVtbl->get_InvalidModuleHandle(This,moduleHandle) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Plugin.ModuleManager
 *
 * RuntimeClass can be activated.
 *
 * RuntimeClass contains static methods.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Plugin.IModuleManager ** Default Interface **
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Plugin_ModuleManager_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Plugin_ModuleManager_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Plugin_ModuleManager[] = L"RealtimeStreaming.Plugin.ModuleManager";
#endif


/*
 *
 * Interface RealtimeStreaming.Plugin.IPluginManager
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.PluginManager
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IPluginManager[] = L"RealtimeStreaming.Plugin.IPluginManager";
/* [object, version, uuid("c9630182-dd7a-4fe8-b797-14aa4daf297a"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [propget] */HRESULT ( STDMETHODCALLTYPE *get_ModuleManager )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager * This,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * * moduleManager
        );
    /* [propget] */HRESULT ( STDMETHODCALLTYPE *get_DirectXManager )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager * This,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager * * dxManager
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerVtbl;

interface __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_get_ModuleManager(This,moduleManager) \
    ( (This)->lpVtbl->get_ModuleManager(This,moduleManager) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_get_DirectXManager(This,dxManager) \
    ( (This)->lpVtbl->get_DirectXManager(This,dxManager) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Plugin.IPluginManagerStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Plugin.PluginManager
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IPluginManagerStatics[] = L"RealtimeStreaming.Plugin.IPluginManagerStatics";
/* [object, version, uuid("172d17ad-464c-4da4-ad26-34d2a70d4f07"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStaticsVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [propget] */HRESULT ( STDMETHODCALLTYPE *get_Instance )(
        __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics * This,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CPlugin_CIPluginManager * * pluginManager
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStaticsVtbl;

interface __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStaticsVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_get_Instance(This,pluginManager) \
    ( (This)->lpVtbl->get_Instance(This,pluginManager) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIPluginManagerStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Plugin.PluginManager
 *
 * RuntimeClass contains static methods.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Plugin.IPluginManager ** Default Interface **
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Plugin_PluginManager_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Plugin_PluginManager_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Plugin_PluginManager[] = L"RealtimeStreaming.Plugin.PluginManager";
#endif


/*
 *
 * Interface RealtimeStreaming.Plugin.IDirectXManager
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Plugin_IDirectXManager[] = L"RealtimeStreaming.Plugin.IDirectXManager";
/* [object, version, uuid("712c3f26-ea19-44a2-b947-0313c7d41a2b")] */
typedef struct __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManagerVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManagerVtbl;

interface __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManagerVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CPlugin_CIDirectXManager_INTERFACE_DEFINED__) */





/*
 *
 * Delegate RealtimeStreaming.Network.DisconnectedEventHandler
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_INTERFACE_DEFINED__
/* [object, version, uuid("7f6533cb-58a5-4634-a0ca-1af12e82f508")] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandlerVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject);

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler * This);

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler * This);
HRESULT ( STDMETHODCALLTYPE *Invoke )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnection * sender
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandlerVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandlerVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_Invoke(This,sender) \
    ( (This)->lpVtbl->Invoke(This,sender) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler_INTERFACE_DEFINED__) */


/*
 *
 * Delegate RealtimeStreaming.Network.BundleReceivedEventHandler
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_INTERFACE_DEFINED__
/* [object, version, uuid("351aade9-5b42-4ae8-9d72-dd5b765d3420")] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandlerVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject);

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler * This);

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler * This);
HRESULT ( STDMETHODCALLTYPE *Invoke )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnection * sender,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * args
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandlerVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandlerVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_Invoke(This,sender,args) \
    ( (This)->lpVtbl->Invoke(This,sender,args) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler_INTERFACE_DEFINED__) */


/*
 *
 * Delegate RealtimeStreaming.Network.ConnectedEventHandler
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_INTERFACE_DEFINED__
/* [object, version, uuid("6f17b4ae-f506-4d6e-a666-893638581b19")] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandlerVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject);

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler * This);

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler * This);
HRESULT ( STDMETHODCALLTYPE *Invoke )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnection * connection
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandlerVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandlerVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_Invoke(This,connection) \
    ( (This)->lpVtbl->Invoke(This,connection) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectedEventHandler_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IDataPacket
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Storage.Streams.IBuffer
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataPacket[] = L"RealtimeStreaming.Network.IDataPacket";
/* [object, version, uuid("3fcabce1-d85d-4990-93cc-64b26b8b9505")] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacketVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacketVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacketVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataPacket_INTERFACE_DEFINED__) */



/*
 *
 * Class RealtimeStreaming.Network.DataPacket
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IDataPacket ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_DataPacket_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_DataPacket_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_DataPacket[] = L"RealtimeStreaming.Network.DataPacket";
#endif


/*
 *
 * Interface RealtimeStreaming.Network.IDataBuffer
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBuffer[] = L"RealtimeStreaming.Network.IDataBuffer";
/* [object, version, uuid("2fffa1dc-aa9b-4f0f-a67c-414da47613b8")] */
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
/* [propget] */HRESULT ( STDMETHODCALLTYPE *get_Offset )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [retval, out] */DWORD * offset
        );
    /* [propput] */HRESULT ( STDMETHODCALLTYPE *put_Offset )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [in] */DWORD offset
        );
    /* [propget] */HRESULT ( STDMETHODCALLTYPE *get_CurrentLength )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [retval, out] */DWORD * currentLength
        );
    /* [propput] */HRESULT ( STDMETHODCALLTYPE *put_CurrentLength )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [in] */DWORD currentLength
        );
    HRESULT ( STDMETHODCALLTYPE *TrimLeft )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [in] */ULONG cbSize
        );
    HRESULT ( STDMETHODCALLTYPE *TrimRight )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
        /* [in] */ULONG cbSize,
        /* [out] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * * dataBuffer
        );
    HRESULT ( STDMETHODCALLTYPE *Reset )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This
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

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_get_Offset(This,offset) \
    ( (This)->lpVtbl->get_Offset(This,offset) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_put_Offset(This,offset) \
    ( (This)->lpVtbl->put_Offset(This,offset) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_get_CurrentLength(This,currentLength) \
    ( (This)->lpVtbl->get_CurrentLength(This,currentLength) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_put_CurrentLength(This,currentLength) \
    ( (This)->lpVtbl->put_CurrentLength(This,currentLength) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_TrimLeft(This,cbSize) \
    ( (This)->lpVtbl->TrimLeft(This,cbSize) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_TrimRight(This,cbSize,dataBuffer) \
    ( (This)->lpVtbl->TrimRight(This,cbSize,dataBuffer) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_Reset(This) \
    ( (This)->lpVtbl->Reset(This) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IDataBufferStatics
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBufferStatics[] = L"RealtimeStreaming.Network.IDataBufferStatics";
/* [object, version, uuid("eb453df3-e098-4fcb-b76c-ef3838583033")] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStaticsVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [overload] */HRESULT ( STDMETHODCALLTYPE *Create )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics * This,
        /* [in] */ULONG size
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStaticsVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStaticsVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_Create(This,size) \
    ( (This)->lpVtbl->Create(This,size) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.DataBuffer
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IDataBuffer ** Default Interface **
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


/*
 *
 * Interface RealtimeStreaming.Network.IDataBufferLock
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBufferLock[] = L"RealtimeStreaming.Network.IDataBufferLock";
/* [object, version, uuid("4375ee9d-0302-416b-af1a-2b8ff632a06f")] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLockVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLockVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLockVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferLock_INTERFACE_DEFINED__) */



/*
 *
 * Class RealtimeStreaming.Network.DataBufferLock
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IDataBufferLock ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_DataBufferLock_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_DataBufferLock_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_DataBufferLock[] = L"RealtimeStreaming.Network.DataBufferLock";
#endif


/*
 *
 * Interface RealtimeStreaming.Network.IDataBundle
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBundle[] = L"RealtimeStreaming.Network.IDataBundle";
/* [object, version, uuid("049cd02e-12ec-45dc-8f58-931c342087ed")] */
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
        /* [retval, out] */UINT32 * count
        );
    /* [propget] */HRESULT ( STDMETHODCALLTYPE *get_TotalSize )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [retval, out] */ULONG * size
        );
    HRESULT ( STDMETHODCALLTYPE *AddBuffer )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * dataBuffer
        );
    HRESULT ( STDMETHODCALLTYPE *InsertBuffer )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [in] */UINT32 index,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * dataBuffer
        );
    HRESULT ( STDMETHODCALLTYPE *RemoveBuffer )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * dataBuffer
        );
    HRESULT ( STDMETHODCALLTYPE *RemoveBufferByIndex )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
        /* [in] */UINT32 index,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * * dataBuffer
        );
    HRESULT ( STDMETHODCALLTYPE *Reset )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This
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

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_get_BufferCount(This,count) \
    ( (This)->lpVtbl->get_BufferCount(This,count) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_get_TotalSize(This,size) \
    ( (This)->lpVtbl->get_TotalSize(This,size) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_AddBuffer(This,dataBuffer) \
    ( (This)->lpVtbl->AddBuffer(This,dataBuffer) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_InsertBuffer(This,index,dataBuffer) \
    ( (This)->lpVtbl->InsertBuffer(This,index,dataBuffer) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_RemoveBuffer(This,dataBuffer) \
    ( (This)->lpVtbl->RemoveBuffer(This,dataBuffer) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_RemoveBufferByIndex(This,index,dataBuffer) \
    ( (This)->lpVtbl->RemoveBufferByIndex(This,index,dataBuffer) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_Reset(This) \
    ( (This)->lpVtbl->Reset(This) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IDataBundleStatics
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IDataBundleStatics[] = L"RealtimeStreaming.Network.IDataBundleStatics";
/* [object, version, uuid("bac3c09b-76ca-44d9-ad40-080f3101884a")] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStaticsVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [overload] */HRESULT ( STDMETHODCALLTYPE *Create )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * dataBuffer
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStaticsVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStaticsVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_Create(This,dataBuffer) \
    ( (This)->lpVtbl->Create(This,dataBuffer) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleStatics_INTERFACE_DEFINED__) */


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


/*
 *
 * Interface RealtimeStreaming.Network.IBundleReceivedArgs
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IBundleReceivedArgs[] = L"RealtimeStreaming.Network.IBundleReceivedArgs";
/* [object, version, uuid("c0d9ad8e-f586-4314-af4d-69b80513b470")] */
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
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CPayloadType * payloadType
        );
    /* [propget] */HRESULT ( STDMETHODCALLTYPE *get_Connection )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnection * * connection
        );
    /* [propget] */HRESULT ( STDMETHODCALLTYPE *get_DataBundle )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * * dataBundle
        );
    /* [propget] */HRESULT ( STDMETHODCALLTYPE *get_RemoteUri )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
        /* [retval, out] */__x_ABI_CWindows_CFoundation_CIUriRuntimeClass * * uri
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

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_get_PayloadType(This,payloadType) \
    ( (This)->lpVtbl->get_PayloadType(This,payloadType) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_get_Connection(This,connection) \
    ( (This)->lpVtbl->get_Connection(This,connection) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_get_DataBundle(This,dataBundle) \
    ( (This)->lpVtbl->get_DataBundle(This,dataBundle) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_get_RemoteUri(This,uri) \
    ( (This)->lpVtbl->get_RemoteUri(This,uri) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IBundleReceivedArgsStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.BundleReceivedArgs
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IBundleReceivedArgsStatics[] = L"RealtimeStreaming.Network.IBundleReceivedArgsStatics";
/* [object, version, uuid("3fa67b47-6b91-42ad-912c-ddd97ce02d50"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStaticsVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [overload] */HRESULT ( STDMETHODCALLTYPE *Create )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CPayloadType type,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnection * connection,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * dataBundle,
        /* [in] */__x_ABI_CWindows_CFoundation_CIUriRuntimeClass * uri,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * * args
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStaticsVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStaticsVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_Create(This,type,connection,dataBundle,uri,args) \
    ( (This)->lpVtbl->Create(This,type,connection,dataBundle,uri,args) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.BundleReceivedArgs
 *
 * RuntimeClass can be activated.
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


/*
 *
 * Interface RealtimeStreaming.Network.IConnection
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connection
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     RealtimeStreaming.Network.IConnectionInternal
 *     RealtimeStreaming.Plugin.IModule
 *     Windows.Foundation.IClosable
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnection[] = L"RealtimeStreaming.Network.IConnection";
/* [object, version, uuid("027d0a6d-3ffb-480e-9e68-255cba264d40"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [propget] */HRESULT ( STDMETHODCALLTYPE *get_IsConnected )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
        /* [retval, out] */boolean * connected
        );
    /* [eventadd] */HRESULT ( STDMETHODCALLTYPE *add_Disconnected )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIDisconnectedEventHandler  * eventHandler,
        /* [retval, out] */EventRegistrationToken * token
        );
    /* [eventremove] */HRESULT ( STDMETHODCALLTYPE *remove_Disconnected )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
        /* [in] */EventRegistrationToken token
        );
    /* [eventadd] */HRESULT ( STDMETHODCALLTYPE *add_Received )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedEventHandler  * eventHandler,
        /* [retval, out] */EventRegistrationToken * token
        );
    /* [eventremove] */HRESULT ( STDMETHODCALLTYPE *remove_Received )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
        /* [in] */EventRegistrationToken token
        );
    HRESULT ( STDMETHODCALLTYPE *SendPayloadType )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CPayloadType type
        );
    HRESULT ( STDMETHODCALLTYPE *SendBundle )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * dataBundle
        );
    HRESULT ( STDMETHODCALLTYPE *SendBundleAsync )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * dataBundle,
        /* [retval, out] */__x_ABI_CWindows_CFoundation_CIAsyncAction * * asyncAction
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnection
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_get_IsConnected(This,connected) \
    ( (This)->lpVtbl->get_IsConnected(This,connected) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_add_Disconnected(This,eventHandler,token) \
    ( (This)->lpVtbl->add_Disconnected(This,eventHandler,token) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_remove_Disconnected(This,token) \
    ( (This)->lpVtbl->remove_Disconnected(This,token) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_add_Received(This,eventHandler,token) \
    ( (This)->lpVtbl->add_Received(This,eventHandler,token) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_remove_Received(This,token) \
    ( (This)->lpVtbl->remove_Received(This,token) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_SendPayloadType(This,type) \
    ( (This)->lpVtbl->SendPayloadType(This,type) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_SendBundle(This,dataBundle) \
    ( (This)->lpVtbl->SendBundle(This,dataBundle) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_SendBundleAsync(This,dataBundle,asyncAction) \
    ( (This)->lpVtbl->SendBundleAsync(This,dataBundle,asyncAction) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnection;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IConnectionInternal
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connection
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnectionInternal[] = L"RealtimeStreaming.Network.IConnectionInternal";
/* [object, version, uuid("7e41360e-7307-4ee3-8f5d-b644e7e5c55a"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternalVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *CheckClosed )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal * This
        );
    HRESULT ( STDMETHODCALLTYPE *WaitForHeader )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal * This
        );
    HRESULT ( STDMETHODCALLTYPE *WaitForPayload )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal * This
        );
    HRESULT ( STDMETHODCALLTYPE *ResetBundle )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal * This
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternalVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternalVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_CheckClosed(This) \
    ( (This)->lpVtbl->CheckClosed(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_WaitForHeader(This) \
    ( (This)->lpVtbl->WaitForHeader(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_WaitForPayload(This) \
    ( (This)->lpVtbl->WaitForPayload(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_ResetBundle(This) \
    ( (This)->lpVtbl->ResetBundle(This) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionInternal_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IConnectionStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connection
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnectionStatics[] = L"RealtimeStreaming.Network.IConnectionStatics";
/* [object, version, uuid("b3c719f9-b1c6-425a-9b21-8a36eaac7be4"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStaticsVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *Create )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics * This,
        /* [in] */__x_ABI_CWindows_CNetworking_CSockets_CIStreamSocket * streamSocket,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnection * * connection
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStaticsVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStaticsVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_Create(This,streamSocket,connection) \
    ( (This)->lpVtbl->Create(This,streamSocket,connection) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectionStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.Connection
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IConnection ** Default Interface **
 *    RealtimeStreaming.Network.IConnectionInternal
 *    RealtimeStreaming.Plugin.IModule
 *    Windows.Foundation.IClosable
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_Connection_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_Connection_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_Connection[] = L"RealtimeStreaming.Network.Connection";
#endif


/*
 *
 * Interface RealtimeStreaming.Network.IListener
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Listener
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Foundation.IClosable
 *     RealtimeStreaming.Plugin.IModule
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIListener_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIListener_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IListener[] = L"RealtimeStreaming.Network.IListener";
/* [object, version, uuid("887397c7-ce0f-4ced-86f1-54f4ee079d0b"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIListenerVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListener * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListener * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListener * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListener * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListener * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListener * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [eventadd] */HRESULT ( STDMETHODCALLTYPE *add_Closed )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIListener * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler  * eventHandler,
        /* [retval, out] */EventRegistrationToken * token
        );
    /* [eventremove] */HRESULT ( STDMETHODCALLTYPE *remove_Closed )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIListener * This,
        /* [in] */EventRegistrationToken token
        );
    HRESULT ( STDMETHODCALLTYPE *ListenAsync )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIListener * This,
        /* [retval, out] */__FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * * operation
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIListenerVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIListener
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIListenerVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIListener_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListener_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListener_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListener_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListener_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListener_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListener_add_Closed(This,eventHandler,token) \
    ( (This)->lpVtbl->add_Closed(This,eventHandler,token) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListener_remove_Closed(This,token) \
    ( (This)->lpVtbl->remove_Closed(This,token) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListener_ListenAsync(This,operation) \
    ( (This)->lpVtbl->ListenAsync(This,operation) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIListener;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIListener_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IListenerStatics
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IListenerStatics[] = L"RealtimeStreaming.Network.IListenerStatics";
/* [object, version, uuid("0cfcc430-07f5-49c1-b86a-123da17030b5")] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStaticsVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [overload] */HRESULT ( STDMETHODCALLTYPE *Create )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics * This,
        /* [in] */UINT16 port,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CNetwork_CIListener * * listener
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStaticsVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStaticsVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_Create(This,port,listener) \
    ( (This)->lpVtbl->Create(This,port,listener) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIListenerStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.Listener
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IListener ** Default Interface **
 *    RealtimeStreaming.Plugin.IModule
 *    Windows.Foundation.IClosable
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Network_Listener_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Network_Listener_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Network_Listener[] = L"RealtimeStreaming.Network.Listener";
#endif


/*
 *
 * Interface RealtimeStreaming.Network.IConnector
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connector
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Foundation.IClosable
 *     RealtimeStreaming.Plugin.IModule
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnector[] = L"RealtimeStreaming.Network.IConnector";
/* [object, version, uuid("4d3288f4-b509-4ffe-848d-200218029096"), exclusiveto] */
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
/* [eventadd] */HRESULT ( STDMETHODCALLTYPE *add_Closed )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler  * eventHandler,
        /* [retval, out] */EventRegistrationToken * token
        );
    /* [eventremove] */HRESULT ( STDMETHODCALLTYPE *remove_Closed )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
        /* [in] */EventRegistrationToken token
        );
    HRESULT ( STDMETHODCALLTYPE *ConnectAsync )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
        /* [retval, out] */__FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * * operation
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

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_add_Closed(This,eventHandler,token) \
    ( (This)->lpVtbl->add_Closed(This,eventHandler,token) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_remove_Closed(This,token) \
    ( (This)->lpVtbl->remove_Closed(This,token) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_ConnectAsync(This,operation) \
    ( (This)->lpVtbl->ConnectAsync(This,operation) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnector;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Network.IConnectorStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Network.Connector
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Network_IConnectorStatics[] = L"RealtimeStreaming.Network.IConnectorStatics";
/* [object, version, uuid("5af2e9f2-5052-46f5-a118-85d281b11314"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStaticsVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *Create )(
        __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics * This,
        /* [in] */HSTRING hostAddress,
        /* [in] */UINT16 port,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnector * * connector
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStaticsVtbl;

interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStaticsVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_Create(This,hostAddress,port,connector) \
    ( (This)->lpVtbl->Create(This,hostAddress,port,connector) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CNetwork_CIConnectorStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Network.Connector
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Network.IConnector ** Default Interface **
 *    RealtimeStreaming.Plugin.IModule
 *    Windows.Foundation.IClosable
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




/*
 *
 * Struct RealtimeStreaming.Media.SourceStreamState
 *
 */

/* [v1_enum, version] */
enum __x_ABI_CRealtimeStreaming_CMedia_CSourceStreamState
{
    SourceStreamState_Invalid,
    SourceStreamState_Opening,
    SourceStreamState_Starting,
    SourceStreamState_Started,
    SourceStreamState_Stopped,
    SourceStreamState_Shutdown,
    SourceStreamState_Count,
};


/*
 *
 * Struct RealtimeStreaming.Media.SinkStreamState
 *
 */

/* [v1_enum, version] */
enum __x_ABI_CRealtimeStreaming_CMedia_CSinkStreamState
{
    SinkStreamState_NotSet = 0,
    SinkStreamState_Ready,
    SinkStreamState_Started,
    SinkStreamState_Stopped,
    SinkStreamState_Paused,
    SinkStreamState_Count,
};


/*
 *
 * Struct RealtimeStreaming.Media.SinkStreamOperation
 *
 */

/* [v1_enum, version] */
enum __x_ABI_CRealtimeStreaming_CMedia_CSinkStreamOperation
{
    SinkStreamOperation_SetMediaType = 0,
    SinkStreamOperation_Start,
    SinkStreamOperation_Restart,
    SinkStreamOperation_Pause,
    SinkStreamOperation_Stop,
    SinkStreamOperation_ProcessSample,
    SinkStreamOperation_PlaceMarker,
    SinkStreamOperation_Count,
};


/*
 *
 * Struct RealtimeStreaming.Media.AudioMixerMode
 *
 */

/* [v1_enum, version] */
enum __x_ABI_CRealtimeStreaming_CMedia_CAudioMixerMode
{
    AudioMixerMode_Mic = 0,
    AudioMixerMode_Loopback = 1,
    AudioMixerMode_MicAndLoopback = 2,
};


/*
 *
 * Interface RealtimeStreaming.Media.IMrvcSchemeHandler
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.MrvcSchemeHandler
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Media.IMediaExtension
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_IMrvcSchemeHandler[] = L"RealtimeStreaming.Media.IMrvcSchemeHandler";
/* [object, version, uuid("5a4a9427-1f66-40c6-ad6a-b3df73370112"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandlerVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [propget] */HRESULT ( STDMETHODCALLTYPE *get_Connection )(
        __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler * This,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnection * * connection
        );
    /* [propput] */HRESULT ( STDMETHODCALLTYPE *put_Connection )(
        __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnection * connection
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandlerVtbl;

interface __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandlerVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_get_Connection(This,connection) \
    ( (This)->lpVtbl->get_Connection(This,connection) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_put_Connection(This,connection) \
    ( (This)->lpVtbl->put_Connection(This,connection) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CIMrvcSchemeHandler_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Media.MrvcSchemeHandler
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    Windows.Media.IMediaExtension ** Default Interface **
 *    RealtimeStreaming.Media.IMrvcSchemeHandler
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Media_MrvcSchemeHandler_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Media_MrvcSchemeHandler_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Media_MrvcSchemeHandler[] = L"RealtimeStreaming.Media.MrvcSchemeHandler";
#endif


/*
 *
 * Interface RealtimeStreaming.Media.INetworkMediaSink
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.NetworkMediaSink
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     Windows.Media.IMediaExtension
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_INetworkMediaSink[] = L"RealtimeStreaming.Media.INetworkMediaSink";
/* [object, version, uuid("c9fff32c-31f3-452f-b126-3c5c6da57206"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSinkVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
/* [eventadd] */HRESULT ( STDMETHODCALLTYPE *add_Closed )(
        __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CPlugin_CIClosedEventHandler  * eventHandler,
        /* [retval, out] */EventRegistrationToken * token
        );
    /* [eventremove] */HRESULT ( STDMETHODCALLTYPE *remove_Closed )(
        __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This,
        /* [in] */EventRegistrationToken token
        );
    HRESULT ( STDMETHODCALLTYPE *OnEndOfStream )(
        __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This,
        /* [in] */DWORD streamId
        );
    HRESULT ( STDMETHODCALLTYPE *HandleError )(
        __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This,
        /* [in] */HRESULT hr
        );
    HRESULT ( STDMETHODCALLTYPE *CheckShutdown )(
        __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This
        );
    HRESULT ( STDMETHODCALLTYPE *SendDescription )(
        __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This
        );
    HRESULT ( STDMETHODCALLTYPE *SetMediaStreamProperties )(
        __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink * This,
        /* [in] */__x_ABI_CWindows_CMedia_CCapture_CMediaStreamType MediaStreamType,
        /* [in] */__x_ABI_CWindows_CMedia_CMediaProperties_CIMediaEncodingProperties * mediaEncodingProperties
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSinkVtbl;

interface __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSinkVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_add_Closed(This,eventHandler,token) \
    ( (This)->lpVtbl->add_Closed(This,eventHandler,token) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_remove_Closed(This,token) \
    ( (This)->lpVtbl->remove_Closed(This,token) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_OnEndOfStream(This,streamId) \
    ( (This)->lpVtbl->OnEndOfStream(This,streamId) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_HandleError(This,hr) \
    ( (This)->lpVtbl->HandleError(This,hr) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_CheckShutdown(This) \
    ( (This)->lpVtbl->CheckShutdown(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_SendDescription(This) \
    ( (This)->lpVtbl->SendDescription(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_SetMediaStreamProperties(This,MediaStreamType,mediaEncodingProperties) \
    ( (This)->lpVtbl->SetMediaStreamProperties(This,MediaStreamType,mediaEncodingProperties) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CINetworkMediaSink_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Media.NetworkMediaSink
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    Windows.Media.IMediaExtension ** Default Interface **
 *    RealtimeStreaming.Media.INetworkMediaSink
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_RealtimeStreaming_Media_NetworkMediaSink_DEFINED
#define RUNTIMECLASS_RealtimeStreaming_Media_NetworkMediaSink_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_RealtimeStreaming_Media_NetworkMediaSink[] = L"RealtimeStreaming.Media.NetworkMediaSink";
#endif


/*
 *
 * Interface RealtimeStreaming.Media.IRealtimeServer
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.RealtimeServer
 *
 *
 * Any object which implements this interface must also implement the following interfaces:
 *     RealtimeStreaming.Plugin.IModule
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_IRealtimeServer[] = L"RealtimeStreaming.Media.IRealtimeServer";
/* [object, version, uuid("5e9858a9-5ef5-4b84-9645-eb3c11610a17"), exclusiveto] */
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
HRESULT ( STDMETHODCALLTYPE *Shutdown )(
        __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This
        );
    HRESULT ( STDMETHODCALLTYPE *WriteFrame )(
        __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
        /* [in] */UINT32 bufferSize,
        /* [size_is(bufferSize), in] */byte * buffer
        );
    HRESULT ( STDMETHODCALLTYPE *GetCurrentResolution )(
        __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
        /* [out] */UINT32 * pWidth,
        /* [out] */UINT32 * pHeight
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

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_Shutdown(This) \
    ( (This)->lpVtbl->Shutdown(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_WriteFrame(This,bufferSize,buffer) \
    ( (This)->lpVtbl->WriteFrame(This,bufferSize,buffer) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_GetCurrentResolution(This,pWidth,pHeight) \
    ( (This)->lpVtbl->GetCurrentResolution(This,pWidth,pHeight) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__) */


/*
 *
 * Interface RealtimeStreaming.Media.IRealtimeServerStatics
 *
 * Interface is a part of the implementation of type RealtimeStreaming.Media.RealtimeServer
 *
 *
 */
#if !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_INTERFACE_DEFINED__)
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_RealtimeStreaming_Media_IRealtimeServerStatics[] = L"RealtimeStreaming.Media.IRealtimeServerStatics";
/* [object, version, uuid("da706722-7bf4-4527-ad4c-2222cb81a7ab"), exclusiveto] */
typedef struct __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStaticsVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *Create )(
        __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics * This,
        /* [in] */__x_ABI_CRealtimeStreaming_CNetwork_CIConnection * connection,
        /* [in] */GUID inputMediaType,
        /* [in] */__x_ABI_CWindows_CMedia_CMediaProperties_CIMediaEncodingProfile * mediaEncodingProfile,
        /* [retval, out] */__x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * * RealtimeServer
        );
    END_INTERFACE
    
} __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStaticsVtbl;

interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics
{
    CONST_VTBL struct __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStaticsVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_Create(This,connection,inputMediaType,mediaEncodingProfile,RealtimeServer) \
    ( (This)->lpVtbl->Create(This,connection,inputMediaType,mediaEncodingProfile,RealtimeServer) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics;
#endif /* !defined(____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerStatics_INTERFACE_DEFINED__) */


/*
 *
 * Class RealtimeStreaming.Media.RealtimeServer
 *
 * RuntimeClass contains static methods.
 *
 * Class implements the following interfaces:
 *    RealtimeStreaming.Media.IRealtimeServer ** Default Interface **
 *    RealtimeStreaming.Plugin.IModule
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
#endif // __RealtimeStreaming_p_h__

#endif // __RealtimeStreaming_h__
