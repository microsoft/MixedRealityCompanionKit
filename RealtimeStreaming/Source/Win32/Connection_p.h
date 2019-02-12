

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for C:\Users\TROYFE~1\AppData\Local\Temp\Connection.idl-21876c51:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Connection_p_h__
#define __Connection_p_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ____FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs_FWD_DEFINED__
#define ____FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs_FWD_DEFINED__
typedef interface __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs;

#endif 	/* ____FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs_FWD_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnection __x_ABI_CRealtimeStreaming_CNetwork_CIConnection;

#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "Windows.Foundation.h"
#include "Windows.Networking.Sockets.h"
#include "RealtimeStreaming.h"
#include "DataBundle.h"
#include "DataBundleArgs.h"
#include "ModuleManager.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_Connection_0000_0000 */
/* [local] */ 








extern RPC_IF_HANDLE __MIDL_itf_Connection_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connection_0000_0000_v0_0_s_ifspec;

/* interface __MIDL_itf_Connection2Eidl_0000_2263 */




extern RPC_IF_HANDLE __MIDL_itf_Connection2Eidl_0000_2263_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connection2Eidl_0000_2263_v0_0_s_ifspec;

/* interface __MIDL_itf_Connection_0000_0001 */
/* [local] */ 

#ifndef DEF___FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs
#define DEF___FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


extern RPC_IF_HANDLE __MIDL_itf_Connection_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connection_0000_0001_v0_0_s_ifspec;

#ifndef ____FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs_INTERFACE_DEFINED__
#define ____FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs_INTERFACE_DEFINED__

/* interface __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("fd75c4c7-22c5-5112-8322-cb204f5163b9")
    __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ IInspectable *sender,
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs *e) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs * This,
            /* [in] */ IInspectable *sender,
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs *e);
        
        END_INTERFACE
    } __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgsVtbl;

    interface __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs
    {
        CONST_VTBL struct __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs_Invoke(This,sender,e)	\
    ( (This)->lpVtbl -> Invoke(This,sender,e) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_Connection_0000_0002 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs */


extern RPC_IF_HANDLE __MIDL_itf_Connection_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connection_0000_0002_v0_0_s_ifspec;

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnection */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("05b7d68c-2e80-11e9-b210-d663bd873d93")
    __x_ABI_CRealtimeStreaming_CNetwork_CIConnection : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SendPayloadType( 
            /* [in] */ __x_ABI_CRealtimeStreaming_CPayloadType type,
            /* [retval][out] */ HRESULT *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SendBundleAsync( 
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle *dataBundle,
            /* [retval][out] */ __x_ABI_CWindows_CFoundation_CIAsyncAction **operation) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsConnected( 
            /* [retval][out] */ boolean *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ConnectionInfo( 
            /* [retval][out] */ __x_ABI_CWindows_CNetworking_CSockets_CIStreamSocketInformation **value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add_Disconnected( 
            /* [in] */ __FIEventHandler_1_IInspectable *handler,
            /* [out][retval] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_Disconnected( 
            /* [in] */ EventRegistrationToken token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add_Received( 
            /* [in] */ __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs *handler,
            /* [out][retval] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_Received( 
            /* [in] */ EventRegistrationToken token) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *SendPayloadType )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [in] */ __x_ABI_CRealtimeStreaming_CPayloadType type,
            /* [retval][out] */ HRESULT *result);
        
        HRESULT ( STDMETHODCALLTYPE *SendBundleAsync )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle *dataBundle,
            /* [retval][out] */ __x_ABI_CWindows_CFoundation_CIAsyncAction **operation);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsConnected )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [retval][out] */ boolean *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ConnectionInfo )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [retval][out] */ __x_ABI_CWindows_CNetworking_CSockets_CIStreamSocketInformation **value);
        
        HRESULT ( STDMETHODCALLTYPE *add_Disconnected )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [in] */ __FIEventHandler_1_IInspectable *handler,
            /* [out][retval] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_Disconnected )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [in] */ EventRegistrationToken token);
        
        HRESULT ( STDMETHODCALLTYPE *add_Received )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [in] */ __FIEventHandler_1_RealtimeStreaming__CNetwork__CBundleReceivedArgs *handler,
            /* [out][retval] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_Received )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnection * This,
            /* [in] */ EventRegistrationToken token);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionVtbl;

    interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnection
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_SendPayloadType(This,type,result)	\
    ( (This)->lpVtbl -> SendPayloadType(This,type,result) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_SendBundleAsync(This,dataBundle,operation)	\
    ( (This)->lpVtbl -> SendBundleAsync(This,dataBundle,operation) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_get_IsConnected(This,value)	\
    ( (This)->lpVtbl -> get_IsConnected(This,value) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_get_ConnectionInfo(This,value)	\
    ( (This)->lpVtbl -> get_ConnectionInfo(This,value) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_add_Disconnected(This,handler,token)	\
    ( (This)->lpVtbl -> add_Disconnected(This,handler,token) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_remove_Disconnected(This,token)	\
    ( (This)->lpVtbl -> remove_Disconnected(This,token) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_add_Received(This,handler,token)	\
    ( (This)->lpVtbl -> add_Received(This,handler,token) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnection_remove_Received(This,token)	\
    ( (This)->lpVtbl -> remove_Received(This,token) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIConnection_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_Connection_0000_0003 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_Connection_0000_0003_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connection_0000_0003_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


