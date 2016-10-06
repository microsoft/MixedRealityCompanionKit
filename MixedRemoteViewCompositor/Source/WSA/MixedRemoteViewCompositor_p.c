

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.01.0620 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for C:\Users\carmines\AppData\Local\Temp\MixedRemoteViewCompositor.idl-dc9448ec:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0620 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "MixedRemoteViewCompositor.h"

#define TYPE_FORMAT_STRING_SIZE   1189                              
#define PROC_FORMAT_STRING_SIZE   4411                              
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

typedef struct _MixedRemoteViewCompositor_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MixedRemoteViewCompositor_MIDL_TYPE_FORMAT_STRING;

typedef struct _MixedRemoteViewCompositor_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MixedRemoteViewCompositor_MIDL_PROC_FORMAT_STRING;

typedef struct _MixedRemoteViewCompositor_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } MixedRemoteViewCompositor_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MixedRemoteViewCompositor_MIDL_TYPE_FORMAT_STRING MixedRemoteViewCompositor__MIDL_TypeFormatString;
extern const MixedRemoteViewCompositor_MIDL_PROC_FORMAT_STRING MixedRemoteViewCompositor__MIDL_ProcFormatString;
extern const MixedRemoteViewCompositor_MIDL_EXPR_FORMAT_STRING MixedRemoteViewCompositor__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MixedRemoteViewCompositor_MIDL_PROC_FORMAT_STRING MixedRemoteViewCompositor__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure Invoke */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x3 ),	/* 3 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sender */

/* 24 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IsConnected */


	/* Procedure get_IsInitialized */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x6 ),	/* 6 */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x21 ),	/* 33 */
/* 50 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 52 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter connected */


	/* Parameter isInitialized */

/* 60 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 66 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure WaitForHeader */


	/* Procedure Uninitialize */

/* 72 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x7 ),	/* 7 */
/* 80 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x8 ),	/* 8 */
/* 86 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 88 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 90 */	NdrFcShort( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */


	/* Return value */

/* 96 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 98 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddModule */

/* 102 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 104 */	NdrFcLong( 0x0 ),	/* 0 */
/* 108 */	NdrFcShort( 0x6 ),	/* 6 */
/* 110 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0x24 ),	/* 36 */
/* 116 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 118 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 124 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pluginModule */

/* 126 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 128 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 130 */	NdrFcShort( 0x18 ),	/* Type Offset=24 */

	/* Parameter moduleHandle */

/* 132 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 134 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 136 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetModule */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0x7 ),	/* 7 */
/* 152 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 154 */	NdrFcShort( 0x8 ),	/* 8 */
/* 156 */	NdrFcShort( 0x8 ),	/* 8 */
/* 158 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 160 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter moduleHandle */

/* 168 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pluginModule */

/* 174 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 180 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 182 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 184 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReleaseModule */

/* 186 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 188 */	NdrFcLong( 0x0 ),	/* 0 */
/* 192 */	NdrFcShort( 0x8 ),	/* 8 */
/* 194 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 196 */	NdrFcShort( 0x8 ),	/* 8 */
/* 198 */	NdrFcShort( 0x8 ),	/* 8 */
/* 200 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 202 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 206 */	NdrFcShort( 0x0 ),	/* 0 */
/* 208 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter moduleHandle */

/* 210 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 212 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 216 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 218 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 220 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ResetBundle */


	/* Procedure Uninitialize */

/* 222 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 224 */	NdrFcLong( 0x0 ),	/* 0 */
/* 228 */	NdrFcShort( 0x9 ),	/* 9 */
/* 230 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 232 */	NdrFcShort( 0x0 ),	/* 0 */
/* 234 */	NdrFcShort( 0x8 ),	/* 8 */
/* 236 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 238 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 240 */	NdrFcShort( 0x0 ),	/* 0 */
/* 242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 244 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */


	/* Return value */

/* 246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 248 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Width */


	/* Procedure get_Width */


	/* Procedure get_BufferCount */


	/* Procedure get_Offset */


	/* Procedure get_InvalidModuleHandle */

/* 252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0x6 ),	/* 6 */
/* 260 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 264 */	NdrFcShort( 0x24 ),	/* 36 */
/* 266 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 268 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0x0 ),	/* 0 */
/* 274 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter width */


	/* Parameter width */


	/* Parameter count */


	/* Parameter offset */


	/* Parameter moduleHandle */

/* 276 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 278 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */

/* 282 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 284 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ModuleManager */

/* 288 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 290 */	NdrFcLong( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0x6 ),	/* 6 */
/* 296 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 300 */	NdrFcShort( 0x8 ),	/* 8 */
/* 302 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 304 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 310 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter moduleManager */

/* 312 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 314 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 316 */	NdrFcShort( 0x32 ),	/* Type Offset=50 */

	/* Return value */

/* 318 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 320 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_DirectXManager */

/* 324 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 326 */	NdrFcLong( 0x0 ),	/* 0 */
/* 330 */	NdrFcShort( 0x7 ),	/* 7 */
/* 332 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 334 */	NdrFcShort( 0x0 ),	/* 0 */
/* 336 */	NdrFcShort( 0x8 ),	/* 8 */
/* 338 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 340 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dxManager */

/* 348 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 350 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 352 */	NdrFcShort( 0x48 ),	/* Type Offset=72 */

	/* Return value */

/* 354 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 356 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 358 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Instance */

/* 360 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 362 */	NdrFcLong( 0x0 ),	/* 0 */
/* 366 */	NdrFcShort( 0x6 ),	/* 6 */
/* 368 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x8 ),	/* 8 */
/* 374 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 376 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 378 */	NdrFcShort( 0x0 ),	/* 0 */
/* 380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 382 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pluginManager */

/* 384 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 386 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 388 */	NdrFcShort( 0x5e ),	/* Type Offset=94 */

	/* Return value */

/* 390 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 392 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Current */

/* 396 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 398 */	NdrFcLong( 0x0 ),	/* 0 */
/* 402 */	NdrFcShort( 0x6 ),	/* 6 */
/* 404 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0x8 ),	/* 8 */
/* 410 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 412 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 414 */	NdrFcShort( 0x0 ),	/* 0 */
/* 416 */	NdrFcShort( 0x0 ),	/* 0 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter current */

/* 420 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 422 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 424 */	NdrFcShort( 0x74 ),	/* Type Offset=116 */

	/* Return value */

/* 426 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 428 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 430 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_HasCurrent */

/* 432 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 434 */	NdrFcLong( 0x0 ),	/* 0 */
/* 438 */	NdrFcShort( 0x7 ),	/* 7 */
/* 440 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 444 */	NdrFcShort( 0x21 ),	/* 33 */
/* 446 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 448 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 450 */	NdrFcShort( 0x0 ),	/* 0 */
/* 452 */	NdrFcShort( 0x0 ),	/* 0 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hasCurrent */

/* 456 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 458 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 460 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 462 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 464 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 466 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_HologramComposition */


	/* Procedure MoveNext */

/* 468 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 470 */	NdrFcLong( 0x0 ),	/* 0 */
/* 474 */	NdrFcShort( 0x8 ),	/* 8 */
/* 476 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 480 */	NdrFcShort( 0x21 ),	/* 33 */
/* 482 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 484 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 486 */	NdrFcShort( 0x0 ),	/* 0 */
/* 488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 490 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */


	/* Parameter hasCurrent */

/* 492 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 494 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 496 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 498 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 500 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 502 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetMany */

/* 504 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 506 */	NdrFcLong( 0x0 ),	/* 0 */
/* 510 */	NdrFcShort( 0x9 ),	/* 9 */
/* 512 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 514 */	NdrFcShort( 0x8 ),	/* 8 */
/* 516 */	NdrFcShort( 0x24 ),	/* 36 */
/* 518 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x4,		/* 4 */
/* 520 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 522 */	NdrFcShort( 0x1 ),	/* 1 */
/* 524 */	NdrFcShort( 0x0 ),	/* 0 */
/* 526 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter capacity */

/* 528 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 530 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 532 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter items */

/* 534 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 536 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 538 */	NdrFcShort( 0x8e ),	/* Type Offset=142 */

	/* Parameter actual */

/* 540 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 542 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 544 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 546 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 548 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 550 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure First */

/* 552 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 554 */	NdrFcLong( 0x0 ),	/* 0 */
/* 558 */	NdrFcShort( 0x6 ),	/* 6 */
/* 560 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 562 */	NdrFcShort( 0x0 ),	/* 0 */
/* 564 */	NdrFcShort( 0x8 ),	/* 8 */
/* 566 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 568 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 570 */	NdrFcShort( 0x0 ),	/* 0 */
/* 572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 574 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter first */

/* 576 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 578 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 580 */	NdrFcShort( 0xa4 ),	/* Type Offset=164 */

	/* Return value */

/* 582 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 584 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 586 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAt */


	/* Procedure GetAt */

/* 588 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 590 */	NdrFcLong( 0x0 ),	/* 0 */
/* 594 */	NdrFcShort( 0x6 ),	/* 6 */
/* 596 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 598 */	NdrFcShort( 0x8 ),	/* 8 */
/* 600 */	NdrFcShort( 0x8 ),	/* 8 */
/* 602 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 604 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 606 */	NdrFcShort( 0x0 ),	/* 0 */
/* 608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */


	/* Parameter index */

/* 612 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 614 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 616 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter item */


	/* Parameter item */

/* 618 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 620 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 622 */	NdrFcShort( 0x74 ),	/* Type Offset=116 */

	/* Return value */


	/* Return value */

/* 624 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 626 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 628 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Height */


	/* Procedure get_Height */


	/* Procedure get_TotalSize */


	/* Procedure get_Size */


	/* Procedure get_Size */

/* 630 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 632 */	NdrFcLong( 0x0 ),	/* 0 */
/* 636 */	NdrFcShort( 0x7 ),	/* 7 */
/* 638 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 642 */	NdrFcShort( 0x24 ),	/* 36 */
/* 644 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 646 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 652 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter height */


	/* Parameter height */


	/* Parameter size */


	/* Parameter size */


	/* Parameter size */

/* 654 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 656 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 658 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */

/* 660 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 662 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 664 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IndexOf */

/* 666 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 668 */	NdrFcLong( 0x0 ),	/* 0 */
/* 672 */	NdrFcShort( 0x8 ),	/* 8 */
/* 674 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 678 */	NdrFcShort( 0x3d ),	/* 61 */
/* 680 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 682 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 684 */	NdrFcShort( 0x0 ),	/* 0 */
/* 686 */	NdrFcShort( 0x0 ),	/* 0 */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter item */

/* 690 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 692 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 694 */	NdrFcShort( 0x78 ),	/* Type Offset=120 */

	/* Parameter index */

/* 696 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 698 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 700 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter found */

/* 702 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 704 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 706 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 708 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 710 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 712 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetMany */

/* 714 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 716 */	NdrFcLong( 0x0 ),	/* 0 */
/* 720 */	NdrFcShort( 0x9 ),	/* 9 */
/* 722 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 724 */	NdrFcShort( 0x10 ),	/* 16 */
/* 726 */	NdrFcShort( 0x24 ),	/* 36 */
/* 728 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x5,		/* 5 */
/* 730 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 732 */	NdrFcShort( 0x1 ),	/* 1 */
/* 734 */	NdrFcShort( 0x0 ),	/* 0 */
/* 736 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter startIndex */

/* 738 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 740 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 742 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter capacity */

/* 744 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 746 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 748 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter items */

/* 750 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 752 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 754 */	NdrFcShort( 0xbe ),	/* Type Offset=190 */

	/* Parameter actual */

/* 756 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 758 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 760 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 762 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 764 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 766 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetView */

/* 768 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 770 */	NdrFcLong( 0x0 ),	/* 0 */
/* 774 */	NdrFcShort( 0x8 ),	/* 8 */
/* 776 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x8 ),	/* 8 */
/* 782 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 784 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 786 */	NdrFcShort( 0x0 ),	/* 0 */
/* 788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 790 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter view */

/* 792 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 794 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 796 */	NdrFcShort( 0xd4 ),	/* Type Offset=212 */

	/* Return value */

/* 798 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 800 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 802 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure IndexOf */

/* 804 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 806 */	NdrFcLong( 0x0 ),	/* 0 */
/* 810 */	NdrFcShort( 0x9 ),	/* 9 */
/* 812 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 816 */	NdrFcShort( 0x3d ),	/* 61 */
/* 818 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 820 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 824 */	NdrFcShort( 0x0 ),	/* 0 */
/* 826 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter item */

/* 828 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 830 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 832 */	NdrFcShort( 0x78 ),	/* Type Offset=120 */

	/* Parameter index */

/* 834 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 836 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 838 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter found */

/* 840 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 842 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 844 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 846 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 848 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 850 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetAt */

/* 852 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 854 */	NdrFcLong( 0x0 ),	/* 0 */
/* 858 */	NdrFcShort( 0xa ),	/* 10 */
/* 860 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 862 */	NdrFcShort( 0x8 ),	/* 8 */
/* 864 */	NdrFcShort( 0x8 ),	/* 8 */
/* 866 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 868 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 870 */	NdrFcShort( 0x0 ),	/* 0 */
/* 872 */	NdrFcShort( 0x0 ),	/* 0 */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 876 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 878 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 880 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter item */

/* 882 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 884 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 886 */	NdrFcShort( 0x78 ),	/* Type Offset=120 */

	/* Return value */

/* 888 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 890 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 892 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure InsertAt */

/* 894 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 896 */	NdrFcLong( 0x0 ),	/* 0 */
/* 900 */	NdrFcShort( 0xb ),	/* 11 */
/* 902 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 904 */	NdrFcShort( 0x8 ),	/* 8 */
/* 906 */	NdrFcShort( 0x8 ),	/* 8 */
/* 908 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 910 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 912 */	NdrFcShort( 0x0 ),	/* 0 */
/* 914 */	NdrFcShort( 0x0 ),	/* 0 */
/* 916 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 918 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 920 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 922 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter item */

