//********************************************************* 
// 
// Copyright (c) Microsoft. All rights reserved. 
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF 
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY 
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR 
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT. 
// 
//********************************************************* 

#include "pch.h"
#include "AppView.h"

#include <ppltasks.h>

using namespace RemotingHostSample;

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Numerics;
using namespace Windows::Graphics::Holographic;
using namespace Windows::Perception;
using namespace Windows::Perception::Spatial;
using namespace Windows::Perception::Spatial::Surfaces;
using namespace Windows::UI::Core;

AppView::AppView()
{
    // At this point we have access to the device and we can create device-dependent
    // resources, and we know which code path is appropriate for this session.
    m_deviceResources = std::make_shared<DX::DeviceResources>();
    
    m_main = std::make_unique<AppMain>(m_deviceResources);
}

// Called when the app sets up a connection to a source (remoting or simulation).
void AppView::Initialize(HolographicSpace^ holographicSpace, Microsoft::Holographic::RemoteSpeech^ speech)
{
    m_holographicSpace = holographicSpace;

    // The DeviceResources class uses the preferred DXGI adapter ID from the holographic
    // space (when available) to create a Direct3D device. The HolographicSpace
    // uses this ID3D11Device to create and manage device-based resources such as
    // swap chains.
    m_deviceResources->SetHolographicSpace(m_holographicSpace);

    // The main class uses the holographic space for updates and rendering.
    m_main->SetHolographicSpace(m_holographicSpace);

    if (speech)
    {
        m_main->SetSpeechWrapper(speech);
    }

    m_isInitialized = true;
}

// This method is called after the window becomes active. It oversees the
// update, draw, and present loop, and it also oversees window message processing.
void AppView::Tick()
{
    if (m_main)
    {
        // When running on Windows Holographic, we can use the holographic rendering system.
        HolographicFrame^ holographicFrame = m_main->Update();

        if (holographicFrame && m_main->Render(holographicFrame))
        {
            // The holographic frame has an API that presents the swap chain for each
            // holographic camera.
            m_deviceResources->Present(holographicFrame);
        }
    }
}
