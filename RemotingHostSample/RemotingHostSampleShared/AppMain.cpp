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

#include "pch.h"
#include <HolographicStreamerHelpers.h>
#include "AppMain.h"
#include "Common\DirectXHelper.h"

#include <windows.graphics.directx.direct3d11.interop.h>
#include <Collection.h>
#include <ppltasks.h>

using namespace RemotingHostSample;

using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Foundation::Numerics;
using namespace Windows::Graphics::Holographic;
using namespace Windows::Perception::Spatial;
using namespace Windows::Perception::Spatial::Surfaces;
using namespace Windows::Storage;
using namespace Windows::UI::Input::Spatial;
using namespace std::placeholders;

// Loads and initializes application assets when the application is loaded.
AppMain::AppMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
    m_deviceResources(deviceResources)
{
    // Register to be notified if the device is lost or recreated.
    m_deviceResources->RegisterDeviceNotify(this);
}

task<StorageFolder^> GetTempFolderAsync()
{
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
    wchar_t tempFolderPath[MAX_PATH];
    if (ExpandEnvironmentStrings(L"%TEMP%", tempFolderPath, _countof(tempFolderPath)) == 0) { throw GetLastError(); }
    return create_task(Windows::Storage::StorageFolder::GetFolderFromPathAsync(Platform::StringReference(tempFolderPath)));
#endif
    return create_task([]() { return Windows::Storage::ApplicationData::Current->LocalCacheFolder; });
}

task<StorageFile^> CreateGrammarFileAsync()
{
    Platform::String^ ns = "http://www.w3.org/2001/06/grammar";
    auto doc = ref new Windows::Data::Xml::Dom::XmlDocument();

    auto grammar = doc->CreateElementNS(ns, "grammar");
    grammar->SetAttribute("version", "1.0");
    grammar->SetAttribute("xml:lang", "en-US");
    grammar->SetAttribute("root", "remoting");
    doc->AppendChild(grammar);

    auto rule = doc->CreateElementNS(ns, "rule");
    rule->SetAttribute("id", "remoting");
    grammar->AppendChild(rule);

    auto item = doc->CreateElementNS(ns, "item");
    item->InnerText = "Hello world";

    rule->AppendChild(item);

    return GetTempFolderAsync().then([](StorageFolder^ tempFolder)
    {
        return create_task(tempFolder->CreateFileAsync("grammar.xml", CreationCollisionOption::ReplaceExisting));
    }).then([doc](StorageFile^ grammarFile)
    {
        return create_task(doc->SaveToFileAsync(grammarFile)).then([grammarFile] { return grammarFile; });
    });
}

void AppMain::SetSpeechWrapper(Microsoft::Holographic::RemoteSpeech^ speechWrapper)
{
    using namespace Microsoft::Holographic;
    SpeechState^ state = ref new SpeechState();
    state->Dictionary->Append("Red");
    state->Dictionary->Append("Blue");
    state->Dictionary->Append("Green");
    state->Dictionary->Append("Default");
    state->Dictionary->Append("Aquamarine");
    state->Dictionary->Append("Save position");
    state->Dictionary->Append("Load position");
    state->Language = "en-US";
    state->mode = RemotingSpeechRecognitionMode::On;

    CreateGrammarFileAsync().then([this, speechWrapper, state](StorageFile^ grammarFile)
    {
        m_remoteSpeech = speechWrapper;
        m_remoteSpeech->SetSpeechState(state);
        m_remoteSpeech->SetGrammarFile(grammarFile);
        m_speechToken = speechWrapper->OnRecognizedSpeech += ref new Windows::Foundation::EventHandler<RecognizedSpeech^>(std::bind(&AppMain::OnRecognizedSpeech, this, _1, _2)); //
    });
}

