// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#define CHESS_SQUARE_SIZE 0.0677 // Width of the chess squares in meters = 2.66 inches.
// Note: must have > 3 columns and rows.
#define GRID_CELLS_X 6 // Number of columns in the chess board.
#define GRID_CELLS_Y 4 // Number of rows in the chess board.

// NOTE: If you change this to the HoloLens IP, you will need to:
//       1. Under the Security tab in the developer portal, disable SSL connection.
//       2. Use http://%DEVICE_IP%/ - do not use https://
#define HOLOLENS_ADDRESS L"http://127.0.0.1:10080/"
#define HOLOLENS_USER L"user"
#define HOLOLENS_PW L"password"

// Color feed from Hololens.
#define HOLO_WIDTH 1408
#define HOLO_HEIGHT 792
#define HOLO_BPP 4     // RGBA
#define HOLO_BUFSIZE (HOLO_WIDTH * HOLO_HEIGHT * HOLO_BPP)

#define CALIBRATION_FREQUENCY_SECONDS 3

// Known DSLR intrinsics (enable only if you can provide all these values)
#define DSLR_USE_KNOWN_INTRINSICS FALSE
#define DSLR_FOCAL_LENGTH 0.014
#define DSLR_MATRIX_WIDTH 0.036
#define DSLR_MATRIX_HEIGHT 0.024
// Set to true if you want calibration app to avoid adjustment of
// supplied focal length and/or matrix dimensions
#define DSLR_FIX_FOCAL_LENGTH FALSE
// Set to true if you are sure that optical axis of your lens
// intersects with matrix at its center
#define DSLR_FIX_PRINCIPAL_POINT FALSE

// Use the C++ standard templated min/max
#define NOMINMAX

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include "Keyboard.h"
#include <ScreenGrab.h>
#include <wincodec.h>

#include <string>

#include "CompositorShared.h"

namespace Calibration
{
    namespace DX
    {
        // Helper class for COM exceptions
        class com_exception : public std::exception
        {
        public:
            com_exception(HRESULT hr) : result(hr) {}

            virtual const char* what() const override
            {
                static char s_str[64] = { 0 };
                sprintf_s(s_str, "Failure with HRESULT of %08X", result);
                return s_str;
            }

        private:
            HRESULT result;
        };

        // Helper utility converts D3D API failures into exceptions.
        inline void ThrowIfFailed(HRESULT hr)
        {
            if (FAILED(hr))
            {
                throw com_exception(hr);
            }
        }

        // Safe release for interfaces
        template<class Interface>
        inline void SafeRelease(Interface *& pInterfaceToRelease)
        {
            if (pInterfaceToRelease != NULL)
            {
                pInterfaceToRelease->Release();
                pInterfaceToRelease = NULL;
            }
        }
    }
}

