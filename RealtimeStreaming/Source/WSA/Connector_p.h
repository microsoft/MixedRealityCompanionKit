

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for C:\Users\TROYFE~1\AppData\Local\Temp\Connector.idl-00b52da5:
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

#ifndef __Connector_p_h__
#define __Connector_p_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__cplusplus)
#if defined(__MIDL_USE_C_ENUM)
#define MIDL_ENUM enum
#else
#define MIDL_ENUM enum class
#endif
#endif


/* Forward Declarations */ 

#ifndef ____FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_FWD_DEFINED__
#define ____FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_FWD_DEFINED__
typedef interface __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection;

#endif 	/* ____FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_FWD_DEFINED__ */


#ifndef ____FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_FWD_DEFINED__
#define ____FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_FWD_DEFINED__
typedef interface __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection;

#endif 	/* ____FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_FWD_DEFINED__ */


#ifndef ____FIEventHandler_1_boolean_FWD_DEFINED__
#define ____FIEventHandler_1_boolean_FWD_DEFINED__
typedef interface __FIEventHandler_1_boolean __FIEventHandler_1_boolean;

#endif 	/* ____FIEventHandler_1_boolean_FWD_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnector __x_ABI_CRealtimeStreaming_CNetwork_CIConnector;

#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "Connection.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_Connector_0000_0000 */
/* [local] */ 








extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0000_v0_0_s_ifspec;

/* interface __MIDL_itf_Connector2Eidl_0000_2266 */




extern RPC_IF_HANDLE __MIDL_itf_Connector2Eidl_0000_2266_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connector2Eidl_0000_2266_v0_0_s_ifspec;

/* interface __MIDL_itf_Connector_0000_0001 */
/* [local] */ 

#ifndef DEF___FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection
#define DEF___FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)



extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0001_v0_0_s_ifspec;

#ifndef ____FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__
#define ____FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__

/* interface __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("45c830d1-4827-5175-b3da-fe2695798e03")
    __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection *asyncInfo,
            /* [in] */ AsyncStatus status) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection * This,
            /* [in] */ __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection *asyncInfo,
            /* [in] */ AsyncStatus status);
        
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


#endif 	/* C style interface */




#endif 	/* ____FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_Connector_0000_0002 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection */


extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0002_v0_0_s_ifspec;

/* interface __MIDL_itf_Connector2Eidl_0000_2267 */




extern RPC_IF_HANDLE __MIDL_itf_Connector2Eidl_0000_2267_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connector2Eidl_0000_2267_v0_0_s_ifspec;

/* interface __MIDL_itf_Connector_0000_0003 */
/* [local] */ 

#ifndef DEF___FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection
#define DEF___FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0003_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0003_v0_0_s_ifspec;

#ifndef ____FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__
#define ____FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__

/* interface __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b0827db8-0aab-584a-9d02-9f162504c7a6")
    __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection : public IInspectable
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Completed( 
            /* [in] */ __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection *handler) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Completed( 
            /* [retval][out] */ __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection **handler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetResults( 
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CNetwork_CIConnection **results) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Completed )( 
            __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This,
            /* [in] */ __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection *handler);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Completed )( 
            __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This,
            /* [retval][out] */ __FIAsyncOperationCompletedHandler_1_RealtimeStreaming__CNetwork__CConnection **handler);
        
        HRESULT ( STDMETHODCALLTYPE *GetResults )( 
            __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection * This,
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CNetwork_CIConnection **results);
        
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


#endif 	/* C style interface */




#endif 	/* ____FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_Connector_0000_0004 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection */


extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0004_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0004_v0_0_s_ifspec;

/* interface __MIDL_itf_Connector2Eidl_0000_2268 */




extern RPC_IF_HANDLE __MIDL_itf_Connector2Eidl_0000_2268_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connector2Eidl_0000_2268_v0_0_s_ifspec;

/* interface __MIDL_itf_Connector_0000_0005 */
/* [local] */ 

#ifndef DEF___FIEventHandler_1_boolean
#define DEF___FIEventHandler_1_boolean
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0005_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0005_v0_0_s_ifspec;

#ifndef ____FIEventHandler_1_boolean_INTERFACE_DEFINED__
#define ____FIEventHandler_1_boolean_INTERFACE_DEFINED__

/* interface __FIEventHandler_1_boolean */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FIEventHandler_1_boolean;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6ea71ff9-100d-54c3-bb4a-6922523cf9a9")
    __FIEventHandler_1_boolean : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ IInspectable *sender,
            /* [in] */ boolean e) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FIEventHandler_1_booleanVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FIEventHandler_1_boolean * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FIEventHandler_1_boolean * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FIEventHandler_1_boolean * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            __FIEventHandler_1_boolean * This,
            /* [in] */ IInspectable *sender,
            /* [in] */ boolean e);
        
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


#endif 	/* C style interface */




#endif 	/* ____FIEventHandler_1_boolean_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_Connector_0000_0006 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FIEventHandler_1_boolean */


extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0006_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0006_v0_0_s_ifspec;

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnector */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIConnector;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6229e89c-2e85-11e9-b210-d663bd873d93")
    __x_ABI_CRealtimeStreaming_CNetwork_CIConnector : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ConnectAsync( 
            /* [retval][out] */ __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection **operation) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add_Closed( 
            /* [in] */ __FIEventHandler_1_boolean *handler,
            /* [out][retval] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_Closed( 
            /* [in] */ EventRegistrationToken token) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *ConnectAsync )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
            /* [retval][out] */ __FIAsyncOperation_1_RealtimeStreaming__CNetwork__CConnection **operation);
        
        HRESULT ( STDMETHODCALLTYPE *add_Closed )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
            /* [in] */ __FIEventHandler_1_boolean *handler,
            /* [out][retval] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_Closed )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIConnector * This,
            /* [in] */ EventRegistrationToken token);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorVtbl;

    interface __x_ABI_CRealtimeStreaming_CNetwork_CIConnector
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIConnectorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_ConnectAsync(This,operation)	\
    ( (This)->lpVtbl -> ConnectAsync(This,operation) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_add_Closed(This,handler,token)	\
    ( (This)->lpVtbl -> add_Closed(This,handler,token) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIConnector_remove_Closed(This,token)	\
    ( (This)->lpVtbl -> remove_Closed(This,token) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIConnector_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_Connector_0000_0007 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0007_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Connector_0000_0007_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


