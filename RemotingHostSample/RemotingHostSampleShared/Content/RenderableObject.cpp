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
#include "RenderableObject.h"
#include "..\Common\DirectXHelper.h"

using namespace Concurrency;
using namespace RemotingHostSample;
using namespace Microsoft::WRL;
using namespace Windows::Foundation::Numerics;
using namespace Windows::Perception;
using namespace Windows::Perception::Spatial;

RenderableObject::RenderableObject(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
    m_deviceResources(deviceResources)
{
    CreateDeviceDependentResources();
}

void RenderableObject::UpdateModelConstantBuffer(const float4x4& modelTransform)
{
    float4x4 normalTransform = modelTransform;
    normalTransform.m41 = normalTransform.m42 = normalTransform.m43 = 0;
    UpdateModelConstantBuffer(modelTransform, normalTransform);
}

void RenderableObject::UpdateModelConstantBuffer(const float4x4& modelTransform,
                                                 const float4x4& normalTransform)
{
    if (m_loadingComplete)
    {
        m_modelConstantBufferData.model = reinterpret_cast<DirectX::XMFLOAT4X4&>(transpose(modelTransform));
        m_modelConstantBufferData.normal = reinterpret_cast<DirectX::XMFLOAT4X4&>(transpose(normalTransform));

        // Use the D3D device context to update Direct3D device-based resources.
        const auto context = m_deviceResources->GetD3DDeviceContext();

        // Update the model transform buffer for the hologram.
        context->UpdateSubresource(
            m_modelConstantBuffer.Get(),
            0,
            nullptr,
            &m_modelConstantBufferData,
            0,
            0
            );
    }
}

void RenderableObject::Render(bool isStereo)
{
    if (!m_loadingComplete)
    {
        return;
    }

    // Use the D3D device context to update Direct3D device-based resources.
    const auto context = m_deviceResources->GetD3DDeviceContext();

    context->IASetInputLayout(m_inputLayout.Get());
    context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // Attach the vertex shader.
    context->VSSetShader(
        m_vertexShader.Get(),
        nullptr,
        0
        );

    // Apply the model constant buffer to the vertex shader.
    context->VSSetConstantBuffers(
        0,
        1,
        m_modelConstantBuffer.GetAddressOf()
        );

    if (!m_usingVprtShaders)
    {
        // On devices that do not support the D3D11_FEATURE_D3D11_OPTIONS3::
        // VPAndRTArrayIndexFromAnyShaderFeedingRasterizer optional feature,
        // a pass-through geometry shader is used to set the render target 
        // array index.
        context->GSSetShader(
            m_geometryShader.Get(),
            nullptr,
            0
            );
    }

    Draw(isStereo ? 2 : 1);
}

void RenderableObject::CreateDeviceDependentResources()
{
    m_usingVprtShaders = m_deviceResources->GetDeviceSupportsVprt();

    // On devices that do support the D3D11_FEATURE_D3D11_OPTIONS3::
    // VPAndRTArrayIndexFromAnyShaderFeedingRasterizer optional feature
    // we can avoid using a pass-through geometry shader to set the render
    // target array index, thus avoiding any overhead that would be 
    // incurred by setting the geometry shader stage.
    std::wstring vertexShaderFileName = m_usingVprtShaders ? L"VprtVertexShader.cso" : L"VertexShader.cso";

    // Load shaders asynchronously.
    task<std::vector<byte>> loadVSTask = DX::ReadDataAsync(vertexShaderFileName);
    task<std::vector<byte>> loadPSTask = DX::ReadDataAsync(L"PixelShader.cso");

    task<std::vector<byte>> loadGSTask;
    if (!m_usingVprtShaders)
    {
        // Load the pass-through geometry shader.
        loadGSTask = DX::ReadDataAsync(L"GeometryShader.cso");
    }

    // After the vertex shader file is loaded, create the shader and input layout.
    task<void> createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData)
    {
        DX::ThrowIfFailed(
            m_deviceResources->GetD3DDevice()->CreateVertexShader(
                &fileData[0],
                fileData.size(),
                nullptr,
                &m_vertexShader
                )
            );

        static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        DX::ThrowIfFailed(
            m_deviceResources->GetD3DDevice()->CreateInputLayout(
                vertexDesc,
                ARRAYSIZE(vertexDesc),
                &fileData[0],
                fileData.size(),
                &m_inputLayout
                )
            );
    });

    // After the pixel shader file is loaded, create the shader and constant buffer.
    task<void> createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData)
    {
        DX::ThrowIfFailed(
            m_deviceResources->GetD3DDevice()->CreatePixelShader(
                &fileData[0],
                fileData.size(),
                nullptr,
                &m_pixelShader
                )
            );

        const ModelConstantBuffer constantBuffer
        {
            reinterpret_cast<DirectX::XMFLOAT4X4&>(float4x4::identity()),
            reinterpret_cast<DirectX::XMFLOAT4X4&>(float4x4::identity()),
        };
        D3D11_SUBRESOURCE_DATA constantBufferData = { &constantBuffer, 0, 0 };
        const CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
        DX::ThrowIfFailed(
            m_deviceResources->GetD3DDevice()->CreateBuffer(
                &constantBufferDesc,
                &constantBufferData,
                &m_modelConstantBuffer
                )
            );
    });

    task<void> createGSTask;
    if (!m_usingVprtShaders)
    {
        // After the pass-through geometry shader file is loaded, create the shader.
        createGSTask = loadGSTask.then([this](const std::vector<byte>& fileData)
        {
            DX::ThrowIfFailed(
                m_deviceResources->GetD3DDevice()->CreateGeometryShader(
                    &fileData[0],
                    fileData.size(),
                    nullptr,
                    &m_geometryShader
                    )
                );
        });
    }

    task<void> shaderTaskGroup = m_usingVprtShaders ? (createPSTask && createVSTask) : (createPSTask && createVSTask && createGSTask);
    shaderTaskGroup.then([this]() { m_loadingComplete = true; });
}

void RenderableObject::ReleaseDeviceDependentResources()
{
    m_loadingComplete = false;
    m_usingVprtShaders = false;
    m_vertexShader.Reset();
    m_inputLayout.Reset();
    m_pixelShader.Reset();
    m_geometryShader.Reset();
    m_modelConstantBuffer.Reset();
}
