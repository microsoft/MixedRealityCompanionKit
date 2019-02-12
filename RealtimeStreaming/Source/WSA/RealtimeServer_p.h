

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for C:\Users\TROYFE~1\AppData\Local\Temp\RealtimeServer.idl-08d14e7f:
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

#ifndef __RealtimeServer_p_h__
#define __RealtimeServer_p_h__

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

#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer;

#endif 	/* ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_FWD_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory;

#endif 	/* ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "Windows.Media.h"
#include "Connection.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_RealtimeServer_0000_0000 */
/* [local] */ 






extern RPC_IF_HANDLE __MIDL_itf_RealtimeServer_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_RealtimeServer_0000_0000_v0_0_s_ifspec;

#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer */
/* [object][uuid] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("bdf2a077-3916-5de4-b9ea-74f026b059d5")
    __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE WriteFrame( 
            /* [in] */ unsigned int bufferSize,
            /* [in] */ unsigned int pBufferLength,
            /* [in][size_is] */ byte *pBuffer) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_VideoProperties( 
            /* [retval][out] */ __x_ABI_CWindows_CMedia_CMediaProperties_CIVideoEncodingProperties **value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *WriteFrame )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
            /* [in] */ unsigned int bufferSize,
            /* [in] */ unsigned int pBufferLength,
            /* [in][size_is] */ byte *pBuffer);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_VideoProperties )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer * This,
            /* [retval][out] */ __x_ABI_CWindows_CMedia_CMediaProperties_CIVideoEncodingProperties **value);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerVtbl;

    interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_WriteFrame(This,bufferSize,pBufferLength,pBuffer)	\
    ( (This)->lpVtbl -> WriteFrame(This,bufferSize,pBufferLength,pBuffer) ) 

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_get_VideoProperties(This,value)	\
    ( (This)->lpVtbl -> get_VideoProperties(This,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer_INTERFACE_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory */
/* [object][uuid] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3baec682-8a74-582b-97a6-e4ec999c4c7f")
    __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIConnection *connection,
            /* [in] */ GUID inputMediaType,
            /* [in] */ __x_ABI_CWindows_CMedia_CMediaProperties_CIMediaEncodingProfile *mediaEncodingProperties,
            /* [out][retval] */ __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer **value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *CreateInstance )( 
            __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory * This,
            /* [in] */ __x_ABI_CRealtimeStreaming_CNetwork_CIConnection *connection,
            /* [in] */ GUID inputMediaType,
            /* [in] */ __x_ABI_CWindows_CMedia_CMediaProperties_CIMediaEncodingProfile *mediaEncodingProperties,
            /* [out][retval] */ __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServer **value);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactoryVtbl;

    interface __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_CreateInstance(This,connection,inputMediaType,mediaEncodingProperties,value)	\
    ( (This)->lpVtbl -> CreateInstance(This,connection,inputMediaType,mediaEncodingProperties,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CMedia_CIRealtimeServerFactory_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


