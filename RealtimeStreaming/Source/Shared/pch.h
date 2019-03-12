#pragma once
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#define WIN32_LEAN_AND_MEAN

#pragma once
#include <unknwn.h>
#include <winrt/windows.foundation.h>
#include <winrt/windows.media.h>
#include <winrt/windows.media.mediaproperties.h>
#include <winrt/windows.networking.sockets.h>
#include <winrt/windows.storage.h>
#include <winrt/windows.storage.streams.h>
#include <winrt/windows.system.threading.h>

#include <ppltasks.h>

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
#include <mfreadwrite.h>

#include "ErrorHandling.h"

#ifndef MODULE_HANDLE
typedef UINT32 MODULE_HANDLE;
    #define MODULE_HANDLE_INVALID (UINT)0x0bad
    #define MODULE_HANDLE_START (UINT)0x0bae
#endif // MODULE_HANDLE")

typedef int32_t INSTANCE_HANDLE;

const ULONG c_cbReceiveBufferSize = 2 * 1024;
const ULONG c_cbMaxBundleSize = 1024 * 1024;
const UINT16 c_cbMaxBufferFailures = 7;
const UINT16 c_cbMaxBundleFailures = 3;
extern wchar_t const __declspec(selectany)c_szNetworkScheme[] = L"mrvc";
extern wchar_t const __declspec(selectany)c_szNetworkSchemeWithColon[] = L"mrvc:";

const winrt::hstring c_UDP_Communication_Port = L"56788";
const winrt::hstring c_UDP_Multicast_IP = L"237.1.3.37";

#ifndef INSTANCE_HANDLE_INVALID
#define INSTANCE_HANDLE_INVALID static_cast<INSTANCE_HANDLE>(0x0bad)
#define INSTANCE_HANDLE_START static_cast<INSTANCE_HANDLE>(0x0bae)
#endif // INSTANCE_HANDLE_INVALID

#ifndef RTDLL

#define RTDLL EXTERN_C HRESULT __declspec(dllexport) STDAPICALLTYPE
#define RTDLL_(type) EXTERN_C type __declspec(dllexport) STDAPICALLTYPE
/*
#else
#define RTDLL EXTERN_C HRESULT __declspec(dllimport) STDAPICALLTYPE
#define RTDLL_(type) EXTERN_C type __declspec(dllimport) STDAPICALLTYPE
#endif // REALTIMESTREAMING_EXPORTS
*/
#endif
