

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for C:\Users\TROYFE~1\AppData\Local\Temp\DataBundle.idl-f9e0c826:
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

#ifndef __DataBundle_p_h__
#define __DataBundle_p_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle;

#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_FWD_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory;

#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "DataBuffer.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DataBundle_0000_0000 */
/* [local] */ 






extern RPC_IF_HANDLE __MIDL_itf_DataBundle_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DataBundle_0000_0000_v0_0_s_ifspec;

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("01458ce6-2e5f-11e9-b210-d663bd873d93")
    __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle : public IInspectable
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BufferCount( 
            /* [retval][out] */ unsigned int *value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_TotalSize( 
            /* [retval][out] */ unsigned __int64 *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddBuffer( 
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer *dataBuffer,
            /* [retval][out] */ HRESULT *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InsertBuffer( 
            /* [in] */ unsigned int index,
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer *dataBuffer,
            /* [retval][out] */ HRESULT *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveBuffer( 
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer *dataBuffer,
            /* [retval][out] */ HRESULT *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveBufferByIndex( 
            /* [in] */ unsigned int index,
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer **result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( 
            /* [retval][out] */ HRESULT *result) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BufferCount )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
            /* [retval][out] */ unsigned int *value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_TotalSize )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
            /* [retval][out] */ unsigned __int64 *value);
        
        HRESULT ( STDMETHODCALLTYPE *AddBuffer )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer *dataBuffer,
            /* [retval][out] */ HRESULT *result);
        
        HRESULT ( STDMETHODCALLTYPE *InsertBuffer )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
            /* [in] */ unsigned int index,
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer *dataBuffer,
            /* [retval][out] */ HRESULT *result);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveBuffer )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer *dataBuffer,
            /* [retval][out] */ HRESULT *result);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveBufferByIndex )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
            /* [in] */ unsigned int index,
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer **result);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle * This,
            /* [retval][out] */ HRESULT *result);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleVtbl;

    interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_get_BufferCount(This,value)	\
    ( (This)->lpVtbl -> get_BufferCount(This,value) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_get_TotalSize(This,value)	\
    ( (This)->lpVtbl -> get_TotalSize(This,value) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_AddBuffer(This,dataBuffer,result)	\
    ( (This)->lpVtbl -> AddBuffer(This,dataBuffer,result) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_InsertBuffer(This,index,dataBuffer,result)	\
    ( (This)->lpVtbl -> InsertBuffer(This,index,dataBuffer,result) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_RemoveBuffer(This,dataBuffer,result)	\
    ( (This)->lpVtbl -> RemoveBuffer(This,dataBuffer,result) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_RemoveBufferByIndex(This,index,result)	\
    ( (This)->lpVtbl -> RemoveBufferByIndex(This,index,result) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_Reset(This,result)	\
    ( (This)->lpVtbl -> Reset(This,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundle_INTERFACE_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("042b0fbc-2e5f-11e9-b210-d663bd873d93")
    __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory : public IInspectable
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory * This,
            /* [out] */ TrustLevel *trustLevel);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactoryVtbl;

    interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBundleFactory_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


