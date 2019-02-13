#pragma once
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include <unknwn.h>
#include <winrt/windows.foundation.h>
#include <winrt/windows.media.h>
#include <winrt/windows.media.mediaproperties.h>
#include <winrt/windows.networking.sockets.h>
#include <winrt/windows.storage.h>
#include <winrt/windows.storage.streams.h>
#include <winrt/windows.system.threading.h>

// DirectX
#include <directxmath.h>
#include <directxpackedvector.h>
#include <d3d11.h>
#include <d3d11_2.h>
#pragma comment(lib, "d3d11")
#include <dxgi.h>
#pragma comment(lib, "dxgi")

// Media Foundation
#include <mfidl.h>
#include <mferror.h>
#include <mfapi.h>
#include <mfobjects.h>
//#include <mfmediacapture.h>
//#include <mfmediaengine.h>
#include <mfreadwrite.h>

#include "Common/ErrorHandling.h"

#include "Generated Files\winrt\RealtimeStreaming.Common.h"

/*
#include <sdkddkver.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#endif

#define LOG_LEVEL Log_Level_All

// Standard C++ first
#include <assert.h>
#include <vector>
#include <map>
#include <unordered_set>
#include <memory>
#include <future>
#include <random>

// Windows
#include <unknwn.h>
#include <winrt/windows.foundation.h>
//#include <initguid.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.foundation.numerics.h>
#include <winrt/windows.media.h>
#include <winrt/windows.media.effects.h>
#include <winrt/windows.media.mediaproperties.h>
#include <winrt/windows.networking.sockets.h>
#include <winrt/windows.perception.h>
#include <winrt/windows.perception.spatial.h>
#include <winrt/windows.storage.h>
#include <winrt/windows.storage.streams.h>
#include <winrt/windows.system.threading.h>
//#include <robuffer.h>
//#pragma comment(lib, "runtimeobject")

//#include <ppltasks.h>

// DirectX
#include <directxmath.h>
#include <directxpackedvector.h>
#include <d3d11.h>
#include <d3d11_2.h>
#pragma comment(lib, "d3d11")
#include <dxgi.h>
#pragma comment(lib, "dxgi")

// Media Foundation
#include <mfidl.h>
#include <mferror.h>
#include <mfapi.h>
#include <mfobjects.h>
//#include <mfmediacapture.h>
#include <mfmediaengine.h>
#include <mfreadwrite.h>
#pragma comment(lib, "mf")
#pragma comment(lib, "mfplat")
#pragma comment(lib, "mfuuid")
#pragma comment(lib, "mfreadwrite.lib")

#ifndef RTDLL
#ifdef REALTIMESTREAMING_EXPORTS
#define RTDLL EXTERN_C HRESULT __declspec(dllexport) STDAPICALLTYPE
#define RTDLL_(type) EXTERN_C type __declspec(dllexport) STDAPICALLTYPE
#else
#define RTDLL EXTERN_C HRESULT __declspec(dllimport) STDAPICALLTYPE
#define RTDLL_(type) EXTERN_C type __declspec(dllimport) STDAPICALLTYPE
#endif // REALTIMESTREAMING_EXPORTS
#endif

/* TODO: TROY look at data buffer
IUnknown* pUnk = reinterpret_cast<IUnknown*>(buff);
IBufferByteAccess* pBufferByteAccess = nullptr;
HRESULT hr = pUnk->QueryInterface(IID_PPV_ARGS(pBufferByteAccess);
byte *pbytes = nullptr;
hr = pBufferByteAccess->Buffer(&pbytes);


template <typename T>
inline T GetDataType(_In_ Windows::Storage::Streams::IBuffer pBuffer)
{
    // QI for BufferByteAccess
    com_ptr<Windows::Storage::Streams::IBufferByteAccess> spBufferByteAccess;
    if (SUCCEEDED(spBuffer.As(&spBufferByteAccess)))
    {
        byte* pBytes = nullptr;
        if (SUCCEEDED(spBufferByteAccess->Buffer(&pBytes)))
        {
            // cast to correct type
            return reinterpret_cast<T>(pBytes);
        };
    }

    return nullptr;
}

inline std::wstring to_wstring(unsigned int d)
{
    wchar_t buffer[65] = { 0 }; //64 + sign
    _ui64tow_s(d, buffer, sizeof(buffer) / sizeof(buffer[0]), 10);

    return std::wstring(buffer);
}

//using namespace Wrappers;

using namespace Windows::Foundation;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Foundation::Numerics;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Media;
using namespace Windows::Media::Capture;
using namespace Windows::Media::MediaProperties;
using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Perception::Spatial;
using namespace Windows::Storage::Streams;
using namespace Windows::System::Threading;

#include "ErrorHandling.h"
#include "LinkList.h"
#include "MediaUtils.h"

//#include "RealtimeStreaming.h"

#include "RenderingPlugin.h"
#include "ModuleManager.h"
#include "DirectXManager.h"
#include "RealtimeMediaSource.h"
#include "RealtimeMediaPlayer.h"
#include "DataBuffer.h"
#include "DataBundle.h"
#include "DataBundleArgs.h"
#include "Connection.h"
#include "Listener.h"
#include "Connector.h"
#include "Marker.h"
#include "NetworkMediaSinkStream.h"
#include "NetworkMediaSink.h"
#include "RealtimeServer.h"
#include "PluginManager.h"

*/