// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"

#include "PluginManagerStatics.h"

using namespace Microsoft::WRL;
using namespace MixedRemoteViewCompositor::Plugin;
using namespace MixedRemoteViewCompositor::Network;

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

MRVCDLL_(void) UnityPluginLoad(_In_ IUnityInterfaces* unityInterfaces)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        instance->Load(unityInterfaces, PluginManagerStaticsImpl::OnGraphicsDeviceEvent);
    }
}

MRVCDLL_(void) UnityPluginUnload()
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        instance->UnLoad();
    }
}

MRVCDLL_(UnityRenderingEvent) MrvcGetPluginEventFunc()
{
    return PluginManagerStaticsImpl::OnPluginEvent;
}

MRVCDLL_(void) MrvcSetStreamingAssetsPath(_In_ LPCWSTR filePath)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        instance->SetStreamingAssetsPath(filePath);
    }
}

MRVCDLL_(void) MrvcSetTime(_In_ float t)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        instance->SetTime(t);
    }
}

MRVCDLL MrvcListenerCreateAndStart(
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
MRVCDLL MrvcListenerStopAndClose(
    _In_ UINT32 handle)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ListenerStopAndClose(handle);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcConnectorCreateAndStart(
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
MRVCDLL MrvcConnectorStopAndClose(
    _In_ UINT32 handle)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ConnectorStopAndClose(handle);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcConnectionAddDisconnected(
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
MRVCDLL MrvcConnectionRemoveDisconnected(
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
MRVCDLL MrvcConnectionAddReceived(
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
MRVCDLL MrvcConnectionRemoveReceived(
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
MRVCDLL MrvcConnectionSendRawData(
    _In_ ModuleHandle connectionHandle, 
    _In_ UINT16 payloadType,
    __in_ecount(bufferSize) byte* pBuffer, 
    _In_ UINT32 bufferSize)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ConnectionSendRawData(connectionHandle, (MixedRemoteViewCompositor::Network::PayloadType)payloadType, pBuffer, bufferSize);
    }

    return RPC_E_WRONG_THREAD;
}
MRVCDLL MrvcConnectionClose(
    _In_ UINT32 handle)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->ConnectionClose(handle);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL CreateStreamCaptureEngine(
	_Inout_ UINT32* captureHandle)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->CaptureCreate(captureHandle);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcCaptureInit(
	_In_ bool enableAudio,
    _In_ UINT32 captureHandle, 
    _In_ UINT32 connectionHandle)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->CaptureInit(enableAudio, captureHandle, connectionHandle);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcCaptureStop(
    _In_ UINT32 captureHandle)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->CaptureShutdown(captureHandle);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcCaptureWrite(
	_In_ UINT32 captureHandle)
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->CaptureWriteFrame(captureHandle);
	}

	return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcCaptureWriteData(
	_In_ UINT32 captureHandle,
	__in_ecount(bufferSize) byte* pBuffer,
	_In_ UINT32 bufferSize)
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->CaptureWriteFrameData(captureHandle, pBuffer, bufferSize);
	}

	return RPC_E_WRONG_THREAD;
}

// StreamingPlayer

// TODO: Clean up -> Use module manager?
//static ComPtr<IStreamingMediaPlayer> s_spStreamingPlayer;

MRVCDLL CreateStreamingPlayer(
	_In_ ModuleHandle connectionHandle,
	_In_ StateChangedCallback fnCallback,
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
		return instance->CreateStreamPlayer(connectionHandle, 
			//fnCallback, 
			callback, 
			managedObject);
	}

	return RPC_E_WRONG_THREAD;
}

MRVCDLL ReleaseMediaPlayback()
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->ReleaseMediaPlayback();
	}

	return RPC_E_WRONG_THREAD;
}

MRVCDLL CreateStreamingTexture(_In_ UINT32 width, _In_ UINT32 height, _COM_Outptr_ void** ppvTexture)
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->CreateStreamingTexture(width, height, ppvTexture);
	}

	return RPC_E_WRONG_THREAD;
}

MRVCDLL StreamingPlay()
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->StreamingPlay();
	}

	return RPC_E_WRONG_THREAD;
}

MRVCDLL StreamingPause()
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->StreamingPause();
	}

	return RPC_E_WRONG_THREAD;
}

MRVCDLL StreamingStop()
{
	auto instance = PluginManagerStaticsImpl::GetInstance();
	if (nullptr != instance)
	{
		return instance->StreamingStop();
	}

	return RPC_E_WRONG_THREAD;
}


// PlaybackEngine

MRVCDLL MrvcPlaybackCreate(
    _In_ ModuleHandle connectionHandle,
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
        return instance->PlaybackCreate(connectionHandle, callback, managedObject);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcPlaybackAddSizeChanged(
    _In_ ModuleHandle handle,
    _In_ FrameSizeChanged callback,
	_In_ void* managedObject,
    _Out_ INT64* tokenValue)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->PlaybackAddSizeChanged(handle, callback, managedObject, tokenValue);
    }

    return RPC_E_WRONG_THREAD;
}
MRVCDLL MrvcPlaybackRemoveSizeChanged(
    _In_ ModuleHandle handle,
    _Out_ INT64 tokenValue) 
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->PlaybackRemoveSizeChanged(handle, tokenValue);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcPlaybackAddSampleUpdated(
    _In_ ModuleHandle handle,
    _In_ SampleUpdated callback,
	_In_ void* managedObject,
    _Out_ INT64* tokenValue) 
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->PlaybackAddSampleUpdated(handle, callback, managedObject, tokenValue);
    }

    return RPC_E_WRONG_THREAD;
}
MRVCDLL MrvcPlaybackRemoveSampleUpdated(
    _In_ ModuleHandle handle,
    _Out_ INT64 tokenValue) 
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->PlaybackRemoveSampleUpdated(handle, tokenValue);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcPlaybackGetFrameData(
    _In_ ModuleHandle handle,
    _Inout_ MediaSampleArgs* args)
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->PlaybackGetFrameData(handle, args);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcPlaybackStart(
    _In_ ModuleHandle handle) 
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->PlaybackStart(handle);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcPlaybackStop(
    _In_ ModuleHandle handle) 
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->PlaybackStop(handle);
    }

    return RPC_E_WRONG_THREAD;
}

MRVCDLL MrvcPlaybackClose(
    _In_ ModuleHandle handle) 
{
    auto instance = PluginManagerStaticsImpl::GetInstance();
    if (nullptr != instance)
    {
        return instance->PlaybackClose(handle);
    }

    return RPC_E_WRONG_THREAD;
}
