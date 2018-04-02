// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"

using namespace Windows::Perception;
using namespace Windows::Perception::Spatial;

static SpatialCoordinateSystem^ unitySpatialCoordinateSystem = nullptr;
static Windows::Globalization::Calendar^ calendar = nullptr;

// Pass in WorldManager.GetNativeISpatialCoordinateSystemPtr()
EXTERN_C __declspec(dllexport) bool GetHeadTransform(IUnknown* coordinateSystem, int nanoSecondsPast, 
    float& rotX, float& rotY, float& rotZ, float& rotW, 
    float& posX, float& posY, float& posZ)
{
    try
    {
        if (unitySpatialCoordinateSystem == nullptr)
        {
            if (coordinateSystem == nullptr)
            {
                return false;
            }

            ComPtr<IInspectable> spInspectable;
            if (SUCCEEDED(coordinateSystem->QueryInterface(IID_PPV_ARGS(&spInspectable))))
            {
                unitySpatialCoordinateSystem = safe_cast<SpatialCoordinateSystem^>(reinterpret_cast<Platform::Object^>(spInspectable.Get()));
            }

            if (unitySpatialCoordinateSystem == nullptr)
            {
                return false;
            }
        }

        // Get a timestamp to send to the perception APIs adjusted for the purported latency between sample time and now.
        if (calendar == nullptr)
        {
            calendar = ref new Windows::Globalization::Calendar();

            if (calendar == nullptr)
            {
                return false;
            }
        }

        calendar->SetToNow();
        if (nanoSecondsPast > 0)
        {
            nanoSecondsPast *= -1;
        }
        calendar->AddNanoseconds((int)nanoSecondsPast);

        // Get a timestamp to send to the perception APIs adjusted for the purported latency between sample time and now.
        PerceptionTimestamp^ perceptionTimestamp = PerceptionTimestampHelper::FromHistoricalTargetTime(calendar->GetDateTime());
        SpatialLocation^ headPose = nullptr;

        if (perceptionTimestamp != nullptr)
        {
            TimeSpan predictionAmount = perceptionTimestamp->PredictionAmount;

            // When running on a HoloLens, the default spatial locater represents the device.
            SpatialLocator^ locator = SpatialLocator::GetDefault();
            if (locator == nullptr)
            {
                return false;
            }

            headPose = locator->TryLocateAtTimestamp(perceptionTimestamp, unitySpatialCoordinateSystem);
        }

        if (headPose != nullptr)
        {
            // Convert position and rotation to Unity space.
            Windows::Foundation::Numerics::quaternion rot = headPose->Orientation;
            rotX = -rot.x;
            rotY = -rot.y;
            rotZ = rot.z;
            rotW = rot.w;

            Windows::Foundation::Numerics::float3 pos = headPose->Position;
            posX = pos.x;
            posY = pos.y;
            posZ = -pos.z;
        }

        perceptionTimestamp = nullptr;
        headPose = nullptr;
    }
    catch (...) {}

    return true;
}