/* 924 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 926 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 928 */	NdrFcShort( 0x78 ),	/* Type Offset=120 */

	/* Return value */

/* 930 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 932 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 934 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveAt */

/* 936 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 938 */	NdrFcLong( 0x0 ),	/* 0 */
/* 942 */	NdrFcShort( 0xc ),	/* 12 */
/* 944 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 946 */	NdrFcShort( 0x8 ),	/* 8 */
/* 948 */	NdrFcShort( 0x8 ),	/* 8 */
/* 950 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 952 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 954 */	NdrFcShort( 0x0 ),	/* 0 */
/* 956 */	NdrFcShort( 0x0 ),	/* 0 */
/* 958 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 960 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 962 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 964 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 966 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 968 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 970 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Append */

/* 972 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 974 */	NdrFcLong( 0x0 ),	/* 0 */
/* 978 */	NdrFcShort( 0xd ),	/* 13 */
/* 980 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 982 */	NdrFcShort( 0x0 ),	/* 0 */
/* 984 */	NdrFcShort( 0x8 ),	/* 8 */
/* 986 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 988 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 990 */	NdrFcShort( 0x0 ),	/* 0 */
/* 992 */	NdrFcShort( 0x0 ),	/* 0 */
/* 994 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter item */

/* 996 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 998 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1000 */	NdrFcShort( 0x78 ),	/* Type Offset=120 */

	/* Return value */

/* 1002 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1004 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1006 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StopPlayback */


	/* Procedure RemoveAtEnd */

/* 1008 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1010 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1014 */	NdrFcShort( 0xe ),	/* 14 */
/* 1016 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1018 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1020 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1022 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1024 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1026 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1028 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1030 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */


	/* Return value */

/* 1032 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1034 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1036 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Clear */

/* 1038 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1040 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1044 */	NdrFcShort( 0xf ),	/* 15 */
/* 1046 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1050 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1052 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1054 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1056 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1058 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1060 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1062 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1064 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1066 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetMany */

/* 1068 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1070 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1074 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1076 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1078 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1080 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1082 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x5,		/* 5 */
/* 1084 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1086 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1088 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1090 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter startIndex */

/* 1092 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1094 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1096 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter capacity */

/* 1098 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1100 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1102 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter items */

/* 1104 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 1106 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1108 */	NdrFcShort( 0xbe ),	/* Type Offset=190 */

	/* Parameter actual */

/* 1110 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1112 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1114 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1116 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1118 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1120 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReplaceAll */

/* 1122 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1124 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1128 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1130 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1132 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1134 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1136 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1138 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1140 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1142 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1144 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter count */

/* 1146 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1148 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter value */

/* 1152 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1154 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1156 */	NdrFcShort( 0xee ),	/* Type Offset=238 */

	/* Return value */

/* 1158 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1160 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1162 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Invoke */

/* 1164 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1166 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1170 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1172 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1174 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1176 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1178 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1180 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1182 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1186 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter asyncInfo */

/* 1188 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1190 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1192 */	NdrFcShort( 0x104 ),	/* Type Offset=260 */

	/* Parameter status */

/* 1194 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1196 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1198 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Return value */

/* 1200 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1202 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1204 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Completed */

/* 1206 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1208 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1212 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1214 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1216 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1218 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1220 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1222 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1224 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1228 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter handler */

/* 1230 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1232 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1234 */	NdrFcShort( 0x116 ),	/* Type Offset=278 */

	/* Return value */

/* 1236 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1238 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1240 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Completed */

/* 1242 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1244 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1248 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1250 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1252 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1254 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1256 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1258 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1264 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter handler */

/* 1266 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1268 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1270 */	NdrFcShort( 0x128 ),	/* Type Offset=296 */

	/* Return value */

/* 1272 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1274 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1276 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetResults */

/* 1278 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1280 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1284 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1286 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1290 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1292 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1294 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1296 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1300 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter results */

/* 1302 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1304 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1306 */	NdrFcShort( 0x12c ),	/* Type Offset=300 */

	/* Return value */

/* 1308 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1310 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Invoke */


	/* Procedure Invoke */

/* 1314 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1316 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1320 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1322 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1326 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1328 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1330 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1334 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1336 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter connection */


	/* Parameter sender */

/* 1338 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1340 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1342 */	NdrFcShort( 0x130 ),	/* Type Offset=304 */

	/* Return value */


	/* Return value */

/* 1344 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1346 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1348 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Invoke */

/* 1350 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1352 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1356 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1358 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1362 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1364 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1366 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1372 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter sender */

/* 1374 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1376 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1378 */	NdrFcShort( 0x130 ),	/* Type Offset=304 */

	/* Parameter args */

/* 1380 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1382 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1384 */	NdrFcShort( 0x142 ),	/* Type Offset=322 */

	/* Return value */

/* 1386 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1388 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1390 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Offset */

/* 1392 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1394 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1398 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1400 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1402 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1404 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1406 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1408 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1410 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1412 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1414 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter offset */

/* 1416 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1418 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1420 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1422 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1424 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1426 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_CurrentLength */

/* 1428 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1430 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1434 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1436 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1438 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1440 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1442 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1444 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1446 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1448 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1450 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter currentLength */

/* 1452 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1454 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1456 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1458 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1460 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1462 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_CurrentLength */

/* 1464 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1466 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1470 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1472 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1474 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1476 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1478 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1480 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1482 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1484 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1486 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter currentLength */

/* 1488 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1490 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1492 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1494 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1496 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1498 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure TrimLeft */

/* 1500 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1502 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1506 */	NdrFcShort( 0xa ),	/* 10 */
/* 1508 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1510 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1512 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1514 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1516 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1518 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1522 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter cbSize */

/* 1524 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1526 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1528 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1530 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1532 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1534 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveBufferByIndex */


	/* Procedure TrimRight */

/* 1536 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1538 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1542 */	NdrFcShort( 0xb ),	/* 11 */
/* 1544 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1546 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1548 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1550 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 1552 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1556 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1558 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */


	/* Parameter cbSize */

/* 1560 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1562 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1564 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dataBuffer */


	/* Parameter dataBuffer */

/* 1566 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1568 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1570 */	NdrFcShort( 0x74 ),	/* Type Offset=116 */

	/* Return value */


	/* Return value */

/* 1572 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1574 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1576 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Reset */


	/* Procedure Reset */

/* 1578 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1580 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1584 */	NdrFcShort( 0xc ),	/* 12 */
/* 1586 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1588 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1590 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1592 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1594 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1600 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */


	/* Return value */

/* 1602 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1604 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1606 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Create */

/* 1608 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1610 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1614 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1616 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1618 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1620 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1622 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1624 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1626 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1628 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1630 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter size */

/* 1632 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1634 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1636 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1638 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1640 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1642 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddBuffer */

/* 1644 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1646 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1650 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1652 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1654 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1656 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1658 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1660 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1662 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1666 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataBuffer */

/* 1668 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1670 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1672 */	NdrFcShort( 0x78 ),	/* Type Offset=120 */

	/* Return value */

/* 1674 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1676 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1678 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure InsertBuffer */

/* 1680 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1682 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1686 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1688 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1690 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1692 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1694 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1696 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1698 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1702 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1704 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1706 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1708 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dataBuffer */

/* 1710 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1712 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1714 */	NdrFcShort( 0x78 ),	/* Type Offset=120 */

	/* Return value */

/* 1716 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1718 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1720 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RemoveBuffer */

/* 1722 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1724 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1728 */	NdrFcShort( 0xa ),	/* 10 */
/* 1730 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1732 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1734 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1736 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1738 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1740 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1744 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataBuffer */

/* 1746 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1748 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1750 */	NdrFcShort( 0x78 ),	/* Type Offset=120 */

	/* Return value */

/* 1752 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1754 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1756 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Create */

/* 1758 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1760 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1764 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1766 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1768 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1770 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1772 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1774 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1780 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataBuffer */

/* 1782 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1784 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1786 */	NdrFcShort( 0x78 ),	/* Type Offset=120 */

	/* Return value */

/* 1788 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1790 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1792 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_StreamType */


	/* Procedure get_MixerMode */


	/* Procedure get_PayloadType */

/* 1794 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1796 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1800 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1802 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1804 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1806 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1808 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1810 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1816 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */


	/* Parameter mixerMode */


	/* Parameter payloadType */

/* 1818 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1820 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1822 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 1824 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1826 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1828 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Connection */

/* 1830 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1832 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1836 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1838 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1842 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1844 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1846 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1848 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1850 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1852 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter connection */

/* 1854 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1856 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1858 */	NdrFcShort( 0x12c ),	/* Type Offset=300 */

	/* Return value */

/* 1860 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1862 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1864 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_DataBundle */

/* 1866 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1868 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1872 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1874 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1876 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1878 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1880 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1882 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1884 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1886 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1888 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataBundle */

/* 1890 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1892 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1894 */	NdrFcShort( 0x158 ),	/* Type Offset=344 */

	/* Return value */

/* 1896 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1898 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1900 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RemoteUri */

/* 1902 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1904 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1908 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1910 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1912 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1914 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1916 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1918 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1922 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1924 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter uri */

/* 1926 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1928 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1930 */	NdrFcShort( 0x16e ),	/* Type Offset=366 */

	/* Return value */

/* 1932 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1934 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1936 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Create */

/* 1938 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1940 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1944 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1946 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 1948 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1950 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1952 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x6,		/* 6 */
/* 1954 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1956 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1958 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1960 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter type */

/* 1962 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1964 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1966 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Parameter connection */

/* 1968 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1970 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1972 */	NdrFcShort( 0x130 ),	/* Type Offset=304 */

	/* Parameter dataBundle */

/* 1974 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1976 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1978 */	NdrFcShort( 0x15c ),	/* Type Offset=348 */

	/* Parameter uri */

/* 1980 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1982 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1984 */	NdrFcShort( 0x172 ),	/* Type Offset=370 */

	/* Parameter args */

/* 1986 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1988 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1990 */	NdrFcShort( 0x184 ),	/* Type Offset=388 */

	/* Return value */

/* 1992 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1994 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 1996 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add_Disconnected */

/* 1998 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2000 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2004 */	NdrFcShort( 0x7 ),	/* 7 */
/* 2006 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2008 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2010 */	NdrFcShort( 0x34 ),	/* 52 */
/* 2012 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2014 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2016 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2018 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2020 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter eventHandler */

/* 2022 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2024 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2026 */	NdrFcShort( 0x188 ),	/* Type Offset=392 */

	/* Parameter token */

/* 2028 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 2030 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2032 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 2034 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2036 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2038 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure remove_Disconnected */

/* 2040 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2042 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2046 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2048 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2050 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2052 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2054 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2056 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2058 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2060 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2062 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter token */

/* 2064 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
/* 2066 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2068 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 2070 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2072 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2074 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add_Received */

/* 2076 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2078 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2082 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2084 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2086 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2088 */	NdrFcShort( 0x34 ),	/* 52 */
/* 2090 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2092 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2094 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2096 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2098 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter eventHandler */

/* 2100 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2102 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2104 */	NdrFcShort( 0x1a4 ),	/* Type Offset=420 */

	/* Parameter token */

/* 2106 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 2108 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2110 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 2112 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2114 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2116 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure remove_Received */

/* 2118 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2120 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2124 */	NdrFcShort( 0xa ),	/* 10 */
/* 2126 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2128 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2130 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2132 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2134 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2138 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2140 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter token */

/* 2142 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
/* 2144 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2146 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 2148 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2150 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2152 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendPayloadType */

/* 2154 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2156 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2160 */	NdrFcShort( 0xb ),	/* 11 */
/* 2162 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2164 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2166 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2168 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2170 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2176 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter type */

/* 2178 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2180 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2182 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Return value */

/* 2184 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2186 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2188 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendBundle */

/* 2190 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2192 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2196 */	NdrFcShort( 0xc ),	/* 12 */
/* 2198 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2202 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2204 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2206 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2212 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataBundle */

/* 2214 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2216 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2218 */	NdrFcShort( 0x1b6 ),	/* Type Offset=438 */

	/* Return value */

/* 2220 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2222 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2224 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendBundleAsync */

/* 2226 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2228 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2232 */	NdrFcShort( 0xd ),	/* 13 */
/* 2234 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2238 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2240 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2242 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2248 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter dataBundle */

/* 2250 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2252 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2254 */	NdrFcShort( 0x1b6 ),	/* Type Offset=438 */

	/* Parameter asyncAction */

/* 2256 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2258 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2260 */	NdrFcShort( 0x1c8 ),	/* Type Offset=456 */

	/* Return value */

/* 2262 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2264 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2266 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CheckClosed */

/* 2268 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2270 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2274 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2276 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2278 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2280 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2282 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2284 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2290 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2292 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2294 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2296 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure WaitForPayload */

/* 2298 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2300 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2304 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2306 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2310 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2312 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2314 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2318 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2320 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2322 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2324 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2326 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Create */

/* 2328 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2330 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2334 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2336 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2338 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2340 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2342 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2344 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2346 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2348 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2350 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter streamSocket */

/* 2352 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2354 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2356 */	NdrFcShort( 0x1de ),	/* Type Offset=478 */

	/* Parameter connection */

/* 2358 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2360 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2362 */	NdrFcShort( 0x1f0 ),	/* Type Offset=496 */

	/* Return value */

/* 2364 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2366 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2368 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add_Closed */


	/* Procedure add_Closed */

/* 2370 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2372 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2376 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2378 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2382 */	NdrFcShort( 0x34 ),	/* 52 */
/* 2384 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2386 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2392 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter eventHandler */


	/* Parameter eventHandler */

