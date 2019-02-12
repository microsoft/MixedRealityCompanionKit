

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for C:\Users\TroyF\AppData\Local\Temp\DataBuffer.idl-b1fed64b:
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

#ifndef __DataBuffer_p_h__
#define __DataBuffer_p_h__

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

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer;

#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_FWD_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory;

#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "Windows.Storage.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DataBuffer_0000_0000 */
/* [local] */ 




#pragma once



extern RPC_IF_HANDLE __MIDL_itf_DataBuffer_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DataBuffer_0000_0000_v0_0_s_ifspec;

#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer */
/* [object][uuid] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9c3e8bdb-28c3-5a16-8688-85ea99443847")
    __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer : public IInspectable
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_CurrentLength( 
            /* [retval][out] */ unsigned __int64 *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_CurrentLength( 
            /* [in] */ unsigned __int64 value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Offset( 
            /* [retval][out] */ unsigned __int64 *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Offset( 
            /* [in] */ unsigned __int64 value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TrimLeft( 
            /* [in] */ unsigned __int64 cbSize,
            /* [retval][out] */ HRESULT *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE TrimRight( 
            /* [in] */ unsigned __int64 cbSize,
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer **result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( 
            /* [retval][out] */ HRESULT *result) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
            /* [out] */ TrustLevel *trustLevel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurrentLength )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
            /* [retval][out] */ unsigned __int64 *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurrentLength )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
            /* [in] */ unsigned __int64 value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Offset )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
            /* [retval][out] */ unsigned __int64 *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Offset )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
            /* [in] */ unsigned __int64 value);
        
        HRESULT ( STDMETHODCALLTYPE *TrimLeft )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
            /* [in] */ unsigned __int64 cbSize,
            /* [retval][out] */ HRESULT *result);
        
        HRESULT ( STDMETHODCALLTYPE *TrimRight )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
            /* [in] */ unsigned __int64 cbSize,
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer **result);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer * This,
            /* [retval][out] */ HRESULT *result);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferVtbl;

    interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_get_CurrentLength(This,value)	\
    ( (This)->lpVtbl -> get_CurrentLength(This,value) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_put_CurrentLength(This,value)	\
    ( (This)->lpVtbl -> put_CurrentLength(This,value) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_get_Offset(This,value)	\
    ( (This)->lpVtbl -> get_Offset(This,value) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_put_Offset(This,value)	\
    ( (This)->lpVtbl -> put_Offset(This,value) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_TrimLeft(This,cbSize,result)	\
    ( (This)->lpVtbl -> TrimLeft(This,cbSize,result) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_TrimRight(This,cbSize,result)	\
    ( (This)->lpVtbl -> TrimRight(This,cbSize,result) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_Reset(This,result)	\
    ( (This)->lpVtbl -> Reset(This,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer_INTERFACE_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory */
/* [object][uuid] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6ac59985-ec6a-5753-aa3f-9e45717bda8b")
    __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [in] */ unsigned __int64 size,
            /* [out][retval] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer **value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *CreateInstance )( 
            __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory * This,
            /* [in] */ unsigned __int64 size,
            /* [out][retval] */ __x_ABI_CRealtimeStreaming_CNetwork_CIDataBuffer **value);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactoryVtbl;

    interface __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_CreateInstance(This,size,value)	\
    ( (This)->lpVtbl -> CreateInstance(This,size,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CNetwork_CIDataBufferFactory_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


