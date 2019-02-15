// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "UnityDeviceResource.h"
#include "PlatformBase.h"

std::shared_ptr<IUnityDeviceResource> CreateDeviceResource(UnityGfxRenderer apiType)
{
#   if SUPPORT_D3D11
    if (apiType == kUnityGfxRendererD3D11)
    {
        extern std::shared_ptr<IUnityDeviceResource> __stdcall CreateD3D11DeviceResource();
        return CreateD3D11DeviceResource();
    }
#   endif // if SUPPORT_D3D11

#   if SUPPORT_D3D12
    if (apiType == kUnityGfxRendererD3D12)
    {
        extern std::shared_ptr<IUnityDeviceResource> __stdcall CreateD3D12DeviceResources();
        return CreateD3D12DeviceResources();
    }
#   endif // if SUPPORT_D3D12

# if SUPPORT_OPENGL_UNIFIED
    if (apiType == kUnityGfxRendererOpenGLCore || apiType == kUnityGfxRendererOpenGLES20 || apiType == kUnityGfxRendererOpenGLES30)
    {
        //        extern std::shared_ptr<IUnityDeviceResource> __stdcall CreateOGLESDeviceResource(UnityGfxRenderer apiType);
        //        return CreateOGLESDeviceResource(apiType);
    }
# endif // if SUPPORT_OPENGL_UNIFIED

# if SUPPORT_METAL
    if (apiType == kUnityGfxRendererMetal)
    {
        //        extern std::shared_ptr<IUnityDeviceResource> __stdcall CreateMetalDeviceResource();
        //        return CreateMetalDeviceResource();
    }
# endif // if SUPPORT_METAL

    // Unknown or unsupported graphics API
    return NULL;
}