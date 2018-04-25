// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
// https://docs.microsoft.com/en-us/windows/mixed-reality/spatial-mapping-in-directx
// https://github.com/Microsoft/Windows-universal-samples/tree/master/Samples/HolographicSpatialMapping

#include "RealtimeSurfaceMeshRenderer.h"
#include <collection.h>
#include <ppltasks.h>
#include "CompositorConstants.h"

using namespace concurrency;
using namespace Windows::Graphics::DirectX;
using namespace Windows::Graphics::Holographic;
using namespace Windows::Perception::Spatial;
using namespace Windows::Perception::Spatial::Surfaces;

class SpatialMappingManager
{
private:
    // Handle surface change events.
    void OnSurfacesChanged(Windows::Perception::Spatial::Surfaces::SpatialSurfaceObserver^ sender, Platform::Object^ args);
    void RegisterSurfaceEventHandler();

    void Reset();

    SpatialCoordinateSystem^ prevCoordSystem = nullptr;

    // A data handler for surface meshes.
    std::unique_ptr<RealtimeSurfaceMeshRenderer> m_meshRenderer;

    // Obtains spatial mapping data from the device in real time.
    Windows::Perception::Spatial::Surfaces::SpatialSurfaceObserver^     m_surfaceObserver;
    Windows::Perception::Spatial::Surfaces::SpatialSurfaceMeshOptions^  m_surfaceMeshOptions;

    // Determines the rendering mode.
    bool                                                                m_drawWireframe = true;

    Windows::Foundation::EventRegistrationToken                         m_surfacesChangedToken;

    // Indicates whether access to spatial mapping data has been granted.
    bool                                                                m_surfaceAccessAllowed = false;

    // Indicates whether the surface observer initialization process was started.
    bool                                                                m_spatialPerceptionAccessRequested = false;

    std::mutex m_resetMutex;

    LARGE_INTEGER timeFreq;
    LARGE_INTEGER currentTime;
    LONGLONG timeSurfaceObserverRegistered;
    const int surfaceObserverActiveTime = 10;

    bool isSpatialMappingActive = false;

public:
    SpatialMappingManager();
    ~SpatialMappingManager();

    void Initialize(const std::shared_ptr<DX::DeviceResources>& deviceResources);
    void UnregisterHolographicEventHandlers();

    void Update(SpatialCoordinateSystem^ currentCoordinateSystem);
    void UpdateMeshRenderer(DX::StepTimer const& timer, SpatialCoordinateSystem^ coordinateSystem);

    void Render(bool stereoscopic);

    void CreateDeviceDependentResources();
    void ReleaseDeviceDependentResources();

    byte* GetMeshData(int& length, SpatialCoordinateSystem^ cs);
};

