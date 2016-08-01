//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include <HolographicStreamerHelpers.h>

#include "AppView.h"
#include "resource.h"

#define STREAMER_WIDTH          1280
#define STREAMER_HEIGHT         720

#define INITIAL_WINDOW_WIDTH    STREAMER_WIDTH
#define INITIAL_WINDOW_HEIGHT   STREAMER_HEIGHT

class DesktopWindow
{
public:
    DesktopWindow(Platform::String^ ipAddress);

    void InitializeHwnd(_In_ HWND hWnd);
    void CreateSwapChain(_In_ const Microsoft::WRL::ComPtr<ID3D11Device1>& device);
    void ResizeSwapChain(int width, int height);
    void Tick();
    void ReceiveKeyPress(char key);
    void UpdateWindowTitle();

    Windows::Graphics::Holographic::HolographicSpace^ GetHolographicSpace() { return m_streamerHelpers ? m_streamerHelpers->HolographicSpace : nullptr; }

private:
    void ConnectToRemoteDevice();
    void DisconnectFromRemoteDevice();

    Microsoft::WRL::Wrappers::SRWLock                   m_connectionStateLock;

    RemotingHostSample::AppView^                        m_appView;
    Platform::String^                                   m_ipAddress;
    Microsoft::Holographic::HolographicStreamerHelpers^ m_streamerHelpers;

    Microsoft::WRL::Wrappers::CriticalSection           m_deviceLock;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>             m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_spTexture;

    HWND m_hWnd         = 0;
    int  m_width        = INITIAL_WINDOW_WIDTH;
    int  m_height       = INITIAL_WINDOW_HEIGHT;
    bool m_showPreview  = true;
};
