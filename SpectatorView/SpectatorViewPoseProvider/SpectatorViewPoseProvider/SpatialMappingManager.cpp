// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "SpatialMappingManager.h"


SpatialMappingManager::SpatialMappingManager()
{
    QueryPerformanceFrequency(&timeFreq);
}


SpatialMappingManager::~SpatialMappingManager()
{
}

void SpatialMappingManager::Reset()
{
    std::lock_guard<std::mutex> lock(m_resetMutex);

    UnregisterHolographicEventHandlers();
    m_surfaceObserver = nullptr;
    ReleaseDeviceDependentResources();
    m_meshRenderer->ClearSurfaces();
    CreateDeviceDependentResources();
}

void SpatialMappingManager::Initialize(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
    m_meshRenderer = std::make_unique<RealtimeSurfaceMeshRenderer>(deviceResources);
}

void SpatialMappingManager::UnregisterHolographicEventHandlers()
{
    if (m_surfaceObserver == nullptr ||
        isSpatialMappingActive == false)
    {
        return;
    }

    OutputDebugString(L"Unregistering surface observer.\n");

    isSpatialMappingActive = false;
    m_surfaceObserver->ObservedSurfacesChanged -= m_surfacesChangedToken;
}

void SpatialMappingManager::StartSurfaceObserver()
{
    if (m_surfaceObserver == nullptr || 
        isSpatialMappingActive)
    {
        return;
    }

    OutputDebugString(L"Registering surface observer.\n");

    m_surfacesChangedToken = m_surfaceObserver->ObservedSurfacesChanged +=
        ref new Windows::Foundation::TypedEventHandler<SpatialSurfaceObserver^, Platform::Object^>(
            std::bind(&SpatialMappingManager::OnSurfacesChanged, this, std::placeholders::_1, std::placeholders::_2)
            );

    QueryPerformanceCounter(&currentTime);
    timeSurfaceObserverRegistered = (currentTime.QuadPart * S2HNS) / timeFreq.QuadPart;

    isSpatialMappingActive = true;
}

byte* SpatialMappingManager::GetMeshData(int& length, SpatialCoordinateSystem^ cs)
{
    std::lock_guard<std::mutex> lock(m_resetMutex);
    if (m_meshRenderer == nullptr)
    {
        return nullptr;
    }

    return m_meshRenderer->SerializeMeshes(length, cs);
}

void SpatialMappingManager::OnSurfacesChanged(
    SpatialSurfaceObserver^ sender,
    Platform::Object^ args)
{
    std::lock_guard<std::mutex> lock(m_resetMutex);
    Windows::Foundation::Collections::IMapView<Platform::Guid, SpatialSurfaceInfo^>^ const& surfaceCollection = sender->GetObservedSurfaces();

    // Process surface adds and updates.
    for (const auto& pair : surfaceCollection)
    {
        auto id = pair->Key;
        auto surfaceInfo = pair->Value;

        if (m_meshRenderer->HasSurface(id))
        {
            if (m_meshRenderer->GetLastUpdateTime(id).UniversalTime < surfaceInfo->UpdateTime.UniversalTime)
            {
                // Update existing surface.
                m_meshRenderer->UpdateSurface(id, surfaceInfo);
            }
        }
        else
        {
            // New surface.
            m_meshRenderer->AddSurface(id, surfaceInfo);
        }
    }

    // Sometimes, a mesh will fall outside the area that is currently visible to
    // the surface observer. In this code sample, we "sleep" any meshes that are
    // not included in the surface collection to avoid rendering them.
    // The system can including them in the collection again later, in which case
    // they will no longer be hidden.
    m_meshRenderer->HideInactiveMeshes(surfaceCollection);
}