/* 2394 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2396 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2398 */	NdrFcShort( 0x206 ),	/* Type Offset=518 */

	/* Parameter token */


	/* Parameter token */

/* 2400 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 2402 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2404 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */


	/* Return value */

/* 2406 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2408 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2410 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure remove_Closed */


	/* Procedure remove_Closed */


	/* Procedure remove_Closed */


	/* Procedure remove_Closed */

/* 2412 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2414 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2418 */	NdrFcShort( 0x7 ),	/* 7 */
/* 2420 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2422 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2424 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2426 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2428 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2430 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2432 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2434 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter token */


	/* Parameter token */


	/* Parameter token */


	/* Parameter token */

/* 2436 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
/* 2438 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2440 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */


	/* Return value */


	/* Return value */


	/* Return value */

/* 2442 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2444 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2446 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ConnectAsync */


	/* Procedure ListenAsync */

/* 2448 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2450 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2454 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2456 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2458 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2460 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2462 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2464 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2468 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2470 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter operation */


	/* Parameter operation */

/* 2472 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2474 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2476 */	NdrFcShort( 0x218 ),	/* Type Offset=536 */

	/* Return value */


	/* Return value */

/* 2478 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2480 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2482 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Create */

/* 2484 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2486 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2490 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2492 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2494 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2496 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2498 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 2500 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2502 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2504 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2506 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter port */

/* 2508 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2510 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2512 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter listener */

/* 2514 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2516 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2518 */	NdrFcShort( 0x22e ),	/* Type Offset=558 */

	/* Return value */

/* 2520 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2522 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2524 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Create */

/* 2526 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2528 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2532 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2534 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2536 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2538 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2540 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 2542 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2544 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2546 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2548 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hostAddress */

/* 2550 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2552 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2554 */	NdrFcShort( 0x25e ),	/* Type Offset=606 */

	/* Parameter port */

/* 2556 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2558 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2560 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter connector */

/* 2562 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2564 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2566 */	NdrFcShort( 0x268 ),	/* Type Offset=616 */

	/* Return value */

/* 2568 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2570 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2572 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Invoke */

/* 2574 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2576 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2580 */	NdrFcShort( 0x3 ),	/* 3 */
/* 2582 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2586 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2588 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2590 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2592 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2596 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter args */

/* 2598 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2600 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2602 */	NdrFcShort( 0x27e ),	/* Type Offset=638 */

	/* Return value */

/* 2604 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2606 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2608 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Invoke */

/* 2610 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2612 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2616 */	NdrFcShort( 0x3 ),	/* 3 */
/* 2618 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2622 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2624 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2626 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2628 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2632 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter args */

/* 2634 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2636 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2638 */	NdrFcShort( 0x290 ),	/* Type Offset=656 */

	/* Return value */

/* 2640 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2642 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2644 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Timestamp */

/* 2646 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2648 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2652 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2654 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2656 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2658 */	NdrFcShort( 0x2c ),	/* 44 */
/* 2660 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2662 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2668 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter timestamp */

/* 2670 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2672 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2674 */	0xb,		/* FC_HYPER */
			0x0,		/* 0 */

	/* Return value */

/* 2676 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2678 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2680 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Transforms */

/* 2682 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2684 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2688 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2690 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2692 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2694 */	NdrFcShort( 0x1ac ),	/* 428 */
/* 2696 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2698 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2704 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter transform */

/* 2706 */	NdrFcShort( 0x112 ),	/* Flags:  must free, out, simple ref, */
/* 2708 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2710 */	NdrFcShort( 0x2fa ),	/* Type Offset=762 */

	/* Return value */

/* 2712 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2714 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2716 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_VideoTexture */

/* 2718 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2720 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2724 */	NdrFcShort( 0xa ),	/* 10 */
/* 2726 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2730 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2732 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2734 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2736 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2740 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppvTexture */

/* 2742 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2744 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2746 */	NdrFcShort( 0x310 ),	/* Type Offset=784 */

	/* Return value */

/* 2748 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2750 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2752 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Connection */

/* 2754 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2756 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2760 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2762 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2766 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2768 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2770 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2772 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2776 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter connection */

/* 2778 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2780 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2782 */	NdrFcShort( 0x1f0 ),	/* Type Offset=496 */

	/* Return value */

/* 2784 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2786 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2788 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Connection */

/* 2790 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2792 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2796 */	NdrFcShort( 0x7 ),	/* 7 */
/* 2798 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2802 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2804 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2806 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2808 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2810 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2812 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter connection */

/* 2814 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2816 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2818 */	NdrFcShort( 0x1f4 ),	/* Type Offset=500 */

	/* Return value */

/* 2820 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2822 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2824 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Create */

/* 2826 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2828 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2832 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2834 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2836 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2838 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2840 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2842 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2846 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2848 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter connection */

/* 2850 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2852 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2854 */	NdrFcShort( 0x1f4 ),	/* Type Offset=500 */

	/* Parameter mediaSource */

/* 2856 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2858 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2860 */	NdrFcShort( 0x326 ),	/* Type Offset=806 */

	/* Return value */

/* 2862 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2864 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2866 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add_Closed */

/* 2868 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2870 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2874 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2876 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2878 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2880 */	NdrFcShort( 0x34 ),	/* 52 */
/* 2882 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2884 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2886 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2888 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2890 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter eventHandler */

/* 2892 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2894 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2896 */	NdrFcShort( 0x33c ),	/* Type Offset=828 */

	/* Parameter token */

/* 2898 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 2900 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2902 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 2904 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2906 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2908 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add_FormatChanged */

/* 2910 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2912 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2916 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2918 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2922 */	NdrFcShort( 0x34 ),	/* 52 */
/* 2924 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2926 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2928 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2930 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2932 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter eventHandler */

/* 2934 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2936 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2938 */	NdrFcShort( 0x34e ),	/* Type Offset=846 */

	/* Parameter token */

/* 2940 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 2942 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2944 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 2946 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2948 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2950 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure remove_Closed */


	/* Procedure remove_FormatChanged */

/* 2952 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2954 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2958 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2960 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2962 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2964 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2966 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2968 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2970 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2974 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter token */


	/* Parameter token */

/* 2976 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
/* 2978 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2980 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */


	/* Return value */

/* 2982 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2984 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2986 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add_SampleUpdated */

/* 2988 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2990 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2994 */	NdrFcShort( 0xa ),	/* 10 */
/* 2996 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 2998 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3000 */	NdrFcShort( 0x34 ),	/* 52 */
/* 3002 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3004 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3006 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3008 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3010 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter eventHandler */

/* 3012 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3014 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3016 */	NdrFcShort( 0x360 ),	/* Type Offset=864 */

	/* Parameter token */

/* 3018 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 3020 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3022 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 3024 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3026 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3028 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure remove_FormatChanged */


	/* Procedure remove_SampleUpdated */

/* 3030 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3032 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3036 */	NdrFcShort( 0xb ),	/* 11 */
/* 3038 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3040 */	NdrFcShort( 0x18 ),	/* 24 */
/* 3042 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3044 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3046 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3050 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3052 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter token */


	/* Parameter token */

/* 3054 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
/* 3056 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3058 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */


	/* Return value */

/* 3060 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3062 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3064 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure InitializeAsync */

/* 3066 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3068 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3072 */	NdrFcShort( 0xc ),	/* 12 */
/* 3074 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3076 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3078 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3080 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 3082 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3084 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3086 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3088 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter asyncAction */

/* 3090 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 3092 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3094 */	NdrFcShort( 0x372 ),	/* Type Offset=882 */

	/* Return value */

/* 3096 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3098 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StartPlayback */

/* 3102 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3104 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3108 */	NdrFcShort( 0xd ),	/* 13 */
/* 3110 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3114 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3116 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3118 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3124 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3126 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3128 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Create */

/* 3132 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3134 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3138 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3140 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3142 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3144 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3146 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3148 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3154 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter connection */

/* 3156 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3158 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3160 */	NdrFcShort( 0x388 ),	/* Type Offset=904 */

	/* Parameter playbackEngine */

/* 3162 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 3164 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3166 */	NdrFcShort( 0x39a ),	/* Type Offset=922 */

	/* Return value */

/* 3168 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3170 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_SpatialCoordinateSystem */

/* 3174 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3176 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3180 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3182 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3186 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3188 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 3190 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3196 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter coordinateSystem */

/* 3198 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 3200 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3202 */	NdrFcShort( 0x3b0 ),	/* Type Offset=944 */

	/* Return value */

/* 3204 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3206 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_SpatialCoordinateSystem */

/* 3210 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3212 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3216 */	NdrFcShort( 0x7 ),	/* 7 */
/* 3218 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3222 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3224 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 3226 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3228 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3230 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3232 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter coordinateSystem */

/* 3234 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3236 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3238 */	NdrFcShort( 0x3b4 ),	/* Type Offset=948 */

	/* Return value */

/* 3240 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3242 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3244 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add_Closed */

/* 3246 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3248 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3252 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3254 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3256 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3258 */	NdrFcShort( 0x34 ),	/* 52 */
/* 3260 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3262 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3264 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3266 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3268 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter eventHandler */

/* 3270 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3272 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3274 */	NdrFcShort( 0x3c6 ),	/* Type Offset=966 */

	/* Parameter token */

/* 3276 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 3278 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3280 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 3282 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3284 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add_FormatChanged */

/* 3288 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3290 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3294 */	NdrFcShort( 0xa ),	/* 10 */
/* 3296 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3300 */	NdrFcShort( 0x34 ),	/* 52 */
/* 3302 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3304 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3310 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter eventHandler */

/* 3312 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3314 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3316 */	NdrFcShort( 0x3d8 ),	/* Type Offset=984 */

	/* Parameter token */

/* 3318 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 3320 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3322 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 3324 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3326 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3328 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add_SampleUpdated */

/* 3330 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3332 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3336 */	NdrFcShort( 0xc ),	/* 12 */
/* 3338 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3340 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3342 */	NdrFcShort( 0x34 ),	/* 52 */
/* 3344 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3346 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3348 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3352 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter eventhandler */

/* 3354 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3356 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3358 */	NdrFcShort( 0x3ea ),	/* Type Offset=1002 */

	/* Parameter token */

/* 3360 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 3362 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3364 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 3366 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3368 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure remove_SampleUpdated */

/* 3372 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3374 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3378 */	NdrFcShort( 0xd ),	/* 13 */
/* 3380 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3382 */	NdrFcShort( 0x18 ),	/* 24 */
/* 3384 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3386 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3388 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3394 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter token */

/* 3396 */	NdrFcShort( 0x8a ),	/* Flags:  must free, in, by val, */
/* 3398 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3400 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 3402 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3404 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3406 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnEndOfStream */

/* 3408 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3410 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3414 */	NdrFcShort( 0xe ),	/* 14 */
/* 3416 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3418 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3420 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3422 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3424 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3426 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3430 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter streamId */

/* 3432 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3434 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3436 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3438 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3440 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure HandleError */

/* 3444 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3446 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3450 */	NdrFcShort( 0xf ),	/* 15 */
/* 3452 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3454 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3456 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3458 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3460 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3466 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hr */

/* 3468 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3470 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3472 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3474 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3476 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3478 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CheckShutdown */

/* 3480 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3482 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3486 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3488 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3490 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3492 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3494 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3496 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3500 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3502 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3504 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3506 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3508 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendDescription */

/* 3510 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3512 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3516 */	NdrFcShort( 0x11 ),	/* 17 */
/* 3518 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3520 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3522 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3524 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3526 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3528 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3530 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3532 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3534 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3536 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3538 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetMediaStreamProperties */

/* 3540 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3542 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3546 */	NdrFcShort( 0x12 ),	/* 18 */
/* 3548 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3550 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3552 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3554 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3556 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3562 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter MediaStreamType */

/* 3564 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3566 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3568 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Parameter mediaEncodingProperties */

/* 3570 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 3572 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3574 */	NdrFcShort( 0x3fc ),	/* Type Offset=1020 */

	/* Return value */

/* 3576 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3578 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3580 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_StreamType */


	/* Procedure put_MixerMode */

/* 3582 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3584 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3588 */	NdrFcShort( 0x7 ),	/* 7 */
/* 3590 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3592 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3594 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3596 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3598 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3600 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3602 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3604 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */


	/* Parameter mixerMode */

/* 3606 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3608 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3610 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 3612 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3614 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3616 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_HologramComposition */

/* 3618 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3620 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3624 */	NdrFcShort( 0x9 ),	/* 9 */
/* 3626 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3628 */	NdrFcShort( 0x5 ),	/* 5 */
/* 3630 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3632 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3634 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3636 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3638 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3640 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 3642 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3644 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3646 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 3648 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3650 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3652 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_VideoStabilization */

/* 3654 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3656 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3660 */	NdrFcShort( 0xa ),	/* 10 */
/* 3662 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3664 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3666 */	NdrFcShort( 0x21 ),	/* 33 */
/* 3668 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3670 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3676 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 3678 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3680 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3682 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 3684 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3686 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3688 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_VideoStabilization */

/* 3690 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3692 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3696 */	NdrFcShort( 0xb ),	/* 11 */
/* 3698 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3700 */	NdrFcShort( 0x5 ),	/* 5 */
/* 3702 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3704 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3706 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3708 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3710 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3712 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 3714 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3716 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3718 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 3720 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3722 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3724 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_VideoStabilizationBufferLength */

/* 3726 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3728 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3732 */	NdrFcShort( 0xc ),	/* 12 */
/* 3734 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3736 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3738 */	NdrFcShort( 0x24 ),	/* 36 */
/* 3740 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3742 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3744 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3748 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 3750 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3752 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3754 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3756 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3758 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3760 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_VideoStabilizationBufferLength */