void AppMain::OnRecognizedSpeech(Object^ sender, Microsoft::Holographic::RecognizedSpeech^ recognizedSpeech)
{
    if (recognizedSpeech->RecognizedText == "Red")
    {
        // set filter color to red
        DirectX::XMFLOAT4 color = { 1, 0, 0, 1 };
        m_spinningCubeRenderer->SetColorFilter(color);
    }
    else if (recognizedSpeech->RecognizedText == "Blue")
    {
        // set filter color to blue
        DirectX::XMFLOAT4 color = { 0, 0, 1, 1 };
        m_spinningCubeRenderer->SetColorFilter(color);
    }
    else if (recognizedSpeech->RecognizedText == "Green")
    {
        // set filter color to green
        DirectX::XMFLOAT4 color = { 0, 1, 0, 1 };
        m_spinningCubeRenderer->SetColorFilter(color);
    }
    else if (recognizedSpeech->RecognizedText == "Default")
    {
        // remove color filter
        DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };
        m_spinningCubeRenderer->SetColorFilter(color);
    }
    else if (recognizedSpeech->RecognizedText == "Aquamarine")
    {
        // set filter color to cyan
        DirectX::XMFLOAT4 color = { 0, 1, 1, 1 };
        m_spinningCubeRenderer->SetColorFilter(color);
    }
    else if (recognizedSpeech->RecognizedText == "Load position")
    {
        create_task(SpatialAnchorManager::RequestStoreAsync()).then([this](SpatialAnchorStore^ store)
        {
            auto anchors = store->GetAllSavedAnchors();
            if (anchors->HasKey("position"))
            {
                auto position = anchors->Lookup("position");
                auto positionToOrigin = position->CoordinateSystem->TryGetTransformTo(m_referenceFrame->CoordinateSystem);
                if (positionToOrigin)
                {
                    m_spinningCubeRenderer->SetPosition(transform(float3::zero(), positionToOrigin->Value));
                    OutputDebugString(L"Loaded cube position from SpatialAnchorStore.\n");
                }
            }
        });
    }
    else if (recognizedSpeech->RecognizedText == "Save position")
    {
        auto position = SpatialAnchor::TryCreateRelativeTo(m_referenceFrame->CoordinateSystem, m_spinningCubeRenderer->GetPosition());
        create_task(SpatialAnchorManager::RequestStoreAsync()).then([position](SpatialAnchorStore^ store)
        {
            store->Clear();
            if (store->TrySave("position", position))
            {
                OutputDebugString(L"Saved cube position to SpatialAnchorStore.\n");
            }
        });
    }
}

void AppMain::SetHolographicSpace(HolographicSpace^ holographicSpace)
{
    UnregisterHolographicEventHandlers();

    m_holographicSpace = holographicSpace;

    // Initialize the sample content.
    m_spinningCubeRenderer = std::make_unique<SpinningCubeRenderer>(m_deviceResources);
    m_spatialInputRenderer = std::make_unique<SpatialInputRenderer>(m_deviceResources);
    m_spatialInputHandler = std::make_shared<SpatialInputHandler>();

    // Use the default SpatialLocator to track the motion of the device.
    m_locator = SpatialLocator::GetDefault();

    // Be able to respond to changes in the positional tracking state.
    m_locatabilityChangedToken =
        m_locator->LocatabilityChanged +=
            ref new Windows::Foundation::TypedEventHandler<SpatialLocator^, Object^>(
                std::bind(&AppMain::OnLocatabilityChanged, this, _1, _2)
                );

    // Respond to camera added events by creating any resources that are specific
    // to that camera, such as the back buffer render target view.
    // When we add an event handler for CameraAdded, the API layer will avoid putting
    // the new camera in new HolographicFrames until we complete the deferral we created
    // for that handler, or return from the handler without creating a deferral. This
    // allows the app to take more than one frame to finish creating resources and
    // loading assets for the new holographic camera.
    // This function should be registered before the app creates any HolographicFrames.
    m_cameraAddedToken =
        m_holographicSpace->CameraAdded +=
            ref new Windows::Foundation::TypedEventHandler<HolographicSpace^, HolographicSpaceCameraAddedEventArgs^>(
                std::bind(&AppMain::OnCameraAdded, this, _1, _2)
                );

    // Respond to camera removed events by releasing resources that were created for that
    // camera.
    // When the app receives a CameraRemoved event, it releases all references to the back
    // buffer right away. This includes render target views, Direct2D target bitmaps, and so on.
    // The app must also ensure that the back buffer is not attached as a render target, as
    // shown in DeviceResources::ReleaseResourcesForBackBuffer.
    m_cameraRemovedToken =
        m_holographicSpace->CameraRemoved +=
            ref new Windows::Foundation::TypedEventHandler<HolographicSpace^, HolographicSpaceCameraRemovedEventArgs^>(
                std::bind(&AppMain::OnCameraRemoved, this, _1, _2)
                );

    // The simplest way to render world-locked holograms is to create a stationary reference frame
    // when the app is launched. This is roughly analogous to creating a "world" coordinate system
    // with the origin placed at the device's position as the app is launched.
    m_referenceFrame = m_locator->CreateStationaryFrameOfReferenceAtCurrentLocation();
}

