// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"

#include "Plugin/PluginManager.h"
#include "Media/RealtimeMediaPlayer.h"

#include "Media.RealtimeServer.g.h"

//using namespace Microsoft::WRL;
using namespace winrt::RealtimeStreaming::Plugin;

namespace PluginInterface = winrt::RealtimeStreaming::Plugin;

/*

TOOD: DllMain needs to move to win32 project?

STDAPI_(BOOL) DllMain(
    _In_opt_ HINSTANCE hInstance, _In_ DWORD dwReason, _In_opt_ LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(lpReserved);

    if (DLL_PROCESS_ATTACH == dwReason)
    {
        //  Don't need per-thread callbacks
        DisableThreadLibraryCalls(hInstance);

        Module<InProc>::GetModule().Create();
    }
    else if (DLL_PROCESS_DETACH == dwReason)
    {
        Module<InProc>::GetModule().Terminate();
    }

    return TRUE;
}

STDAPI DllGetActivationFactory(_In_ HSTRING activatibleClassId, _COM_Outptr_ IActivationFactory** factory)
{
    auto &module = Module< InProc>::GetModule();
    return module.GetActivationFactory(activatibleClassId, factory);
}

STDAPI DllCanUnloadNow()
{
    const auto &module = Module<InProc>::GetModule();
    return module.GetObjectCount() == 0 ? S_OK : S_FALSE;
}
*/

static void UNITY_INTERFACE_API OnRenderEvent(int32_t eventID) {}

RTDLL_(void) UnityPluginLoad(_In_ IUnityInterfaces* unityInterfaces)
{
    implementation::PluginManager::Load(unityInterfaces);
}

RTDLL_(void) UnityPluginUnload()
{
    implementation::PluginManager::UnLoad();
}

RTDLL_(UnityRenderingEvent) GetPluginEventFunc()
{
    return OnRenderEvent;
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

    return PluginInterface::s_instance.ListenerCreateAndStart(port, listenerHandle, callback, managedObject);
}
RTDLL ListenerStopAndClose(
    _In_ UINT32 handle)
{
    return PluginInterface::s_instance.ListenerStopAndClose(handle);
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

    return PluginInterface::s_instance.ConnectorCreateAndStart(address, connectorHandle, callback, managedObject);
}
RTDLL ConnectorStopAndClose(
    _In_ UINT32 handle)
{
    return PluginInterface::s_instance.ConnectorStopAndClose(handle);
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

    return PluginInterface::s_instance.ConnectionAddDisconnected(
        static_cast<ModuleHandle>(handle),
        callback,
        managedObject,
        tokenValue);
}
RTDLL ConnectionRemoveDisconnected(
    _In_ UINT32 handle,
    _In_ INT64 tokenValue)
{
    return PluginInterface::s_instance.ConnectionRemoveDisconnected(handle, tokenValue);
}
RTDLL ConnectionAddReceived(
    _In_ UINT32 handle,
    _In_ DataReceivedHandler callback,
    _In_ void* managedObject,
    _Out_ INT64* tokenValue)
{
    return PluginInterface::s_instance.ConnectionAddReceived(
        static_cast<ModuleHandle>(handle),
        callback,
        managedObject,
        tokenValue);
}
RTDLL ConnectionRemoveReceived(
    _In_ UINT32 handle,
    _In_ INT64 tokenValue)
{
    return PluginInterface::s_instance.ConnectionRemoveReceived(handle, tokenValue);
}

RTDLL ConnectionClose(
    _In_ UINT32 handle)
{
    return PluginInterface::s_instance.ConnectionClose(handle);
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
    return PluginInterface::s_instance.RTServerCreate(connectionHandle, serverHandle);
}

RTDLL RealtimeStreamingWrite(
    _In_ UINT32 serverHandle,
    __in_ecount(bufferSize) BYTE* pBuffer,
    _In_ UINT32 bufferSize)
{
    return PluginInterface::s_instance.RTServerWriteFrame(serverHandle, pBuffer, bufferSize);
}

RTDLL RealtimeStreamingShutdown(
    _In_ UINT32 serverHandle)
{
    return PluginInterface::s_instance.RTServerShutdown(serverHandle);
}

//
//
// Realtime Streaming Playback Interop
//
//

// TODO: Clean up -> Use module manager?
//static com_ptr<IRealtimeMediaPlayer> s_spStreamingPlayer;

RTDLL CreateRealtimePlayer(
    _In_ ModuleHandle connectionHandle,
    _In_ winrt::RealtimeStreaming::Media::StateChangedCallback fnCallback,
    _In_ PlayerCreatedCallback callback,
    _In_ void* managedObject)
{
    if (managedObject == nullptr)
    {
        return E_INVALIDARG;
    }

    return PluginInterface::s_instance.RTPlayerCreate(connectionHandle,
        //fnCallback, 
        callback,
        managedObject);
}

RTDLL ReleaseRealtimePlayer()
{
    return PluginInterface::s_instance.RTPlayerRelease();
}

RTDLL CreateRealtimePlayerTexture(_In_ UINT32 width, _In_ UINT32 height, _COM_Outptr_ void** ppvTexture)
{
    return PluginInterface::s_instance.RTPlayerCreateTexture(
        width, height, ppvTexture);
}

RTDLL RealtimePlayerPlay()
{
    return PluginInterface::s_instance.RTPlayerStart();
}

RTDLL RealtimePlayerPause()
{
    return PluginInterface::s_instance.RTPlayerPause();
}

RTDLL RealtimePlayerStop()
{
    return PluginInterface::s_instance.RTPlayerStop();
}