/* 3762 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3764 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3768 */	NdrFcShort( 0xd ),	/* 13 */
/* 3770 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3772 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3774 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3776 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3778 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3780 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3782 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3784 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 3786 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3788 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3790 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3792 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3794 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3796 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_VideoStabilizationMaximumBufferLength */

/* 3798 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3800 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3804 */	NdrFcShort( 0xe ),	/* 14 */
/* 3806 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3808 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3810 */	NdrFcShort( 0x24 ),	/* 36 */
/* 3812 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3814 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3816 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3818 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3820 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 3822 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3824 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3826 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3828 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3830 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3832 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_GlobalOpacityCoefficient */

/* 3834 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3836 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3840 */	NdrFcShort( 0xf ),	/* 15 */
/* 3842 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3844 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3846 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3848 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3850 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3852 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3854 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3856 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 3858 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3860 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3862 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 3864 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3866 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3868 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_GlobalOpacityCoefficient */

/* 3870 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3872 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3876 */	NdrFcShort( 0x10 ),	/* 16 */
/* 3878 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3880 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3882 */	NdrFcShort( 0x24 ),	/* 36 */
/* 3884 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3886 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3888 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3890 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3892 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 3894 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3896 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3898 */	0xa,		/* FC_FLOAT */
			0x0,		/* 0 */

	/* Return value */

/* 3900 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3902 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3904 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_RecordingIndicatorEnabled */

/* 3906 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3908 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3912 */	NdrFcShort( 0x11 ),	/* 17 */
/* 3914 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3916 */	NdrFcShort( 0x5 ),	/* 5 */
/* 3918 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3920 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3922 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3924 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3926 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3928 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 3930 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3932 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3934 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 3936 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3938 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3940 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_RecordingIndicatorEnabled */

/* 3942 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3944 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3948 */	NdrFcShort( 0x12 ),	/* 18 */
/* 3950 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3952 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3954 */	NdrFcShort( 0x21 ),	/* 33 */
/* 3956 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3958 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3960 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3962 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3964 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter value */

/* 3966 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3968 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3970 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Return value */

/* 3972 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3974 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3976 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure add_Closed */

/* 3978 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3980 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3984 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3986 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3988 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3990 */	NdrFcShort( 0x34 ),	/* 52 */
/* 3992 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 3994 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3996 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3998 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4000 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter eventHandler */

/* 4002 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 4004 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4006 */	NdrFcShort( 0x412 ),	/* Type Offset=1042 */

	/* Parameter token */

/* 4008 */	NdrFcShort( 0x2112 ),	/* Flags:  must free, out, simple ref, srv alloc size=8 */
/* 4010 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4012 */	NdrFcShort( 0x19e ),	/* Type Offset=414 */

	/* Return value */

/* 4014 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4016 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4018 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_SpatialCoordinateSystem */

/* 4020 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4022 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4026 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4028 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4030 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4032 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4034 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 4036 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4038 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4040 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4042 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter coordinateSystem */

/* 4044 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 4046 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4048 */	NdrFcShort( 0x424 ),	/* Type Offset=1060 */

	/* Return value */

/* 4050 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4052 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4054 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_SpatialCoordinateSystem */

/* 4056 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4058 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4062 */	NdrFcShort( 0x9 ),	/* 9 */
/* 4064 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4066 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4068 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4070 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 4072 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4074 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4076 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4078 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter coordinateSystem */

/* 4080 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 4082 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4084 */	NdrFcShort( 0x428 ),	/* Type Offset=1064 */

	/* Return value */

/* 4086 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4088 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4090 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure InitAsync */

/* 4092 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4094 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4098 */	NdrFcShort( 0xa ),	/* 10 */
/* 4100 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4102 */	NdrFcShort( 0x5 ),	/* 5 */
/* 4104 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4106 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 4108 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4110 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4114 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enableAudio */

/* 4116 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4118 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4120 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Parameter action */

/* 4122 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 4124 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4126 */	NdrFcShort( 0x43a ),	/* Type Offset=1082 */

	/* Return value */

/* 4128 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4130 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4132 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StartAsync */

/* 4134 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4136 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4140 */	NdrFcShort( 0xb ),	/* 11 */
/* 4142 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 4144 */	NdrFcShort( 0x5 ),	/* 5 */
/* 4146 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4148 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 4150 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4156 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enableMrc */

/* 4158 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4160 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4162 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Parameter connection */

/* 4164 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 4166 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4168 */	NdrFcShort( 0x450 ),	/* Type Offset=1104 */

	/* Parameter action */

/* 4170 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 4172 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4174 */	NdrFcShort( 0x43a ),	/* Type Offset=1082 */

	/* Return value */

/* 4176 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4178 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4180 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StopAsync */

/* 4182 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4184 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4188 */	NdrFcShort( 0xc ),	/* 12 */
/* 4190 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4194 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4196 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 4198 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4202 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4204 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter action */

/* 4206 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 4208 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4210 */	NdrFcShort( 0x43a ),	/* Type Offset=1082 */

	/* Return value */

/* 4212 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4214 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4216 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Invoke */

/* 4218 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4220 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4224 */	NdrFcShort( 0x3 ),	/* 3 */
/* 4226 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4228 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4230 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4232 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 4234 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4240 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter asyncInfo */

/* 4242 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 4244 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4246 */	NdrFcShort( 0x462 ),	/* Type Offset=1122 */

	/* Parameter status */

/* 4248 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4250 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4252 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Return value */

/* 4254 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4256 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Completed */

/* 4260 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4262 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4266 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4268 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4272 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4274 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 4276 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4278 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4280 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4282 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter handler */

/* 4284 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 4286 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4288 */	NdrFcShort( 0x474 ),	/* Type Offset=1140 */

	/* Return value */

/* 4290 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4292 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4294 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Completed */

/* 4296 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4298 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4302 */	NdrFcShort( 0x7 ),	/* 7 */
/* 4304 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4308 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4310 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 4312 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4318 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter handler */

/* 4320 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 4322 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4324 */	NdrFcShort( 0x486 ),	/* Type Offset=1158 */

	/* Return value */

/* 4326 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4328 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4330 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetResults */

/* 4332 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4334 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4338 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4340 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4344 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4346 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 4348 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4354 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter results */

/* 4356 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 4358 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4360 */	NdrFcShort( 0x48a ),	/* Type Offset=1162 */

	/* Return value */

/* 4362 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4364 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4366 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateAsync */

/* 4368 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 4370 */	NdrFcLong( 0x0 ),	/* 0 */
/* 4374 */	NdrFcShort( 0x6 ),	/* 6 */
/* 4376 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 4378 */	NdrFcShort( 0x5 ),	/* 5 */
/* 4380 */	NdrFcShort( 0x8 ),	/* 8 */
/* 4382 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 4384 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 4386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 4390 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter enableAudio */

/* 4392 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 4394 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 4396 */	0x3,		/* FC_SMALL */
			0x0,		/* 0 */

	/* Parameter createAsynOp */

/* 4398 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 4400 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 4402 */	NdrFcShort( 0x4a0 ),	/* Type Offset=1184 */

	/* Return value */

/* 4404 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 4406 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 4408 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MixedRemoteViewCompositor_MIDL_TYPE_FORMAT_STRING MixedRemoteViewCompositor__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/*  4 */	NdrFcLong( 0xaf86e2e0 ),	/* -1350114592 */
/*  8 */	NdrFcShort( 0xb12d ),	/* -20179 */
/* 10 */	NdrFcShort( 0x4c6a ),	/* 19562 */
/* 12 */	0x9c,		/* 156 */
			0x5a,		/* 90 */
/* 14 */	0xd7,		/* 215 */
			0xaa,		/* 170 */
/* 16 */	0x65,		/* 101 */
			0x10,		/* 16 */
/* 18 */	0x1e,		/* 30 */
			0x90,		/* 144 */
/* 20 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 22 */	0x3,		/* FC_SMALL */
			0x5c,		/* FC_PAD */
/* 24 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 26 */	NdrFcLong( 0x2edeb3fb ),	/* 786347003 */
/* 30 */	NdrFcShort( 0x8008 ),	/* -32760 */
/* 32 */	NdrFcShort( 0x4a13 ),	/* 18963 */
/* 34 */	0xb8,		/* 184 */
			0xd3,		/* 211 */
/* 36 */	0x70,		/* 112 */
			0x81,		/* 129 */
/* 38 */	0xfd,		/* 253 */
			0x11,		/* 17 */
/* 40 */	0x44,		/* 68 */
			0x3f,		/* 63 */
/* 42 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 44 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 46 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 48 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (24) */
/* 50 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 52 */	NdrFcShort( 0x2 ),	/* Offset= 2 (54) */
/* 54 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 56 */	NdrFcLong( 0xee26ebc8 ),	/* -299439160 */
/* 60 */	NdrFcShort( 0x2eba ),	/* 11962 */
/* 62 */	NdrFcShort( 0x4476 ),	/* 17526 */
/* 64 */	0xbe,		/* 190 */
			0x2a,		/* 42 */
/* 66 */	0x4b,		/* 75 */
			0xf9,		/* 249 */
/* 68 */	0x38,		/* 56 */
			0x95,		/* 149 */
/* 70 */	0x90,		/* 144 */
			0xbc,		/* 188 */
/* 72 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 74 */	NdrFcShort( 0x2 ),	/* Offset= 2 (76) */
/* 76 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 78 */	NdrFcLong( 0x712c3f26 ),	/* 1898725158 */
/* 82 */	NdrFcShort( 0xea19 ),	/* -5607 */
/* 84 */	NdrFcShort( 0x44a2 ),	/* 17570 */
/* 86 */	0xb9,		/* 185 */
			0x47,		/* 71 */
/* 88 */	0x3,		/* 3 */
			0x13,		/* 19 */
/* 90 */	0xc7,		/* 199 */
			0xd4,		/* 212 */
/* 92 */	0x1a,		/* 26 */
			0x2b,		/* 43 */
/* 94 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 96 */	NdrFcShort( 0x2 ),	/* Offset= 2 (98) */
/* 98 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 100 */	NdrFcLong( 0xc9630182 ),	/* -916258430 */
/* 104 */	NdrFcShort( 0xdd7a ),	/* -8838 */
/* 106 */	NdrFcShort( 0x4fe8 ),	/* 20456 */
/* 108 */	0xb7,		/* 183 */
			0x97,		/* 151 */
/* 110 */	0x14,		/* 20 */
			0xaa,		/* 170 */
/* 112 */	0x4d,		/* 77 */
			0xaf,		/* 175 */
/* 114 */	0x29,		/* 41 */
			0x7a,		/* 122 */
/* 116 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 118 */	NdrFcShort( 0x2 ),	/* Offset= 2 (120) */
/* 120 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 122 */	NdrFcLong( 0x2fffa1dc ),	/* 805282268 */
/* 126 */	NdrFcShort( 0xaa9b ),	/* -21861 */
/* 128 */	NdrFcShort( 0x4f0f ),	/* 20239 */
/* 130 */	0xa6,		/* 166 */
			0x7c,		/* 124 */
/* 132 */	0x41,		/* 65 */
			0x4d,		/* 77 */
/* 134 */	0xa4,		/* 164 */
			0x76,		/* 118 */
/* 136 */	0x13,		/* 19 */
			0xb8,		/* 184 */
/* 138 */	
			0x11, 0x0,	/* FC_RP */
/* 140 */	NdrFcShort( 0x2 ),	/* Offset= 2 (142) */
/* 142 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 144 */	NdrFcShort( 0x0 ),	/* 0 */
/* 146 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 148 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 150 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 152 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x54,		/* FC_DEREFERENCE */
/* 154 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 156 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 158 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 160 */	NdrFcShort( 0xffd8 ),	/* Offset= -40 (120) */
/* 162 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 164 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 166 */	NdrFcShort( 0x2 ),	/* Offset= 2 (168) */
/* 168 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 170 */	NdrFcLong( 0x7e9b41f5 ),	/* 2124104181 */
/* 174 */	NdrFcShort( 0x30a7 ),	/* 12455 */
/* 176 */	NdrFcShort( 0x5d4d ),	/* 23885 */
/* 178 */	0x8e,		/* 142 */
			0x8f,		/* 143 */
/* 180 */	0x93,		/* 147 */
			0xfa,		/* 250 */
/* 182 */	0xdf,		/* 223 */
			0x3,		/* 3 */
/* 184 */	0x38,		/* 56 */
			0x72,		/* 114 */
/* 186 */	
			0x11, 0x0,	/* FC_RP */
/* 188 */	NdrFcShort( 0x2 ),	/* Offset= 2 (190) */
/* 190 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 194 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 196 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 198 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 200 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x54,		/* FC_DEREFERENCE */
/* 202 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 204 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 206 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 208 */	NdrFcShort( 0xffa8 ),	/* Offset= -88 (120) */
/* 210 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 212 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 214 */	NdrFcShort( 0x2 ),	/* Offset= 2 (216) */
/* 216 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 218 */	NdrFcLong( 0xfb647465 ),	/* -77302683 */
/* 222 */	NdrFcShort( 0x6cc2 ),	/* 27842 */
/* 224 */	NdrFcShort( 0x5259 ),	/* 21081 */
/* 226 */	0x8a,		/* 138 */
			0xf,		/* 15 */
/* 228 */	0x9d,		/* 157 */
			0xba,		/* 186 */
/* 230 */	0xcd,		/* 205 */
			0x87,		/* 135 */
/* 232 */	0x8c,		/* 140 */
			0x7e,		/* 126 */
/* 234 */	
			0x11, 0x0,	/* FC_RP */