void AppMain::UnregisterHolographicEventHandlers()
{
    if (m_holographicSpace != nullptr)
    {
        // Clear previous event registrations.

        if (m_cameraAddedToken.Value != 0)
        {
            m_holographicSpace->CameraAdded -= m_cameraAddedToken;
            m_cameraAddedToken.Value = 0;
        }

        if (m_cameraRemovedToken.Value != 0)
        {
            m_holographicSpace->CameraRemoved -= m_cameraRemovedToken;
            m_cameraRemovedToken.Value = 0;
        }
    }

    if (m_locator != nullptr)
    {
        m_locator->LocatabilityChanged -= m_locatabilityChangedToken;
    }
}

AppMain::~AppMain()
{
    // Deregister device notification.
    m_deviceResources->RegisterDeviceNotify(nullptr);
    if (m_speechToken.Value != 0)
    {
        m_remoteSpeech->OnRecognizedSpeech -= m_speechToken;
    }
    UnregisterHolographicEventHandlers();
}

// Updates the application state once per frame.
HolographicFrame^ AppMain::Update()
{
    if (!m_holographicSpace)
    {
        return nullptr;
    }

    // Before doing the timer update, there is some work to do per-frame
    // to maintain holographic rendering. First, we will get information
    // about the current frame.

    // The HolographicFrame has information that the app needs in order
    // to update and render the current frame. The app begins each new
    // frame by calling CreateNextFrame.
    HolographicFrame^ holographicFrame = m_holographicSpace->CreateNextFrame();

    // Get a prediction of where holographic cameras will be when this frame
    // is presented.
    HolographicFramePrediction^ prediction = holographicFrame->CurrentPrediction;

    // Back buffers can change from frame to frame. Validate each buffer, and recreate
    // resource views and depth buffers as needed.
    m_deviceResources->EnsureCameraResources(holographicFrame, prediction);

    // Next, we get a coordinate system from the attached frame of reference that is
    // associated with the current frame. Later, this coordinate system is used for
    // for creating the stereo view matrices when rendering the sample content.
    SpatialCoordinateSystem^ currentCoordinateSystem = m_referenceFrame->CoordinateSystem;

    // Check for new input state since the last frame.
    SpatialTappedEventArgs^ tapped = m_spatialInputHandler->CheckForTapped();
    if (tapped)
    {
        // When the Tapped spatial input event is received, the sample hologram will be 
        // repositioned two meters in front of the user.
        m_spinningCubeRenderer->PositionHologram(
            tapped->TryGetPointerPose(currentCoordinateSystem)
            );
    }
    else
    {
        auto manipulationStarted = m_spatialInputHandler->CheckForManipulationStarted();
        static float3 initialCubePosition = float3::zero();
        if (manipulationStarted)
        {
            initialCubePosition = m_spinningCubeRenderer->GetPosition();
            m_spinningCubeRenderer->Pause();
        }
        else
        {
            auto manipulationUpdated = m_spatialInputHandler->CheckForManipulationUpdated();
            if (manipulationUpdated)
            {
                auto delta = manipulationUpdated->TryGetCumulativeDelta(currentCoordinateSystem);
                if (delta)
                {
                    m_spinningCubeRenderer->SetPosition(initialCubePosition + delta->Translation);
                }
            }
            else
            {
                switch (m_spatialInputHandler->CheckForManipulationResult())
                {
                case SpatialInputHandler::ManipulationResult::Canceled:
                    m_spinningCubeRenderer->SetPosition(initialCubePosition);
                case SpatialInputHandler::ManipulationResult::Completed:
                    m_spinningCubeRenderer->Unpause();
                    break;
                }
            }
        }
    }

    m_timer.Tick([&] ()
    {
        m_spinningCubeRenderer->Update(m_timer);
        m_spatialInputRenderer->Update(holographicFrame->CurrentPrediction->Timestamp, currentCoordinateSystem);
    });

    // We complete the frame update by using information about our content positioning
    // to set the focus point.

    for (auto cameraPose : prediction->CameraPoses)
    {
        // The HolographicCameraRenderingParameters class provides access to set
        // the image stabilization parameters.
        HolographicCameraRenderingParameters^ renderingParameters = holographicFrame->GetRenderingParameters(cameraPose);

        // SetFocusPoint informs the system about a specific point in your scene to
        // prioritize for image stabilization. The focus point is set independently
        // for each holographic camera.
        // You should set the focus point near the content that the user is looking at.
        // In this example, we put the focus point at the center of the sample hologram,
        // since that is the only hologram available for the user to focus on.
        // You can also set the relative velocity and facing of that content; the sample
        // hologram is at a fixed point so we only need to indicate its position.
        renderingParameters->SetFocusPoint(
            currentCoordinateSystem,
            m_spinningCubeRenderer->GetPosition()
            );
    }

    // The holographic frame will be used to get up-to-date view and projection matrices and
    // to present the swap chain.
    return holographicFrame;
}

