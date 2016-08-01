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

#include <collection.h>
#include <ppltasks.h>    // For create_task

namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch Win32 API errors.
            throw Platform::Exception::CreateException(hr);
        }
    }
    
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
    inline Concurrency::task<Windows::Storage::StorageFolder^> GetLocalDirectoryAsync()
    {
        using namespace Windows::Storage;
        using namespace Concurrency;

        static StorageFolder^ s_folder;
        static std::once_flag s_folderCreationFlag;

        return task<StorageFolder^>([]()
        {
            std::call_once(s_folderCreationFlag, []()
            {
                WCHAR folderBuffer[MAX_PATH] = { 0 };
                HMODULE hModule = GetModuleHandle(nullptr);
                if (!GetModuleFileNameW(hModule, folderBuffer, _countof(folderBuffer)))
                {
                    throw Platform::Exception::CreateException(HRESULT_FROM_WIN32(GetLastError()));
                }

                ThrowIfFailed(PathCchRemoveFileSpec(folderBuffer, _countof(folderBuffer)));

                WCHAR canonicalFolderPath[MAX_PATH] = { 0 };
                ThrowIfFailed(PathCchCanonicalize(canonicalFolderPath, _countof(canonicalFolderPath), folderBuffer));

                create_task(
                    StorageFolder::GetFolderFromPathAsync(Platform::StringReference(canonicalFolderPath))).then(
                        [](StorageFolder^ folder)
                {
                    s_folder = folder;
                }).wait();
            });

            return task_from_result(s_folder);
        });
    }
#endif

    // Function that reads from a binary file asynchronously.
    inline Concurrency::task<std::vector<byte>> ReadDataAsync(std::wstring filename)
    {
        using namespace Windows::Storage;
        using namespace Concurrency;
        
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
        return GetLocalDirectoryAsync().then(
            [filename](StorageFolder^ folder)
            {
                return folder->GetFileAsync(Platform::StringReference(filename.c_str()));
            }).then(
            [] (StorageFile^ file)
            {
                return FileIO::ReadBufferAsync(file);
            }).then(
            [] (Streams::IBuffer^ fileBuffer) -> std::vector<byte>
            {
                std::vector<byte> returnBuffer;
                returnBuffer.resize(fileBuffer->Length);
                Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(
                    Platform::ArrayReference<byte>(returnBuffer.data(), 
                        static_cast<unsigned int>(returnBuffer.size())));
                return returnBuffer;
            });
#else
        auto folder = Windows::ApplicationModel::Package::Current->InstalledLocation;
        return create_task(folder->GetFileAsync(Platform::StringReference(filename.c_str()))).then([](StorageFile^ file)
        {
            return FileIO::ReadBufferAsync(file);
        }).then([](Streams::IBuffer^ fileBuffer) -> std::vector<byte>
        {
            std::vector<byte> returnBuffer;
            returnBuffer.resize(fileBuffer->Length);
            Streams::DataReader::FromBuffer(fileBuffer)->ReadBytes(Platform::ArrayReference<byte>(returnBuffer.data(), fileBuffer->Length));
            return returnBuffer;
        });
#endif
    }

    // Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
    inline float ConvertDipsToPixels(float dips, float dpi)
    {
        static const float dipsPerInch = 96.0f;
        return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
    }

#if defined(_DEBUG)
    // Check for SDK Layer support.
    inline bool SdkLayersAvailable()
    {
        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
            0,
            D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
            nullptr,                    // Any feature level will do.
            0,
            D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
            nullptr,                    // No need to keep the D3D device reference.
            nullptr,                    // No need to know the feature level.
            nullptr                     // No need to keep the D3D device context reference.
            );

        return SUCCEEDED(hr);
    }
#endif
}
