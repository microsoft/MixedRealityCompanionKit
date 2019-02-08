// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"

#include "PluginManagerStatics.h"

using namespace Microsoft::WRL;
using namespace RealtimeStreaming::Plugin;
using namespace RealtimeStreaming::Network;

STDAPI_(BOOL) DllMain(
    _In_opt_ HINSTANCE hInstance, _In_ DWORD dwReason, _In_opt_ LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);

    if (DLL_PROCESS_ATTACH == dwReason)
    {
        //  Don't need per-thread callbacks
        DisableThreadLibraryCalls(hInstance);

        Microsoft::WRL::Module<Microsoft::WRL::InProc>::GetModule().Create();
    }
    else if (DLL_PROCESS_DETACH == dwReason)
    {
        Microsoft::WRL::Module<Microsoft::WRL::InProc>::GetModule().Terminate();
    }

    return TRUE;
}

STDAPI DllGetActivationFactory(_In_ HSTRING activatibleClassId, _COM_Outptr_ IActivationFactory** factory)
{
    auto &module = Microsoft::WRL::Module< Microsoft::WRL::InProc>::GetModule();
    return module.GetActivationFactory(activatibleClassId, factory);
}

STDAPI DllCanUnloadNow()
{
    const auto &module = Microsoft::WRL::Module<Microsoft::WRL::InProc>::GetModule();
    return module.GetObjectCount() == 0 ? S_OK : S_FALSE;
}

RTDLL_(void) UnityPluginLoad(_In_ IUnityInterfaces* unityInterfaces)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        instance->Load(unityInterfaces, PluginManagerStaticsImpl::OnGraphicsDeviceEvent);
    }
}

RTDLL_(void) UnityPluginUnload()
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        instance->UnLoad();
    }
}

RTDLL_(UnityRenderingEvent) GetPluginEventFunc()
{
    return PluginManagerStaticsImpl::OnPluginEvent;
}

RTDLL_(void) SetTime(_In_ float t)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        instance->SetTime(t);
    }
}

//
//
// Network Connection Interop
//
//

RTDLL ListenerCreateAndStart(
    _In_ UINT16 port, 
    _Inout_ UINT32* listenerHandle,
    _In_ PluginCallback callback,
	_In_ void* managedObject)
{
	if (managedObject == nullptr)
	{
		return E_INVALIDARG;
	}

    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ListenerCreateAndStart(port, listenerHandle, callback, managedObject);
    }

    return RPC_E_WRONG_THREAD;
}
RTDLL ListenerStopAndClose(
    _In_ UINT32 handle)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ListenerStopAndClose(handle);
    }

    return RPC_E_WRONG_THREAD;
}

RTDLL ConnectorCreateAndStart(
    _In_ LPCWSTR address, 
    _Inout_ UINT32* connectorHandle,
    _In_ PluginCallback callback,
	_In_ void* managedObject)
{
	if (managedObject == nullptr)
	{
		return E_INVALIDARG;
	}

    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ConnectorCreateAndStart(address, connectorHandle, callback, managedObject);
    }

    return RPC_E_WRONG_THREAD;
}
RTDLL ConnectorStopAndClose(
    _In_ UINT32 handle)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ConnectorStopAndClose(handle);
    }

    return RPC_E_WRONG_THREAD;
}

