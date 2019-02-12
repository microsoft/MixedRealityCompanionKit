

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for C:\Users\TROYFE~1\AppData\Local\Temp\ModuleManager.idl-dfb161eb:
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

#ifndef __ModuleManager_p_h__
#define __ModuleManager_p_h__

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

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIModule __x_ABI_CRealtimeStreaming_CPlugin_CIModule;

#endif 	/* ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_FWD_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory;

#endif 	/* ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_FWD_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__
typedef interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager;

#endif 	/* ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "Windows.Foundation.h"
#include "RealtimeStreaming.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_ModuleManager_0000_0000 */
/* [local] */ 








extern RPC_IF_HANDLE __MIDL_itf_ModuleManager_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ModuleManager_0000_0000_v0_0_s_ifspec;

#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CPlugin_CIModule */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModule;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2edeb3fb-8008-4a13-b8d3-7081fd11443f")
    __x_ABI_CRealtimeStreaming_CPlugin_CIModule : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Shutdown( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *Shutdown )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModule * This);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CPlugin_CIModuleVtbl;

    interface __x_ABI_CRealtimeStreaming_CPlugin_CIModule
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CRealtimeStreaming_CPlugin_CIModule_Shutdown(This)	\
    ( (This)->lpVtbl -> Shutdown(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CPlugin_CIModule_INTERFACE_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1d81174e-2dc0-11e9-b210-d663bd873d93")
    __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory : public IInspectable
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory * This,
            /* [out] */ TrustLevel *trustLevel);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactoryVtbl;

    interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleFactory_INTERFACE_DEFINED__ */


#ifndef ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_INTERFACE_DEFINED__
#define ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_INTERFACE_DEFINED__

/* interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager */
/* [uuid][object] */ 


EXTERN_C const IID IID___x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ed7ca192-2e9d-11e9-b210-d663bd873d93")
    __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddModule( 
            /* [in] */ __x_ABI_CRealtimeStreaming_CPlugin_CIModule *pluginModule,
            /* [retval][out] */ unsigned int *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetModule( 
            /* [in] */ unsigned int moduleHandle,
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CPlugin_CIModule **result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReleaseModule( 
            /* [in] */ unsigned int moduleHandle) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
            /* [out] */ HSTRING *className);
        
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
            /* [out] */ TrustLevel *trustLevel);
        
        HRESULT ( STDMETHODCALLTYPE *AddModule )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
            /* [in] */ __x_ABI_CRealtimeStreaming_CPlugin_CIModule *pluginModule,
            /* [retval][out] */ unsigned int *result);
        
        HRESULT ( STDMETHODCALLTYPE *GetModule )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
            /* [in] */ unsigned int moduleHandle,
            /* [retval][out] */ __x_ABI_CRealtimeStreaming_CPlugin_CIModule **result);
        
        HRESULT ( STDMETHODCALLTYPE *ReleaseModule )( 
            __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager * This,
            /* [in] */ unsigned int moduleHandle);
        
        END_INTERFACE
    } __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerVtbl;

    interface __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager
    {
        CONST_VTBL struct __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_AddModule(This,pluginModule,result)	\
    ( (This)->lpVtbl -> AddModule(This,pluginModule,result) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_GetModule(This,moduleHandle,result)	\
    ( (This)->lpVtbl -> GetModule(This,moduleHandle,result) ) 

#define __x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_ReleaseModule(This,moduleHandle)	\
    ( (This)->lpVtbl -> ReleaseModule(This,moduleHandle) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CRealtimeStreaming_CPlugin_CIModuleManager_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


