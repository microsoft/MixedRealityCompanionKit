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
#include "RenderableObject.h"
#include <vector>

namespace RemotingHostSample
{
    class SpatialInputRenderer : public RenderableObject
    {
    public:
        SpatialInputRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);

        void Update(Windows::Perception::PerceptionTimestamp^ timestamp,
                    Windows::Perception::Spatial::SpatialCoordinateSystem^ renderingCoordinateSystem);

    private:
        void Draw(unsigned int numInstances) override;

        Windows::UI::Input::Spatial::SpatialInteractionManager^                 m_manager;
        Windows::Perception::Spatial::SpatialLocatorAttachedFrameOfReference^   m_referenceFrame;
        std::vector<Windows::Foundation::Numerics::float3>                      m_positions;
    };
}
