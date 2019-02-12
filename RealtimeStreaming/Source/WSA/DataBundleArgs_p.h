

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for C:\Users\TROYFE~1\AppData\Local\Temp\DataBundleArgs.idl-de0da49e:
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

#ifndef __DataBundleArgs_p_h__
#define __DataBundleArgs_p_h__

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

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs;

#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_FWD_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory;

#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "RealtimeStreaming.h"
#include "Connection.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DataBundleArgs_0000_0000 */
/* [local] */ 







extern RPC_IF_HANDLE __MIDL_itf_DataBundleArgs_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DataBundleArgs_0000_0000_v0_0_s_ifspec;

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("26a177fe-2e53-11e9-b210-d663bd873d93")
    __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs : public IInspectable
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PayloadType( 
            /* [retval][out] */ enum __x_ABI_CRealtimeStreaming_CPayloadType *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DataConnection( 
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CNetwork_CIConnection **value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Bundle( 
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle **value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PayloadType )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
            /* [retval][out] */ enum __x_ABI_CRealtimeStreaming_CPayloadType *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DataConnection )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CNetwork_CIConnection **value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Bundle )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs * This,
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle **value);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsVtbl;

    interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_get_PayloadType(This,value)	\
    ( (This)->lpVtbl -> get_PayloadType(This,value) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_get_DataConnection(This,value)	\
    ( (This)->lpVtbl -> get_DataConnection(This,value) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_get_Bundle(This,value)	\
    ( (This)->lpVtbl -> get_Bundle(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgs_INTERFACE_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9a172738-2e53-11e9-b210-d663bd873d93")
    __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory : public IInspectable
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory * This,
            /* [out] */ TrustLevel *trustLevel);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactoryVtbl;

    interface __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIBundleReceivedArgsFactory_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