/* 236 */	NdrFcShort( 0x2 ),	/* Offset= 2 (238) */
/* 238 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 240 */	NdrFcShort( 0x0 ),	/* 0 */
/* 242 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x0,		/*  */
/* 244 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 246 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 248 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 252 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 254 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 256 */	NdrFcShort( 0xff78 ),	/* Offset= -136 (120) */
/* 258 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 260 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 262 */	NdrFcLong( 0x66fae8a0 ),	/* 1727719584 */
/* 266 */	NdrFcShort( 0xb7d7 ),	/* -18473 */
/* 268 */	NdrFcShort( 0x5d93 ),	/* 23955 */
/* 270 */	0xb8,		/* 184 */
			0x33,		/* 51 */
/* 272 */	0x6d,		/* 109 */
			0x4b,		/* 75 */
/* 274 */	0x4b,		/* 75 */
			0xe2,		/* 226 */
/* 276 */	0x9d,		/* 157 */
			0xf6,		/* 246 */
/* 278 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 280 */	NdrFcLong( 0x52bbae84 ),	/* 1388031620 */
/* 284 */	NdrFcShort( 0xbb36 ),	/* -17610 */
/* 286 */	NdrFcShort( 0x50ff ),	/* 20735 */
/* 288 */	0xb9,		/* 185 */
			0xde,		/* 222 */
/* 290 */	0x11,		/* 17 */
			0xed,		/* 237 */
/* 292 */	0x4a,		/* 74 */
			0xca,		/* 202 */
/* 294 */	0x36,		/* 54 */
			0x55,		/* 85 */
/* 296 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 298 */	NdrFcShort( 0xffec ),	/* Offset= -20 (278) */
/* 300 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 302 */	NdrFcShort( 0x2 ),	/* Offset= 2 (304) */
/* 304 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 306 */	NdrFcLong( 0x27d0a6d ),	/* 41749101 */
/* 310 */	NdrFcShort( 0x3ffb ),	/* 16379 */
/* 312 */	NdrFcShort( 0x480e ),	/* 18446 */
/* 314 */	0x9e,		/* 158 */
			0x68,		/* 104 */
/* 316 */	0x25,		/* 37 */
			0x5c,		/* 92 */
/* 318 */	0xba,		/* 186 */
			0x26,		/* 38 */
/* 320 */	0x4d,		/* 77 */
			0x40,		/* 64 */
/* 322 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 324 */	NdrFcLong( 0xc0d9ad8e ),	/* -1059476082 */
/* 328 */	NdrFcShort( 0xf586 ),	/* -2682 */
/* 330 */	NdrFcShort( 0x4314 ),	/* 17172 */
/* 332 */	0xaf,		/* 175 */
			0x4d,		/* 77 */
/* 334 */	0x69,		/* 105 */
			0xb8,		/* 184 */
/* 336 */	0x5,		/* 5 */
			0x13,		/* 19 */
/* 338 */	0xb4,		/* 180 */
			0x70,		/* 112 */
/* 340 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 342 */	0xe,		/* FC_ENUM32 */
			0x5c,		/* FC_PAD */
/* 344 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 346 */	NdrFcShort( 0x2 ),	/* Offset= 2 (348) */
/* 348 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 350 */	NdrFcLong( 0x49cd02e ),	/* 77385774 */
/* 354 */	NdrFcShort( 0x12ec ),	/* 4844 */
/* 356 */	NdrFcShort( 0x45dc ),	/* 17884 */
/* 358 */	0x8f,		/* 143 */
			0x58,		/* 88 */
/* 360 */	0x93,		/* 147 */
			0x1c,		/* 28 */
/* 362 */	0x34,		/* 52 */
			0x20,		/* 32 */
/* 364 */	0x87,		/* 135 */
			0xed,		/* 237 */
/* 366 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 368 */	NdrFcShort( 0x2 ),	/* Offset= 2 (370) */
/* 370 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 372 */	NdrFcLong( 0x9e365e57 ),	/* -1640604073 */
/* 376 */	NdrFcShort( 0x48b2 ),	/* 18610 */
/* 378 */	NdrFcShort( 0x4160 ),	/* 16736 */
/* 380 */	0x95,		/* 149 */
			0x6f,		/* 111 */
/* 382 */	0xc7,		/* 199 */
			0x38,		/* 56 */
/* 384 */	0x51,		/* 81 */
			0x20,		/* 32 */
/* 386 */	0xbb,		/* 187 */
			0xfc,		/* 252 */
/* 388 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 390 */	NdrFcShort( 0xffbc ),	/* Offset= -68 (322) */
/* 392 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 394 */	NdrFcLong( 0x7f6533cb ),	/* 2137338827 */
/* 398 */	NdrFcShort( 0x58a5 ),	/* 22693 */
/* 400 */	NdrFcShort( 0x4634 ),	/* 17972 */
/* 402 */	0xa0,		/* 160 */
			0xca,		/* 202 */
/* 404 */	0x1a,		/* 26 */
			0xf1,		/* 241 */
/* 406 */	0x2e,		/* 46 */
			0x82,		/* 130 */
/* 408 */	0xf5,		/* 245 */
			0x8,		/* 8 */
/* 410 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 412 */	NdrFcShort( 0x2 ),	/* Offset= 2 (414) */
/* 414 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 416 */	NdrFcShort( 0x8 ),	/* 8 */
/* 418 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 420 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 422 */	NdrFcLong( 0x351aade9 ),	/* 890940905 */
/* 426 */	NdrFcShort( 0x5b42 ),	/* 23362 */
/* 428 */	NdrFcShort( 0x4ae8 ),	/* 19176 */
/* 430 */	0x9d,		/* 157 */
			0x72,		/* 114 */
/* 432 */	0xdd,		/* 221 */
			0x5b,		/* 91 */
/* 434 */	0x76,		/* 118 */
			0x5d,		/* 93 */
/* 436 */	0x34,		/* 52 */
			0x20,		/* 32 */
/* 438 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 440 */	NdrFcLong( 0x49cd02e ),	/* 77385774 */
/* 444 */	NdrFcShort( 0x12ec ),	/* 4844 */
/* 446 */	NdrFcShort( 0x45dc ),	/* 17884 */
/* 448 */	0x8f,		/* 143 */
			0x58,		/* 88 */
/* 450 */	0x93,		/* 147 */
			0x1c,		/* 28 */
/* 452 */	0x34,		/* 52 */
			0x20,		/* 32 */
/* 454 */	0x87,		/* 135 */
			0xed,		/* 237 */
/* 456 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 458 */	NdrFcShort( 0x2 ),	/* Offset= 2 (460) */
/* 460 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 462 */	NdrFcLong( 0x5a648006 ),	/* 1516535814 */
/* 466 */	NdrFcShort( 0x843a ),	/* -31686 */
/* 468 */	NdrFcShort( 0x4da9 ),	/* 19881 */
/* 470 */	0x86,		/* 134 */
			0x5b,		/* 91 */
/* 472 */	0x9d,		/* 157 */
			0x26,		/* 38 */
/* 474 */	0xe5,		/* 229 */
			0xdf,		/* 223 */
/* 476 */	0xad,		/* 173 */
			0x7b,		/* 123 */
/* 478 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 480 */	NdrFcLong( 0x69a22cf3 ),	/* 1772236019 */
/* 484 */	NdrFcShort( 0xfc7b ),	/* -901 */
/* 486 */	NdrFcShort( 0x4857 ),	/* 18519 */
/* 488 */	0xaf,		/* 175 */
			0x38,		/* 56 */
/* 490 */	0xf6,		/* 246 */
			0xe7,		/* 231 */
/* 492 */	0xde,		/* 222 */
			0x6a,		/* 106 */
/* 494 */	0x5b,		/* 91 */
			0x49,		/* 73 */
/* 496 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 498 */	NdrFcShort( 0x2 ),	/* Offset= 2 (500) */
/* 500 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 502 */	NdrFcLong( 0x27d0a6d ),	/* 41749101 */
/* 506 */	NdrFcShort( 0x3ffb ),	/* 16379 */
/* 508 */	NdrFcShort( 0x480e ),	/* 18446 */
/* 510 */	0x9e,		/* 158 */
			0x68,		/* 104 */
/* 512 */	0x25,		/* 37 */
			0x5c,		/* 92 */
/* 514 */	0xba,		/* 186 */
			0x26,		/* 38 */
/* 516 */	0x4d,		/* 77 */
			0x40,		/* 64 */
/* 518 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 520 */	NdrFcLong( 0x742c3239 ),	/* 1949053497 */
/* 524 */	NdrFcShort( 0x1cf3 ),	/* 7411 */
/* 526 */	NdrFcShort( 0x40bb ),	/* 16571 */
/* 528 */	0x98,		/* 152 */
			0xc8,		/* 200 */
/* 530 */	0x73,		/* 115 */
			0x27,		/* 39 */
/* 532 */	0xe,		/* 14 */
			0xe,		/* 14 */
/* 534 */	0x33,		/* 51 */
			0x7f,		/* 127 */
/* 536 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 538 */	NdrFcShort( 0x2 ),	/* Offset= 2 (540) */
/* 540 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 542 */	NdrFcLong( 0x66fae8a0 ),	/* 1727719584 */
/* 546 */	NdrFcShort( 0xb7d7 ),	/* -18473 */
/* 548 */	NdrFcShort( 0x5d93 ),	/* 23955 */
/* 550 */	0xb8,		/* 184 */
			0x33,		/* 51 */
/* 552 */	0x6d,		/* 109 */
			0x4b,		/* 75 */
/* 554 */	0x4b,		/* 75 */
			0xe2,		/* 226 */
/* 556 */	0x9d,		/* 157 */
			0xf6,		/* 246 */
/* 558 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 560 */	NdrFcShort( 0x2 ),	/* Offset= 2 (562) */
/* 562 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 564 */	NdrFcLong( 0x887397c7 ),	/* -2005690425 */
/* 568 */	NdrFcShort( 0xce0f ),	/* -12785 */
/* 570 */	NdrFcShort( 0x4ced ),	/* 19693 */
/* 572 */	0x86,		/* 134 */
			0xf1,		/* 241 */
/* 574 */	0x54,		/* 84 */
			0xf4,		/* 244 */
/* 576 */	0xee,		/* 238 */
			0x7,		/* 7 */
/* 578 */	0x9d,		/* 157 */
			0xb,		/* 11 */
/* 580 */	
			0x12, 0x0,	/* FC_UP */
/* 582 */	NdrFcShort( 0xe ),	/* Offset= 14 (596) */
/* 584 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 586 */	NdrFcShort( 0x2 ),	/* 2 */
/* 588 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 590 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 592 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 594 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 596 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 598 */	NdrFcShort( 0x8 ),	/* 8 */
/* 600 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (584) */
/* 602 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 604 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 606 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 610 */	NdrFcShort( 0x4 ),	/* 4 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0xffde ),	/* Offset= -34 (580) */
/* 616 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 618 */	NdrFcShort( 0x2 ),	/* Offset= 2 (620) */
/* 620 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 622 */	NdrFcLong( 0x4d3288f4 ),	/* 1295157492 */
/* 626 */	NdrFcShort( 0xb509 ),	/* -19191 */
/* 628 */	NdrFcShort( 0x4ffe ),	/* 20478 */
/* 630 */	0x84,		/* 132 */
			0x8d,		/* 141 */
/* 632 */	0x20,		/* 32 */
			0x2,		/* 2 */
/* 634 */	0x18,		/* 24 */
			0x2,		/* 2 */
/* 636 */	0x90,		/* 144 */
			0x96,		/* 150 */
/* 638 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 640 */	NdrFcLong( 0xaa65736c ),	/* -1436191892 */
/* 644 */	NdrFcShort( 0x5842 ),	/* 22594 */
/* 646 */	NdrFcShort( 0x4510 ),	/* 17680 */
/* 648 */	0x94,		/* 148 */
			0x90,		/* 144 */
/* 650 */	0xfa,		/* 250 */
			0x43,		/* 67 */
/* 652 */	0xdc,		/* 220 */
			0x9f,		/* 159 */
/* 654 */	0x37,		/* 55 */
			0xd4,		/* 212 */
/* 656 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 658 */	NdrFcLong( 0xc8968fde ),	/* -929656866 */
/* 662 */	NdrFcShort( 0x7644 ),	/* 30276 */
/* 664 */	NdrFcShort( 0x497c ),	/* 18812 */
/* 666 */	0xb0,		/* 176 */
			0x3c,		/* 60 */
/* 668 */	0x1c,		/* 28 */
			0x11,		/* 17 */
/* 670 */	0xca,		/* 202 */
			0x62,		/* 98 */
/* 672 */	0x6d,		/* 109 */
			0xd3,		/* 211 */
/* 674 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 676 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 678 */	
			0x11, 0x0,	/* FC_RP */
/* 680 */	NdrFcShort( 0x52 ),	/* Offset= 82 (762) */
/* 682 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 684 */	NdrFcShort( 0x40 ),	/* 64 */
/* 686 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 688 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 690 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 692 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 694 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 696 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 698 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 700 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 702 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 704 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 706 */	NdrFcShort( 0x8 ),	/* 8 */
/* 708 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 710 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 712 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 714 */	NdrFcShort( 0x10 ),	/* 16 */
/* 716 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 718 */	NdrFcShort( 0xfff2 ),	/* Offset= -14 (704) */
/* 720 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 722 */	NdrFcShort( 0xffee ),	/* Offset= -18 (704) */
/* 724 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 726 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 728 */	NdrFcShort( 0x14 ),	/* 20 */
/* 730 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 732 */	0xa,		/* FC_FLOAT */
			0xa,		/* FC_FLOAT */
/* 734 */	0xa,		/* FC_FLOAT */
			0x5b,		/* FC_END */
