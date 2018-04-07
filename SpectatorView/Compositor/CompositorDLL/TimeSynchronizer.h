// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "CompositorConstants.h"

class TimeSynchronizer
{
public:
    TimeSynchronizer()
    {
    }

    ~TimeSynchronizer()
    {
    }

    void Update(int camFrame, float camTime, int poseIndex, float poseTime, float UnityTime)
    {
        if (camFrame != prevCamFrame)
        {
            prevCamFrame = camFrame;
            float newDelta = UnityTime - camTime;
            numCamFrameSamples++;

            deltaCameraToUnity = Lerp(deltaCameraToUnity, newDelta, 1.0f / Min((float)numCamFrameSamples, 60.0f));
        }
        if (poseIndex != prevPoseIndex)
        {
            prevPoseIndex = poseIndex;
            float newDelta = UnityTime - poseTime;
            numPoseIndexSamples++;
            deltaPoseToUnity = Lerp(deltaPoseToUnity, newDelta, 1.0f / Min((float)numPoseIndexSamples, 60.0f));
        }
    }

    float GetPoseTimeFromCameraTime(float cameraTime)
    {
        return (cameraTime + deltaCameraToUnity) - deltaPoseToUnity;
    }

    void Reset()
    {
        prevCamFrame = -1;
        numCamFrameSamples = 0;
        prevPoseIndex = -1;
        numPoseIndexSamples = 0;
    }

private:
    float deltaCameraToUnity;
    float deltaPoseToUnity;

    int prevCamFrame = -1;
    int numCamFrameSamples = 0;
    int prevPoseIndex = -1;
    int numPoseIndexSamples = 0;

    float Lerp(float a, float b, float t)
    {
        return (1 - t) * a + t * b;
    }

    float Min(float a, float b)
    {
        return (a < b) ? a : b;
    }
};