// Renders the current frame to each holographic camera, according to the
// current application and spatial positioning state. Returns true if the
// frame was rendered to at least one camera.
bool AppMain::Render(Windows::Graphics::Holographic::HolographicFrame^ holographicFrame)
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return false;
    }

    // Lock the set of holographic camera resources, then draw to each camera
    // in this frame.
    return m_deviceResources->UseHolographicCameraResources<bool>(
        [this, holographicFrame](std::map<UINT32, std::unique_ptr<DX::CameraResources>>& cameraResourceMap)
    {
        // Up-to-date frame predictions enhance the effectiveness of image stablization and
        // allow more accurate positioning of holograms.
        holographicFrame->UpdateCurrentPrediction();
        HolographicFramePrediction^ prediction = holographicFrame->CurrentPrediction;

        bool atLeastOneCameraRendered = false;
        for (auto cameraPose : prediction->CameraPoses)
        {
            // This represents the device-based resources for a HolographicCamera.
            DX::CameraResources* pCameraResources = cameraResourceMap[cameraPose->HolographicCamera->Id].get();

            // Get the device context.
            const auto context = m_deviceResources->GetD3DDeviceContext();

            // Clear the back buffer view.
            context->ClearRenderTargetView(pCameraResources->GetBackBufferRenderTargetView(), DirectX::Colors::Transparent);

            context->ClearDepthStencilView(pCameraResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

            // The view and projection matrices for each holographic camera will change
            // every frame. This function refreshes the data in the constant buffer for
            // the holographic camera indicated by cameraPose.
            pCameraResources->UpdateViewProjectionBuffer(m_deviceResources, cameraPose, m_referenceFrame->CoordinateSystem);

            // Set up the camera buffer.
            bool cameraActive = pCameraResources->AttachViewProjectionBuffer(m_deviceResources);
            
            // Only render world-locked content when positional tracking is active.
            if (cameraActive)
            {
                // Hologram rendering pass: Draw holographic content.
                context->ClearDepthStencilView(pCameraResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

                // Set the render target, and set the depth target drawing buffer.
                ID3D11RenderTargetView *const targets[1] = { pCameraResources->GetBackBufferRenderTargetView() };
                context->OMSetRenderTargets(1, targets, pCameraResources->GetDepthStencilView());

                // Render the scene objects.
                m_spinningCubeRenderer->Render(pCameraResources->IsRenderingStereoscopic());
                m_spatialInputRenderer->Render(pCameraResources->IsRenderingStereoscopic());
            }

            atLeastOneCameraRendered = true;
        }

        return atLeastOneCameraRendered;
    });
}

void AppMain::SaveAppState()
{
    // This sample persists app state as an immediate response to speech commands.
}

void AppMain::LoadAppState()
{
    // This sample persists app state as an immediate response to speech commands.
}

// Notifies classes that use Direct3D device resources that the device resources
// need to be released before this method returns.
void AppMain::OnDeviceLost()
{
    m_spinningCubeRenderer->ReleaseDeviceDependentResources();
    m_spatialInputRenderer->ReleaseDeviceDependentResources();
}

// Notifies classes that use Direct3D device resources that the device resources
// may now be recreated.
void AppMain::OnDeviceRestored()
{
    m_spinningCubeRenderer->CreateDeviceDependentResources();
    m_spatialInputRenderer->CreateDeviceDependentResources();
}

void AppMain::OnLocatabilityChanged(SpatialLocator^ sender, Object^ args)
{
    switch (sender->Locatability)
    {
    case SpatialLocatability::Unavailable:
        // Holograms cannot be rendered.
        {
            String^ message = L"Warning! Positional tracking is " +
                                        sender->Locatability.ToString() + L".\n";
            OutputDebugStringW(message->Data());
        }
        break;

    // In the following three cases, it is still possible to place holograms using a
    // SpatialLocatorAttachedFrameOfReference.
    case SpatialLocatability::PositionalTrackingActivating:
        // The system is preparing to use positional tracking.

    case SpatialLocatability::OrientationOnly:
        // Positional tracking has not been activated.

    case SpatialLocatability::PositionalTrackingInhibited:
        // Positional tracking is temporarily inhibited. User action may be required
        // in order to restore positional tracking.
        break;

    case SpatialLocatability::PositionalTrackingActive:
        // Positional tracking is active. World-locked content can be rendered.
        break;
    }
}

void AppMain::OnCameraAdded(
    HolographicSpace^ sender,
    HolographicSpaceCameraAddedEventArgs^ args
    )
{
    Deferral^ deferral = args->GetDeferral();
    HolographicCamera^ holographicCamera = args->Camera;
    create_task([this, deferral, holographicCamera] ()
    {
        // Create device-based resources for the holographic camera and add it to the list of
        // cameras used for updates and rendering. Notes:
        //   * Since this function may be called at any time, the AddHolographicCamera function
        //     waits until it can get a lock on the set of holographic camera resources before
        //     adding the new camera. At 60 frames per second this wait should not take long.
        //   * A subsequent Update will take the back buffer from the RenderingParameters of this
        //     camera's CameraPose and use it to create the ID3D11RenderTargetView for this camera.
        //     Content can then be rendered for the HolographicCamera.
        m_deviceResources->AddHolographicCamera(holographicCamera);

        // Holographic frame predictions will not include any information about this camera until
        // the deferral is completed.
        deferral->Complete();
    });
}

void AppMain::OnCameraRemoved(
    HolographicSpace^ sender,
    HolographicSpaceCameraRemovedEventArgs^ args
    )
{
    // Before letting this callback return, ensure that all references to the back buffer 
    // are released.
    // Since this function may be called at any time, the RemoveHolographicCamera function
    // waits until it can get a lock on the set of holographic camera resources before
    // deallocating resources for this camera. At 60 frames per second this wait should
    // not take long.
    m_deviceResources->RemoveHolographicCamera(args->Camera);
}