RTDLL ConnectionAddDisconnected(
    _In_ UINT32 handle,
    _In_ PluginCallback callback,
	_In_ void* managedObject,
    _Out_ INT64* tokenValue)
{
	if (managedObject == nullptr)
	{
		return E_INVALIDARG;
	}

    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ConnectionAddDisconnected(
            static_cast<ModuleHandle>(handle),
            callback,
			managedObject,
            tokenValue);
    }

    return RPC_E_WRONG_THREAD;
}
RTDLL ConnectionRemoveDisconnected(
    _In_ UINT32 handle,
    _In_ INT64 tokenValue)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ConnectionRemoveDisconnected(handle, tokenValue);
    }

    return RPC_E_WRONG_THREAD;
}
RTDLL ConnectionAddReceived(
    _In_ UINT32 handle,
    _In_ DataReceivedHandler callback,
	_In_ void* managedObject,
    _Out_ INT64* tokenValue)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ConnectionAddReceived(
            static_cast<ModuleHandle>(handle),
            callback,
			managedObject,
            tokenValue);
    }

    return RPC_E_WRONG_THREAD;
}
RTDLL ConnectionRemoveReceived(
    _In_ UINT32 handle,
    _In_ INT64 tokenValue)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ConnectionRemoveReceived(handle, tokenValue);
    }

    return RPC_E_WRONG_THREAD;
}
RTDLL ConnectionSendRawData(
    _In_ ModuleHandle connectionHandle, 
    _In_ UINT16 payloadType,
    __in_ecount(bufferSize) byte* pBuffer, 
    _In_ UINT32 bufferSize)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ConnectionSendRawData(connectionHandle, (RealtimeStreaming::Network::PayloadType)payloadType, pBuffer, bufferSize);
    }

    return RPC_E_WRONG_THREAD;
}
RTDLL ConnectionClose(
    _In_ UINT32 handle)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ConnectionClose(handle);
    }

    return RPC_E_WRONG_THREAD;
}

//
//
// Realtime Streaming Provider Interop
//
//

RTDLL CreateRealtimeStreamingServer(
	 //_In_ bool enableAudio,
	 _In_ UINT32 connectionHandle,
	_Inout_ UINT32* serverHandle)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->RTServerCreate(connectionHandle, serverHandle);
    }

    return RPC_E_WRONG_THREAD;
}

RTDLL RealtimeStreamingWrite(
	_In_ UINT32 serverHandle,
	__in_ecount(bufferSize) BYTE* pBuffer,
	_In_ UINT32 bufferSize)
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->RTServerWriteFrame(serverHandle, pBuffer, bufferSize);
	}

	return RPC_E_WRONG_THREAD;
}

RTDLL RealtimeStreamingStop(
	 _In_ UINT32 serverHandle)
{
	 auto instance = PluginManagerStaticsImpl::GetInstance();
	 if (nullptr != instance)
	 {
		  return instance->RTServerShutdown(serverHandle);
	 }

	 return RPC_E_WRONG_THREAD;
}

//
//
// Realtime Streaming Playback Interop
//
//

// TODO: Clean up -> Use module manager?
//static ComPtr<IRealtimeMediaPlayer> s_spStreamingPlayer;

RTDLL CreateRealtimePlayer(
	_In_ ModuleHandle connectionHandle,
	_In_ StateChangedCallback fnCallback,
	_In_ PlayerCreatedCallback callback,
	_In_ void* managedObject)
{

	if (managedObject == nullptr)
	{
		return E_INVALIDARG;
	}

	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->RTPlayerCreate(connectionHandle,
			//fnCallback, 
			callback, 
			managedObject);
	}

	return RPC_E_WRONG_THREAD;
}

RTDLL ReleaseRealtimePlayer()
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->RTPlayerRelease();
	}

	return RPC_E_WRONG_THREAD;
}

RTDLL CreateRealtimePlayerTexture(_In_ UINT32 width, _In_ UINT32 height, _COM_Outptr_ void** ppvTexture)
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->RTPlayerCreateTexture(
			 width, height, ppvTexture);
	}

	return RPC_E_WRONG_THREAD;
}

RTDLL RealtimePlayerPlay()
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->RTPlayerStart();
	}

	return RPC_E_WRONG_THREAD;
}

RTDLL RealtimePlayerPause()
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->RTPlayerPause();
	}

	return RPC_E_WRONG_THREAD;
}

RTDLL RealtimePlayerStop()
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->RTPlayerStop();
	}

	return RPC_E_WRONG_THREAD;
}