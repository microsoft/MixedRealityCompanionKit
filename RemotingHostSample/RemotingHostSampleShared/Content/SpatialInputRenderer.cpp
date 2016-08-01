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
#include "SpatialInputRenderer.h"
#include "..\Common\DirectXHelper.h"

using namespace Concurrency;
using namespace RemotingHostSample;
using namespace Microsoft::WRL;
using namespace Windows::Foundation::Numerics;
using namespace Windows::Perception;
using namespace Windows::Perception::Spatial;
using namespace Windows::UI::Input::Spatial;

SpatialInputRenderer::SpatialInputRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
    RenderableObject(deviceResources),
    m_manager(Windows::UI::Input::Spatial::SpatialInteractionManager::GetForCurrentView()),
    m_referenceFrame(SpatialLocator::GetDefault()->CreateAttachedFrameOfReferenceAtCurrentHeading())
{}

void SpatialInputRenderer::Update(PerceptionTimestamp^ timestamp,
                                  SpatialCoordinateSystem^ renderingCoordinateSystem)
{
    m_positions.clear();

    auto headingAdjustment = m_referenceFrame->TryGetRelativeHeadingAtTimestamp(timestamp);
    if (headingAdjustment)
    {
        // keep coordinate systems facing user
        m_referenceFrame->AdjustHeading(-headingAdjustment->Value);
        auto coordinateSystem = m_referenceFrame->GetStationaryCoordinateSystemAtTimestamp(timestamp);

        auto states = m_manager->GetDetectedSourcesAtTimestamp(timestamp);

        m_positions.reserve(states->Size);
        for (const auto& state : states)
        {
            auto location = state->Properties->TryGetLocation(coordinateSystem);
            if (location && location->Position)
            {
                m_positions.emplace_back(location->Position->Value);
            }
        }

        auto modelTransform = coordinateSystem->TryGetTransformTo(renderingCoordinateSystem);
        if (modelTransform)
        {
            UpdateModelConstantBuffer(modelTransform->Value);
        }
    }
}

void SpatialInputRenderer::Draw(unsigned int numInstances)
{
    if (!m_positions.empty())
    {
        std::vector<VertexPositionNormalColor> vertices;
        for (const auto& position : m_positions)
        {
            static const auto height = float3::unit_y() * 0.03f;
            static const auto width = float3::unit_x() * 0.02f;

            VertexPositionNormalColor vertex
            {
                reinterpret_cast<const DirectX::XMFLOAT3&>(position + height),
                DirectX::XMFLOAT3{ 0, 0, 1 }, // straight back towards the user
                DirectX::XMFLOAT3{ 0, 0, 1 }, // blue (but lighting often makes it appear white)
            };

            // tip, elevated above reported position
            vertices.emplace_back(vertex);

            // base, centered on reported position
            float3& vertexPos = reinterpret_cast<float3&>(vertex.pos);
            vertexPos = position;
            vertexPos += (width / 2);
            vertices.emplace_back(vertex);

            vertexPos -= width;
            vertices.emplace_back(vertex);
        }

        const UINT stride = sizeof(vertices[0]);
        const UINT offset = 0;
        D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
        vertexBufferData.pSysMem = vertices.data();
        const CD3D11_BUFFER_DESC vertexBufferDesc(static_cast<UINT>(vertices.size() * stride), D3D11_BIND_VERTEX_BUFFER);
        ComPtr<ID3D11Buffer> vertexBuffer;
        DX::ThrowIfFailed(
            m_deviceResources->GetD3DDevice()->CreateBuffer(
                &vertexBufferDesc, &vertexBufferData, &vertexBuffer));

        auto context = m_deviceResources->GetD3DDeviceContext();
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
        context->DrawInstanced(static_cast<UINT>(vertices.size()), numInstances, offset, 0);
    }
}