/* 736 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 738 */	NdrFcShort( 0x2c ),	/* 44 */
/* 740 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 742 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 744 */	NdrFcShort( 0xffe0 ),	/* Offset= -32 (712) */
/* 746 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 748 */	NdrFcShort( 0xffea ),	/* Offset= -22 (726) */
/* 750 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 752 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 754 */	NdrFcShort( 0x30 ),	/* 48 */
/* 756 */	0x8,		/* FC_LONG */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 758 */	0x0,		/* 0 */
			NdrFcShort( 0xffe9 ),	/* Offset= -23 (736) */
			0x5b,		/* FC_END */
/* 762 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 764 */	NdrFcShort( 0xf0 ),	/* 240 */
/* 766 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 768 */	NdrFcShort( 0xffaa ),	/* Offset= -86 (682) */
/* 770 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 772 */	NdrFcShort( 0xffa6 ),	/* Offset= -90 (682) */
/* 774 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 776 */	NdrFcShort( 0xffa2 ),	/* Offset= -94 (682) */
/* 778 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 780 */	NdrFcShort( 0xffe4 ),	/* Offset= -28 (752) */
/* 782 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 784 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 786 */	NdrFcShort( 0x2 ),	/* Offset= 2 (788) */
/* 788 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 790 */	NdrFcLong( 0xaf86e2e0 ),	/* -1350114592 */
/* 794 */	NdrFcShort( 0xb12d ),	/* -20179 */
/* 796 */	NdrFcShort( 0x4c6a ),	/* 19562 */
/* 798 */	0x9c,		/* 156 */
			0x5a,		/* 90 */
/* 800 */	0xd7,		/* 215 */
			0xaa,		/* 170 */
/* 802 */	0x65,		/* 101 */
			0x10,		/* 16 */
/* 804 */	0x1e,		/* 30 */
			0x90,		/* 144 */
/* 806 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 808 */	NdrFcShort( 0x2 ),	/* Offset= 2 (810) */
/* 810 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 812 */	NdrFcLong( 0xe7bfb599 ),	/* -406866535 */
/* 816 */	NdrFcShort( 0xa09d ),	/* -24419 */
/* 818 */	NdrFcShort( 0x4c21 ),	/* 19489 */
/* 820 */	0xbc,		/* 188 */
			0xdf,		/* 223 */
/* 822 */	0x20,		/* 32 */
			0xaf,		/* 175 */
/* 824 */	0x4f,		/* 79 */
			0x86,		/* 134 */
/* 826 */	0xb3,		/* 179 */
			0xd9,		/* 217 */
/* 828 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 830 */	NdrFcLong( 0x742c3239 ),	/* 1949053497 */
/* 834 */	NdrFcShort( 0x1cf3 ),	/* 7411 */
/* 836 */	NdrFcShort( 0x40bb ),	/* 16571 */
/* 838 */	0x98,		/* 152 */
			0xc8,		/* 200 */
/* 840 */	0x73,		/* 115 */
			0x27,		/* 39 */
/* 842 */	0xe,		/* 14 */
			0xe,		/* 14 */
/* 844 */	0x33,		/* 51 */
			0x7f,		/* 127 */
/* 846 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 848 */	NdrFcLong( 0x7b2c1549 ),	/* 2066486601 */
/* 852 */	NdrFcShort( 0xc998 ),	/* -13928 */
/* 854 */	NdrFcShort( 0x4d37 ),	/* 19767 */
/* 856 */	0xa3,		/* 163 */
			0x79,		/* 121 */
/* 858 */	0xe0,		/* 224 */
			0xb5,		/* 181 */
/* 860 */	0x12,		/* 18 */
			0x51,		/* 81 */
/* 862 */	0xa5,		/* 165 */
			0x2c,		/* 44 */
/* 864 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 866 */	NdrFcLong( 0x1003b11c ),	/* 268677404 */
/* 870 */	NdrFcShort( 0xdebe ),	/* -8514 */
/* 872 */	NdrFcShort( 0x48a6 ),	/* 18598 */
/* 874 */	0xb1,		/* 177 */
			0xba,		/* 186 */
/* 876 */	0xf,		/* 15 */
			0xd0,		/* 208 */
/* 878 */	0xea,		/* 234 */
			0x5d,		/* 93 */
/* 880 */	0x6,		/* 6 */
			0xe4,		/* 228 */
/* 882 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 884 */	NdrFcShort( 0x2 ),	/* Offset= 2 (886) */
/* 886 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 888 */	NdrFcLong( 0x5a648006 ),	/* 1516535814 */
/* 892 */	NdrFcShort( 0x843a ),	/* -31686 */
/* 894 */	NdrFcShort( 0x4da9 ),	/* 19881 */
/* 896 */	0x86,		/* 134 */
			0x5b,		/* 91 */
/* 898 */	0x9d,		/* 157 */
			0x26,		/* 38 */
/* 900 */	0xe5,		/* 229 */
			0xdf,		/* 223 */
/* 902 */	0xad,		/* 173 */
			0x7b,		/* 123 */
/* 904 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 906 */	NdrFcLong( 0x27d0a6d ),	/* 41749101 */
/* 910 */	NdrFcShort( 0x3ffb ),	/* 16379 */
/* 912 */	NdrFcShort( 0x480e ),	/* 18446 */
/* 914 */	0x9e,		/* 158 */
			0x68,		/* 104 */
/* 916 */	0x25,		/* 37 */
			0x5c,		/* 92 */
/* 918 */	0xba,		/* 186 */
			0x26,		/* 38 */
/* 920 */	0x4d,		/* 77 */
			0x40,		/* 64 */
/* 922 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 924 */	NdrFcShort( 0x2 ),	/* Offset= 2 (926) */
/* 926 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 928 */	NdrFcLong( 0xb55bf4f3 ),	/* -1252264717 */
/* 932 */	NdrFcShort( 0x2883 ),	/* 10371 */
/* 934 */	NdrFcShort( 0x4c09 ),	/* 19465 */
/* 936 */	0xa2,		/* 162 */
			0x31,		/* 49 */
/* 938 */	0x7e,		/* 126 */
			0x75,		/* 117 */
/* 940 */	0xbd,		/* 189 */
			0x59,		/* 89 */
/* 942 */	0xbf,		/* 191 */
			0x9e,		/* 158 */
/* 944 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 946 */	NdrFcShort( 0x2 ),	/* Offset= 2 (948) */
/* 948 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 950 */	NdrFcLong( 0x69ebca4b ),	/* 1777060427 */
/* 954 */	NdrFcShort( 0x60a3 ),	/* 24739 */
/* 956 */	NdrFcShort( 0x3586 ),	/* 13702 */
/* 958 */	0xa6,		/* 166 */
			0x53,		/* 83 */
/* 960 */	0x59,		/* 89 */
			0xa7,		/* 167 */
/* 962 */	0xbd,		/* 189 */
			0x67,		/* 103 */
/* 964 */	0x6d,		/* 109 */
			0x7,		/* 7 */
/* 966 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 968 */	NdrFcLong( 0x742c3239 ),	/* 1949053497 */
/* 972 */	NdrFcShort( 0x1cf3 ),	/* 7411 */
/* 974 */	NdrFcShort( 0x40bb ),	/* 16571 */
/* 976 */	0x98,		/* 152 */
			0xc8,		/* 200 */
/* 978 */	0x73,		/* 115 */
			0x27,		/* 39 */
/* 980 */	0xe,		/* 14 */
			0xe,		/* 14 */
/* 982 */	0x33,		/* 51 */
			0x7f,		/* 127 */
/* 984 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 986 */	NdrFcLong( 0x7b2c1549 ),	/* 2066486601 */
/* 990 */	NdrFcShort( 0xc998 ),	/* -13928 */
/* 992 */	NdrFcShort( 0x4d37 ),	/* 19767 */
/* 994 */	0xa3,		/* 163 */
			0x79,		/* 121 */
/* 996 */	0xe0,		/* 224 */
			0xb5,		/* 181 */
/* 998 */	0x12,		/* 18 */
			0x51,		/* 81 */
/* 1000 */	0xa5,		/* 165 */
			0x2c,		/* 44 */
/* 1002 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1004 */	NdrFcLong( 0x1003b11c ),	/* 268677404 */
/* 1008 */	NdrFcShort( 0xdebe ),	/* -8514 */
/* 1010 */	NdrFcShort( 0x48a6 ),	/* 18598 */
/* 1012 */	0xb1,		/* 177 */
			0xba,		/* 186 */
/* 1014 */	0xf,		/* 15 */
			0xd0,		/* 208 */
/* 1016 */	0xea,		/* 234 */
			0x5d,		/* 93 */
/* 1018 */	0x6,		/* 6 */
			0xe4,		/* 228 */
/* 1020 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1022 */	NdrFcLong( 0xb4002af6 ),	/* -1275057418 */
/* 1026 */	NdrFcShort( 0xacd4 ),	/* -21292 */
/* 1028 */	NdrFcShort( 0x4e5a ),	/* 20058 */
/* 1030 */	0xa2,		/* 162 */
			0x4b,		/* 75 */
/* 1032 */	0x5d,		/* 93 */
			0x74,		/* 116 */
/* 1034 */	0x98,		/* 152 */
			0xa8,		/* 168 */
/* 1036 */	0xb8,		/* 184 */
			0xc4,		/* 196 */
/* 1038 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1040 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1042 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1044 */	NdrFcLong( 0x742c3239 ),	/* 1949053497 */
/* 1048 */	NdrFcShort( 0x1cf3 ),	/* 7411 */
/* 1050 */	NdrFcShort( 0x40bb ),	/* 16571 */
/* 1052 */	0x98,		/* 152 */
			0xc8,		/* 200 */
/* 1054 */	0x73,		/* 115 */
			0x27,		/* 39 */
/* 1056 */	0xe,		/* 14 */
			0xe,		/* 14 */
/* 1058 */	0x33,		/* 51 */
			0x7f,		/* 127 */
/* 1060 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1062 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1064) */
/* 1064 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1066 */	NdrFcLong( 0x69ebca4b ),	/* 1777060427 */
/* 1070 */	NdrFcShort( 0x60a3 ),	/* 24739 */
/* 1072 */	NdrFcShort( 0x3586 ),	/* 13702 */
/* 1074 */	0xa6,		/* 166 */
			0x53,		/* 83 */
/* 1076 */	0x59,		/* 89 */
			0xa7,		/* 167 */
/* 1078 */	0xbd,		/* 189 */
			0x67,		/* 103 */
/* 1080 */	0x6d,		/* 109 */
			0x7,		/* 7 */
/* 1082 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1084 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1086) */
/* 1086 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1088 */	NdrFcLong( 0x5a648006 ),	/* 1516535814 */
/* 1092 */	NdrFcShort( 0x843a ),	/* -31686 */
/* 1094 */	NdrFcShort( 0x4da9 ),	/* 19881 */
/* 1096 */	0x86,		/* 134 */
			0x5b,		/* 91 */
/* 1098 */	0x9d,		/* 157 */
			0x26,		/* 38 */
/* 1100 */	0xe5,		/* 229 */
			0xdf,		/* 223 */
/* 1102 */	0xad,		/* 173 */
			0x7b,		/* 123 */
/* 1104 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1106 */	NdrFcLong( 0x27d0a6d ),	/* 41749101 */
/* 1110 */	NdrFcShort( 0x3ffb ),	/* 16379 */
/* 1112 */	NdrFcShort( 0x480e ),	/* 18446 */
/* 1114 */	0x9e,		/* 158 */
			0x68,		/* 104 */
/* 1116 */	0x25,		/* 37 */
			0x5c,		/* 92 */
/* 1118 */	0xba,		/* 186 */
			0x26,		/* 38 */
/* 1120 */	0x4d,		/* 77 */
			0x40,		/* 64 */
/* 1122 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1124 */	NdrFcLong( 0xd0bb5d7e ),	/* -793027202 */
/* 1128 */	NdrFcShort( 0xacff ),	/* -21249 */
/* 1130 */	NdrFcShort( 0x54f8 ),	/* 21752 */
/* 1132 */	0xb8,		/* 184 */
			0x28,		/* 40 */
/* 1134 */	0xaf,		/* 175 */
			0x4c,		/* 76 */
/* 1136 */	0xbc,		/* 188 */
			0xbf,		/* 191 */
/* 1138 */	0x64,		/* 100 */
			0x13,		/* 19 */
/* 1140 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1142 */	NdrFcLong( 0x9358bb05 ),	/* -1822901499 */
/* 1146 */	NdrFcShort( 0xe2b4 ),	/* -7500 */
/* 1148 */	NdrFcShort( 0x5bc5 ),	/* 23493 */
/* 1150 */	0x81,		/* 129 */
			0x8a,		/* 138 */
/* 1152 */	0x33,		/* 51 */
			0xac,		/* 172 */
/* 1154 */	0x45,		/* 69 */
			0x19,		/* 25 */
/* 1156 */	0xf6,		/* 246 */
			0xb7,		/* 183 */
/* 1158 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1160 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1140) */
/* 1162 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1164 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1166) */
/* 1166 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1168 */	NdrFcLong( 0x5e9858a9 ),	/* 1587042473 */
/* 1172 */	NdrFcShort( 0x5ef5 ),	/* 24309 */
/* 1174 */	NdrFcShort( 0x4b84 ),	/* 19332 */
/* 1176 */	0x96,		/* 150 */
			0x45,		/* 69 */
/* 1178 */	0xeb,		/* 235 */
			0x3c,		/* 60 */
/* 1180 */	0x11,		/* 17 */
			0x61,		/* 97 */
/* 1182 */	0xa,		/* 10 */
			0x17,		/* 23 */
/* 1184 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1186 */	NdrFcShort( 0xffc0 ),	/* Offset= -64 (1122) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            HSTRING_UserSize
            ,HSTRING_UserMarshal
            ,HSTRING_UserUnmarshal
            ,HSTRING_UserFree
            }

        };



/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3291, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0001, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3292, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0002, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3293, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0003, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3294, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0004, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3295, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0005, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3296, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0006, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3297, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0007, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3298, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0008, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3299, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0009, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3300, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0010, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler, ver. 0.0,
   GUID={0x742c3239,0x1cf3,0x40bb,{0x98,0xc8,0x73,0x27,0x0e,0x0e,0x33,0x7f}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler_FormatStringOffsetTable[] =
    {
    0
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandlerProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler::Invoke */
};

const CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandlerStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0011, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IInspectable, ver. 0.0,
   GUID={0xAF86E2E0,0xB12D,0x4c6a,{0x9C,0x5A,0xD7,0xAA,0x65,0x10,0x1E,0x90}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule, ver. 0.0,
   GUID={0x2edeb3fb,0x8008,0x4a13,{0xb8,0xd3,0x70,0x81,0xfd,0x11,0x44,0x3f}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    36,
    72
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(8) ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule::get_IsInitialized */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule::Uninitialize */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule_ServerInfo,
    8,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0012, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager, ver. 0.0,
   GUID={0xee26ebc8,0x2eba,0x4476,{0xbe,0x2a,0x4b,0xf9,0x38,0x95,0x90,0xbc}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    102,
    144,
    186,
    222
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager::AddModule */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager::GetModule */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager::ReleaseModule */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager::Uninitialize */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager_ServerInfo,
    10,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0013, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics, ver. 0.0,
   GUID={0x5feb052c,0xd722,0x4f2a,{0x8b,0xf5,0x7d,0x03,0xba,0x80,0x87,0x94}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    252
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStaticsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics::get_InvalidModuleHandle */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStaticsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics_ServerInfo,
    7,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0014, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager, ver. 0.0,
   GUID={0xc9630182,0xdd7a,0x4fe8,{0xb7,0x97,0x14,0xaa,0x4d,0xaf,0x29,0x7a}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    288,
    324
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(8) ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager::get_ModuleManager */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager::get_DirectXManager */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager_ServerInfo,
    8,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0015, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics, ver. 0.0,
   GUID={0x172d17ad,0x464c,0x4da4,{0xad,0x26,0x34,0xd2,0xa7,0x0d,0x4f,0x07}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    360
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStaticsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics::get_Instance */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStaticsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics_ServerInfo,
    7,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0016, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager, ver. 0.0,
   GUID={0x712c3f26,0xea19,0x44a2,{0xb9,0x47,0x03,0x13,0xc7,0xd4,0x1a,0x2b}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(6) ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManagerProxyVtbl = 
{
    0,
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManagerStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager_ServerInfo,
    6,
    &__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3301, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0018, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer, ver. 0.0,
   GUID={0x7e9b41f5,0x30a7,0x5d4d,{0x8e,0x8f,0x93,0xfa,0xdf,0x03,0x38,0x72}} */

#pragma code_seg(".orpc")
static const unsigned short __FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    396,
    432,
    468,
    504
    };

static const MIDL_STUBLESS_PROXY_INFO __FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) ___FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBufferProxyVtbl = 
{
    &__FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo,
    &IID___FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::get_Current */ ,
    (void *) (INT_PTR) -1 /* __FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::get_HasCurrent */ ,
    (void *) (INT_PTR) -1 /* __FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::MoveNext */ ,
    (void *) (INT_PTR) -1 /* __FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::GetMany */
};


static const PRPC_STUB_FUNCTION __FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBufferStubVtbl =
{
    &IID___FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer,
    &__FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo,
    10,
    &__FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0019, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3302, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0020, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer, ver. 0.0,
   GUID={0x0c2a67f0,0x2c4d,0x52a4,{0xaf,0xad,0xa3,0xf0,0x8f,0x9f,0x34,0x42}} */

#pragma code_seg(".orpc")
static const unsigned short __FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    552
    };

static const MIDL_STUBLESS_PROXY_INFO __FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBufferProxyVtbl = 
{
    &__FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo,
    &IID___FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::First */
};


static const PRPC_STUB_FUNCTION __FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBufferStubVtbl =
{
    &IID___FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer,
    &__FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo,
    7,
    &__FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0021, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3303, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0022, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer, ver. 0.0,
   GUID={0xfb647465,0x6cc2,0x5259,{0x8a,0x0f,0x9d,0xba,0xcd,0x87,0x8c,0x7e}} */

#pragma code_seg(".orpc")
static const unsigned short __FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    588,
    630,
    666,
    714
    };

static const MIDL_STUBLESS_PROXY_INFO __FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) ___FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBufferProxyVtbl = 
{
    &__FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo,
    &IID___FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::GetAt */ ,
    (void *) (INT_PTR) -1 /* __FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::get_Size */ ,
    (void *) (INT_PTR) -1 /* __FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::IndexOf */ ,
    (void *) (INT_PTR) -1 /* __FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::GetMany */
};


static const PRPC_STUB_FUNCTION __FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBufferStubVtbl =
{
    &IID___FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer,
    &__FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo,
    10,
    &__FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0023, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3304, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0024, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer, ver. 0.0,
   GUID={0xf1aac163,0x3e3d,0x5594,{0x98,0xf3,0xff,0xef,0xde,0x4a,0x94,0x30}} */

#pragma code_seg(".orpc")
static const unsigned short __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    588,
    630,
    768,
    804,
    852,
    894,
    936,
    972,
    1008,
    1038,
    1068,
    1122
    };

static const MIDL_STUBLESS_PROXY_INFO __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(18) ___FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBufferProxyVtbl = 
{
    &__FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ProxyInfo,
    &IID___FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::GetAt */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::get_Size */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::GetView */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::IndexOf */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::SetAt */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::InsertAt */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::RemoveAt */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::Append */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::RemoveAtEnd */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::Clear */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::GetMany */ ,
    (void *) (INT_PTR) -1 /* __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer::ReplaceAll */
};


static const PRPC_STUB_FUNCTION __FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBufferStubVtbl =
{
    &IID___FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer,
    &__FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_ServerInfo,
    18,
    &__FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0025, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3305, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0026, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection, ver. 0.0,
   GUID={0x52bbae84,0xbb36,0x50ff,{0xb9,0xde,0x11,0xed,0x4a,0xca,0x36,0x55}} */

#pragma code_seg(".orpc")
static const unsigned short __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection_FormatStringOffsetTable[] =
    {
    1164
    };

static const MIDL_STUBLESS_PROXY_INFO __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) ___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnectionProxyVtbl = 
{
    &__FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection_ProxyInfo,
    &IID___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection::Invoke */
};

const CInterfaceStubVtbl ___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnectionStubVtbl =
{
    &IID___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection,
    &__FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0027, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3306, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0028, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection, ver. 0.0,
   GUID={0x66fae8a0,0xb7d7,0x5d93,{0xb8,0x33,0x6d,0x4b,0x4b,0xe2,0x9d,0xf6}} */

#pragma code_seg(".orpc")
static const unsigned short __FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1206,
    1242,
    1278
    };

static const MIDL_STUBLESS_PROXY_INFO __FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(9) ___FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnectionProxyVtbl = 
{
    &__FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection_ProxyInfo,
    &IID___FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection::put_Completed */ ,
    (void *) (INT_PTR) -1 /* __FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection::get_Completed */ ,
    (void *) (INT_PTR) -1 /* __FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection::GetResults */
};


static const PRPC_STUB_FUNCTION __FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnectionStubVtbl =
{
    &IID___FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection,
    &__FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection_ServerInfo,
    9,
    &__FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0029, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler, ver. 0.0,
   GUID={0x7f6533cb,0x58a5,0x4634,{0xa0,0xca,0x1a,0xf1,0x2e,0x82,0xf5,0x08}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler_FormatStringOffsetTable[] =
    {
    1314
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandlerProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler::Invoke */
};

const CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandlerStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler, ver. 0.0,
   GUID={0x351aade9,0x5b42,0x4ae8,{0x9d,0x72,0xdd,0x5b,0x76,0x5d,0x34,0x20}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler_FormatStringOffsetTable[] =
    {
    1350
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandlerProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler::Invoke */
};

const CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandlerStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler, ver. 0.0,
   GUID={0x6f17b4ae,0xf506,0x4d6e,{0xa6,0x66,0x89,0x36,0x38,0x58,0x1b,0x19}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler_FormatStringOffsetTable[] =
    {
    1314
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandlerProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler::Invoke */
};

const CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandlerStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0032, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket, ver. 0.0,
   GUID={0x3fcabce1,0xd85d,0x4990,{0x93,0xcc,0x64,0xb2,0x6b,0x8b,0x95,0x05}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(6) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacketProxyVtbl = 
{
    0,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacketStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket_ServerInfo,
    6,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0033, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer, ver. 0.0,
   GUID={0x2fffa1dc,0xaa9b,0x4f0f,{0xa6,0x7c,0x41,0x4d,0xa4,0x76,0x13,0xb8}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    252,
    1392,
    1428,
    1464,
    1500,
    1536,
    1578
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer::get_Offset */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer::put_Offset */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer::get_CurrentLength */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer::put_CurrentLength */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer::TrimLeft */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer::TrimRight */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer::Reset */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer_ServerInfo,
    13,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0034, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics, ver. 0.0,
   GUID={0xeb453df3,0xe098,0x4fcb,{0xb7,0x6c,0xef,0x38,0x38,0x58,0x30,0x33}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1608
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStaticsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics::Create */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStaticsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics_ServerInfo,
    7,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0035, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock, ver. 0.0,
   GUID={0x4375ee9d,0x0302,0x416b,{0xaf,0x1a,0x2b,0x8f,0xf6,0x32,0xa0,0x6f}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(6) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLockProxyVtbl = 
{
    0,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLockStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock_ServerInfo,
    6,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0036, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle, ver. 0.0,
   GUID={0x049cd02e,0x12ec,0x45dc,{0x8f,0x58,0x93,0x1c,0x34,0x20,0x87,0xed}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    252,
    630,
    1644,
    1680,
    1722,
    1536,
    1578
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle::get_BufferCount */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle::get_TotalSize */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle::AddBuffer */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle::InsertBuffer */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle::RemoveBuffer */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle::RemoveBufferByIndex */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle::Reset */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle_ServerInfo,
    13,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0037, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics, ver. 0.0,
   GUID={0xbac3c09b,0x76ca,0x44d9,{0xad,0x40,0x08,0x0f,0x31,0x01,0x88,0x4a}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1758
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStaticsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics::Create */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStaticsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics_ServerInfo,
    7,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0038, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs, ver. 0.0,
   GUID={0xc0d9ad8e,0xf586,0x4314,{0xaf,0x4d,0x69,0xb8,0x05,0x13,0xb4,0x70}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1794,
    1830,
    1866,
    1902
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs::get_PayloadType */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs::get_Connection */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs::get_DataBundle */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs::get_RemoteUri */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs_ServerInfo,
    10,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0039, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics, ver. 0.0,
   GUID={0x3fa67b47,0x6b91,0x42ad,{0x91,0x2c,0xdd,0xd9,0x7c,0xe0,0x2d,0x50}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1938
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStaticsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics::Create */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStaticsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics_ServerInfo,
    7,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0040, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection, ver. 0.0,
   GUID={0x027d0a6d,0x3ffb,0x480e,{0x9e,0x68,0x25,0x5c,0xba,0x26,0x4d,0x40}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    36,
    1998,
    2040,
    2076,
    2118,
    2154,
    2190,
    2226
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(14) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection::get_IsConnected */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection::add_Disconnected */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection::remove_Disconnected */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection::add_Received */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection::remove_Received */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection::SendPayloadType */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection::SendBundle */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection::SendBundleAsync */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection_ServerInfo,
    14,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0041, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal, ver. 0.0,
   GUID={0x7e41360e,0x7307,0x4ee3,{0x8f,0x5d,0xb6,0x44,0xe7,0xe5,0xc5,0x5a}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2268,
    72,
    2298,
    222
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternalProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal::CheckClosed */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal::WaitForHeader */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal::WaitForPayload */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal::ResetBundle */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternalStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal_ServerInfo,
    10,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0042, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics, ver. 0.0,
   GUID={0xb3c719f9,0xb1c6,0x425a,{0x9b,0x21,0x8a,0x36,0xea,0xac,0x7b,0xe4}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2328
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStaticsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics::Create */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStaticsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics_ServerInfo,
    7,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0043, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener, ver. 0.0,
   GUID={0x887397c7,0xce0f,0x4ced,{0x86,0xf1,0x54,0xf4,0xee,0x07,0x9d,0x0b}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2370,
    2412,
    2448
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(9) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener::add_Closed */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener::remove_Closed */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener::ListenAsync */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener_ServerInfo,
    9,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0044, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics, ver. 0.0,
   GUID={0x0cfcc430,0x07f5,0x49c1,{0xb8,0x6a,0x12,0x3d,0xa1,0x70,0x30,0xb5}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2484
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStaticsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics::Create */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStaticsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics_ServerInfo,
    7,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0045, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector, ver. 0.0,
   GUID={0x4d3288f4,0xb509,0x4ffe,{0x84,0x8d,0x20,0x02,0x18,0x02,0x90,0x96}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2370,
    2412,
    2448
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(9) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector::add_Closed */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector::remove_Closed */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector::ConnectAsync */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector_ServerInfo,
    9,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0046, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics, ver. 0.0,
   GUID={0x5af2e9f2,0x5052,0x46f5,{0xa1,0x18,0x85,0xd2,0x81,0xb1,0x13,0x14}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2526
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStaticsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics::Create */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStaticsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics_ServerInfo,
    7,
    &__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0047, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler, ver. 0.0,
   GUID={0x7b2c1549,0xc998,0x4d37,{0xa3,0x79,0xe0,0xb5,0x12,0x51,0xa5,0x2c}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler_FormatStringOffsetTable[] =
    {
    2574
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandlerProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler::Invoke */
};

const CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandlerStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0048, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs, ver. 0.0,
   GUID={0xaa65736c,0x5842,0x4510,{0x94,0x90,0xfa,0x43,0xdc,0x9f,0x37,0xd4}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    252,
    630
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(8) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs::get_Width */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs::get_Height */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs_ServerInfo,
    8,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0049, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler, ver. 0.0,
   GUID={0x1003b11c,0xdebe,0x48a6,{0xb1,0xba,0x0f,0xd0,0xea,0x5d,0x06,0xe4}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler_FormatStringOffsetTable[] =
    {
    2610
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandlerProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler::Invoke */
};

const CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandlerStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0050, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs, ver. 0.0,
   GUID={0xc8968fde,0x7644,0x497c,{0xb0,0x3c,0x1c,0x11,0xca,0x62,0x6d,0xd3}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    252,
    630,
    2646,
    2682,
    2718
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(11) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs::get_Width */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs::get_Height */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs::get_Timestamp */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs::get_Transforms */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs::get_VideoTexture */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs_ServerInfo,
    11,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0051, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler, ver. 0.0,
   GUID={0x5a4a9427,0x1f66,0x40c6,{0xad,0x6a,0xb3,0xdf,0x73,0x37,0x01,0x12}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2754,
    2790
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(8) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandlerProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler::get_Connection */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler::put_Connection */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandlerStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler_ServerInfo,
    8,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0052, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream, ver. 0.0,
   GUID={0x43c188c7,0x387b,0x45a9,{0xae,0x18,0x54,0x7a,0x4a,0x0e,0xf4,0x8e}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(6) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStreamProxyVtbl = 
{
    0,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStreamStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream_ServerInfo,
    6,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0053, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource, ver. 0.0,
   GUID={0xe63951ce,0xa61e,0x494b,{0x80,0x10,0x33,0x84,0x7c,0x6d,0xa0,0x0b}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(6) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceProxyVtbl = 
{
    0,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource_ServerInfo,
    6,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0054, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics, ver. 0.0,
   GUID={0xd4238dfb,0x9849,0x485b,{0xab,0xca,0x22,0x6d,0x75,0x30,0xaa,0xed}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2826
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStaticsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics::Create */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStaticsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics_ServerInfo,
    7,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0055, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine, ver. 0.0,
   GUID={0xb55bf4f3,0x2883,0x4c09,{0xa2,0x31,0x7e,0x75,0xbd,0x59,0xbf,0x9e}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2868,
    2412,
    2910,
    2952,
    2988,
    3030,
    3066,
    3102,
    1008
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(15) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine::add_Closed */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine::remove_Closed */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine::add_FormatChanged */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine::remove_FormatChanged */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine::add_SampleUpdated */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine::remove_SampleUpdated */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine::InitializeAsync */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine::StartPlayback */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine::StopPlayback */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine_ServerInfo,
    15,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0056, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics, ver. 0.0,
   GUID={0x22708031,0x4143,0x44e9,{0x98,0xaa,0xca,0x3d,0x9b,0x83,0x6f,0x6a}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    3132
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStaticsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics::Create */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStaticsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics_ServerInfo,
    7,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0057, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink, ver. 0.0,
   GUID={0xc9fff32c,0x31f3,0x452f,{0xb1,0x26,0x3c,0x5c,0x6d,0xa5,0x72,0x06}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    3174,
    3210,
    3246,
    2952,
    3288,
    3030,
    3330,
    3372,
    3408,
    3444,
    3480,
    3510,
    3540
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(19) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSinkProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::get_SpatialCoordinateSystem */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::put_SpatialCoordinateSystem */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::add_Closed */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::remove_Closed */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::add_FormatChanged */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::remove_FormatChanged */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::add_SampleUpdated */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::remove_SampleUpdated */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::OnEndOfStream */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::HandleError */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::CheckShutdown */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::SendDescription */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink::SetMediaStreamProperties */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSinkStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink_ServerInfo,
    19,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0058, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition, ver. 0.0,
   GUID={0xced68fe5,0x28d7,0x4d99,{0xb8,0x37,0xb7,0xb3,0x80,0x3e,0x2b,0x23}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1794,
    3582
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(8) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinitionProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition::get_MixerMode */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition::put_MixerMode */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinitionStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition_ServerInfo,
    8,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0059, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition, ver. 0.0,
   GUID={0x8d37f003,0xfbf7,0x4b94,{0x85,0x30,0xbe,0x34,0xe2,0x6c,0x94,0x3a}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1794,
    3582,
    468,
    3618,
    3654,
    3690,
    3726,
    3762,
    3798,
    3834,
    3870,
    3906,
    3942
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(19) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinitionProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::get_StreamType */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::put_StreamType */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::get_HologramComposition */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::put_HologramComposition */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::get_VideoStabilization */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::put_VideoStabilization */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::get_VideoStabilizationBufferLength */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::put_VideoStabilizationBufferLength */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::get_VideoStabilizationMaximumBufferLength */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::put_GlobalOpacityCoefficient */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::get_GlobalOpacityCoefficient */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::put_RecordingIndicatorEnabled */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition::get_RecordingIndicatorEnabled */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinitionStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition_ServerInfo,
    19,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0060, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine, ver. 0.0,
   GUID={0x5e9858a9,0x5ef5,0x4b84,{0x96,0x45,0xeb,0x3c,0x11,0x61,0x0a,0x17}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    3978,
    2412,
    4020,
    4056,
    4092,
    4134,
    4182
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine::add_Closed */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine::remove_Closed */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine::get_SpatialCoordinateSystem */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine::put_SpatialCoordinateSystem */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine::InitAsync */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine::StartAsync */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine::StopAsync */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine_ServerInfo,
    13,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3307, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0062, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine, ver. 0.0,
   GUID={0x9358bb05,0xe2b4,0x5bc5,{0x81,0x8a,0x33,0xac,0x45,0x19,0xf6,0xb7}} */

#pragma code_seg(".orpc")
static const unsigned short __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_FormatStringOffsetTable[] =
    {
    4218
    };

static const MIDL_STUBLESS_PROXY_INFO __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) ___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngineProxyVtbl = 
{
    &__FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ProxyInfo,
    &IID___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* __FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine::Invoke */
};

const CInterfaceStubVtbl ___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngineStubVtbl =
{
    &IID___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine,
    &__FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0063, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor2Eidl_0000_3308, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0064, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine, ver. 0.0,
   GUID={0xd0bb5d7e,0xacff,0x54f8,{0xb8,0x28,0xaf,0x4c,0xbc,0xbf,0x64,0x13}} */

#pragma code_seg(".orpc")
static const unsigned short __FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    4260,
    4296,
    4332
    };

static const MIDL_STUBLESS_PROXY_INFO __FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(9) ___FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngineProxyVtbl = 
{
    &__FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ProxyInfo,
    &IID___FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine::put_Completed */ ,
    (void *) (INT_PTR) -1 /* __FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine::get_Completed */ ,
    (void *) (INT_PTR) -1 /* __FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine::GetResults */
};


static const PRPC_STUB_FUNCTION __FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl ___FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngineStubVtbl =
{
    &IID___FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine,
    &__FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_ServerInfo,
    9,
    &__FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0065, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics, ver. 0.0,
   GUID={0xda706722,0x7bf4,0x4527,{0xad,0x4c,0x22,0x22,0xcb,0x81,0xa7,0xab}} */

#pragma code_seg(".orpc")
static const unsigned short __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    4368
    };

static const MIDL_STUBLESS_PROXY_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics_ProxyInfo =
    {
    &Object_StubDesc,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MixedRemoteViewCompositor__MIDL_ProcFormatString.Format,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) ___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStaticsProxyVtbl = 
{
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics_ProxyInfo,
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IInspectable::GetIids */ ,
    0 /* IInspectable::GetRuntimeClassName */ ,
    0 /* IInspectable::GetTrustLevel */ ,
    (void *) (INT_PTR) -1 /* __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics::CreateAsync */
};


static const PRPC_STUB_FUNCTION __x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl ___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStaticsStubVtbl =
{
    &IID___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics_ServerInfo,
    7,
    &__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_MixedRemoteViewCompositor_0000_0066, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    MixedRemoteViewCompositor__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x801026c, /* MIDL Version 8.1.620 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _MixedRemoteViewCompositor_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinitionProxyVtbl,
    ( CInterfaceProxyVtbl *) &___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngineProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternalProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandlerProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStaticsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManagerProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandlerProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStaticsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSinkProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStaticsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStaticsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandlerProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStaticsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandlerProxyVtbl,
    ( CInterfaceProxyVtbl *) &___FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBufferProxyVtbl,
    ( CInterfaceProxyVtbl *) &___FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBufferProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionProxyVtbl,
    ( CInterfaceProxyVtbl *) &___FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngineProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerProxyVtbl,
    ( CInterfaceProxyVtbl *) &___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnectionProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStaticsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLockProxyVtbl,
    ( CInterfaceProxyVtbl *) &___FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnectionProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStaticsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandlerProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStreamProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandlerProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacketProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinitionProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandlerProxyVtbl,
    ( CInterfaceProxyVtbl *) &___FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBufferProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStaticsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStaticsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorProxyVtbl,
    ( CInterfaceProxyVtbl *) &___FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBufferProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStaticsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStaticsProxyVtbl,
    ( CInterfaceProxyVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _MixedRemoteViewCompositor_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinitionStubVtbl,
    ( CInterfaceStubVtbl *) &___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngineStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternalStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandlerStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStaticsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManagerStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandlerStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStaticsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSinkStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStaticsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStaticsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandlerStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStaticsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandlerStubVtbl,
    ( CInterfaceStubVtbl *) &___FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBufferStubVtbl,
    ( CInterfaceStubVtbl *) &___FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBufferStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStubVtbl,
    ( CInterfaceStubVtbl *) &___FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngineStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStubVtbl,
    ( CInterfaceStubVtbl *) &___FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnectionStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStaticsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLockStubVtbl,
    ( CInterfaceStubVtbl *) &___FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnectionStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStaticsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandlerStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStreamStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandlerStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacketStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinitionStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandlerStubVtbl,
    ( CInterfaceStubVtbl *) &___FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBufferStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStaticsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStaticsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStubVtbl,
    ( CInterfaceStubVtbl *) &___FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBufferStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStaticsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStaticsStubVtbl,
    ( CInterfaceStubVtbl *) &___x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleStubVtbl,
    0
};

PCInterfaceName const _MixedRemoteViewCompositor_InterfaceNamesList[] = 
{
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcVideoEffectDefinition",
    "__FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionInternal",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventHandler",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngineStatics",
    "__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIDirectXManager",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrvcSchemeHandler",
    "__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManagerStatics",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSink",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundle",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListenerStatics",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngineStatics",
    "__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIClosedEventHandler",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgsStatics",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventHandler",
    "__FIVector_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer",
    "__FIVectorView_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CIFormatChangedEventArgs",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnection",
    "__FIAsyncOperation_1_MixedRemoteViewCompositor__CMedia__CCaptureEngine",
    "__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManager",
    "__FIAsyncOperationCompletedHandler_1_MixedRemoteViewCompositor__CNetwork__CConnection",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedArgs",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBundleStatics",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferLock",
    "__FIAsyncOperation_1_MixedRemoteViewCompositor__CNetwork__CConnection",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CICaptureEngine",
    "__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIPluginManagerStatics",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectedEventHandler",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStream",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIListener",
    "__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModuleManager",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDisconnectedEventHandler",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSource",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBuffer",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CISampleUpdatedEventArgs",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataPacket",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CIMrcAudioEffectDefinition",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIBundleReceivedEventHandler",
    "__FIIterable_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectorStatics",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIDataBufferStatics",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CIPlaybackEngine",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnector",
    "__FIIterator_1_MixedRemoteViewCompositor__CNetwork__CDataBuffer",
    "__x_ABI_CMixedRemoteViewCompositor_CNetwork_CIConnectionStatics",
    "__x_ABI_CMixedRemoteViewCompositor_CMedia_CINetworkMediaSourceStatics",
    "__x_ABI_CMixedRemoteViewCompositor_CPlugin_CIModule",
    0
};

const IID *  const _MixedRemoteViewCompositor_BaseIIDList[] = 
{
    &IID_IInspectable,
    0,
    &IID_IInspectable,
    0,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    0,
    &IID_IInspectable,
    0,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    0,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    0,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    0,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    0,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    &IID_IInspectable,
    0
};


#define _MixedRemoteViewCompositor_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _MixedRemoteViewCompositor, pIID, n)

int __stdcall _MixedRemoteViewCompositor_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _MixedRemoteViewCompositor, 48, 32 )
    IID_BS_LOOKUP_NEXT_TEST( _MixedRemoteViewCompositor, 16 )
    IID_BS_LOOKUP_NEXT_TEST( _MixedRemoteViewCompositor, 8 )
    IID_BS_LOOKUP_NEXT_TEST( _MixedRemoteViewCompositor, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _MixedRemoteViewCompositor, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _MixedRemoteViewCompositor, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _MixedRemoteViewCompositor, 48, *pIndex )
    
}

const ExtendedProxyFileInfo MixedRemoteViewCompositor_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _MixedRemoteViewCompositor_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _MixedRemoteViewCompositor_StubVtblList,
    (const PCInterfaceName * ) & _MixedRemoteViewCompositor_InterfaceNamesList,
    (const IID ** ) & _MixedRemoteViewCompositor_BaseIIDList,
    & _MixedRemoteViewCompositor_IID_Lookup, 
    48,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