void SpatialMappingManager::Update(SpatialCoordinateSystem^ currentCoordinateSystem)
{
    if (prevCoordSystem != nullptr &&
        prevCoordSystem != currentCoordinateSystem)
    {
        // We have a new coordinate system and need to reset all of our surfaces.
        Reset();
    }

    prevCoordSystem = currentCoordinateSystem;

    // Check if we should disable the spatial mapping observer
    if (isSpatialMappingActive)
    {
        QueryPerformanceCounter(&currentTime);
        LONGLONG time = (currentTime.QuadPart * S2HNS) / timeFreq.QuadPart;

        if ((time - timeSurfaceObserverRegistered) > surfaceObserverActiveTime * S2HNS)
        {
            UnregisterHolographicEventHandlers();
        }
    }


    // Only create a surface observer when you need to - do not create a new one each frame.
    if (m_surfaceObserver == nullptr)
    {
        // Initialize the Surface Observer using a valid coordinate system.
        if (!m_spatialPerceptionAccessRequested)
        {
            // The spatial mapping API reads information about the user's environment. The user must
            // grant permission to the app to use this capability of the Windows Holographic device.
            auto initSurfaceObserverTask = create_task(SpatialSurfaceObserver::RequestAccessAsync());
            initSurfaceObserverTask.then([this, currentCoordinateSystem](Windows::Perception::Spatial::SpatialPerceptionAccessStatus status)
            {
                switch (status)
                {
                case SpatialPerceptionAccessStatus::Allowed:
                    m_surfaceAccessAllowed = true;
                    break;
                default:
                    // Access was denied. This usually happens because your AppX manifest file does not declare the
                    // spatialPerception capability.
                    // For info on what else can cause this, see: http://msdn.microsoft.com/library/windows/apps/mt621422.aspx
                    m_surfaceAccessAllowed = false;
                    break;
                }
            });

            m_spatialPerceptionAccessRequested = true;
        }
    }

    if (m_surfaceAccessAllowed)
    {
        SpatialBoundingBox axisAlignedBoundingBox =
        {
            { 0.f,  0.f, 0.f },
            { 20.f, 20.f, 5.f },
        };
        SpatialBoundingVolume^ bounds = SpatialBoundingVolume::FromBox(currentCoordinateSystem, axisAlignedBoundingBox);

        // If status is Allowed, we can create the surface observer.
        if (m_surfaceObserver == nullptr)
        {
            // First, we'll set up the surface observer to use our preferred data formats.
            // In this example, a "preferred" format is chosen that is compatible with our precompiled shader pipeline.
            m_surfaceMeshOptions = ref new SpatialSurfaceMeshOptions();
            Windows::Foundation::Collections::IVectorView<DirectXPixelFormat>^ supportedVertexPositionFormats = m_surfaceMeshOptions->SupportedVertexPositionFormats;
            unsigned int formatIndex = 0;
            if (supportedVertexPositionFormats->IndexOf(DirectXPixelFormat::R16G16B16A16IntNormalized, &formatIndex))
            {
                m_surfaceMeshOptions->VertexPositionFormat = DirectXPixelFormat::R16G16B16A16IntNormalized;
            }
            Windows::Foundation::Collections::IVectorView<DirectXPixelFormat>^ supportedVertexNormalFormats = m_surfaceMeshOptions->SupportedVertexNormalFormats;
            if (supportedVertexNormalFormats->IndexOf(DirectXPixelFormat::R8G8B8A8IntNormalized, &formatIndex))
            {
                m_surfaceMeshOptions->VertexNormalFormat = DirectXPixelFormat::R8G8B8A8IntNormalized;
            }

            // Create the observer.
            m_surfaceObserver = ref new SpatialSurfaceObserver();
            if (m_surfaceObserver)
            {
                m_surfaceObserver->SetBoundingVolume(bounds);

                // If the surface observer was successfully created, we can initialize our
                // collection by pulling the current data set.
                auto mapContainingSurfaceCollection = m_surfaceObserver->GetObservedSurfaces();
                for (auto const& pair : mapContainingSurfaceCollection)
                {
                    // Store the ID and metadata for each surface.
                    auto const& id = pair->Key;
                    auto const& surfaceInfo = pair->Value;
                    m_meshRenderer->AddSurface(id, surfaceInfo);
                }

                // We then subcribe to an event to receive up-to-date data.
                void StartSurfaceObserver();
            }
        }

        // Keep the surface observer positioned at the device's location.
        m_surfaceObserver->SetBoundingVolume(bounds);
    }
}

void SpatialMappingManager::UpdateMeshRenderer(DX::StepTimer const & timer, SpatialCoordinateSystem^ coordinateSystem)
{
    m_meshRenderer->Update(timer, coordinateSystem);
}

void SpatialMappingManager::Render(bool stereoscopic)
{
    std::lock_guard<std::mutex> lock(m_resetMutex);
    m_meshRenderer->Render(stereoscopic, m_drawWireframe);
}

void SpatialMappingManager::CreateDeviceDependentResources()
{
    m_meshRenderer->CreateDeviceDependentResources();
}

void SpatialMappingManager::ReleaseDeviceDependentResources()
{
    m_meshRenderer->ReleaseDeviceDependentResources();
}


