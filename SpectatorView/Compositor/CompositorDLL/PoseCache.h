// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

#define MAX_NUM_POSES 60

class PoseData
{
public:
    XMFLOAT3 Position;
    XMFLOAT4 Rotation;
    float TimeStamp;
    int Index;

    PoseData(XMFLOAT3 Position, XMFLOAT4 Rotation, float TimeStamp, int Index) :
        Position(Position),
        Rotation(Rotation),
        TimeStamp(TimeStamp),
        Index(Index)
    {
    }
};

class PoseCache
{
public:
    PoseCache()
    {
        poses.reserve(MAX_NUM_POSES + 1);
    }

    ~PoseCache()
    {
        poses.clear();
    }

    int LastSelectedIndex;

    bool AddPose(XMFLOAT3 position, XMFLOAT4 rotation, float timeStamp)
    {
        // Already have this pose.
        if (poses.size() > 0 && poses[0].TimeStamp == timeStamp)
        {
            return false;
        }

        // Find index to insert at.
        int i = 0;
        while (i < poses.size() && poses[i].TimeStamp > timeStamp)
        {
            i++;
        }

        poses.insert(poses.begin() + i, PoseData(position, rotation, timeStamp, lastPoseIndex++));

        // Remove oldest.
        if (poses.size() >= MAX_NUM_POSES)
        {
            poses.erase(poses.begin() + MAX_NUM_POSES - 1);
        }
        return true;
    }

    bool GetPose(XMFLOAT3& position, XMFLOAT4& rotation, float poseTime)
    {
        if (poses.size() == 0)
        {
            position = XMFLOAT3(0, 0, 0);
            rotation = XMFLOAT4(0, 0, 0, 1);
            return false;
        }

        //Find index for time
        LastSelectedIndex = 0;
        while (LastSelectedIndex < poses.size())
        {
            if (poses[LastSelectedIndex].TimeStamp < poseTime)
            {
                break;
            }

            LastSelectedIndex++;
        }

        if (LastSelectedIndex == 0)
        {
            position = poses[0].Position;
            rotation = poses[0].Rotation;
        }
        else if (LastSelectedIndex == poses.size())
        {
            position = poses[poses.size() - 1].Position;
            rotation = poses[poses.size() - 1].Rotation;
        }
        else
        {
            //Lerp between 2 poses
            PoseData next = poses[LastSelectedIndex];
            PoseData prev = poses[LastSelectedIndex - 1];

            float lerpVal = (poseTime - prev.TimeStamp) / (next.TimeStamp - prev.TimeStamp);
            if (lerpVal > 1) { lerpVal = 1; }
            if (lerpVal < 0) { lerpVal = 0; }

            XMStoreFloat3(&position, 
                XMVectorLerp(XMLoadFloat3(&prev.Position), XMLoadFloat3(&next.Position), lerpVal)
            );

            XMStoreFloat4(&rotation,
                XMQuaternionSlerp(XMLoadFloat4(&prev.Rotation), XMLoadFloat4(&next.Rotation), lerpVal)
            );
        }

        return true;
    }

    PoseData* GetLatestPose()
    {
        return (poses.size() > 0) ? &poses.at(0) : nullptr;
    }

    void Reset()
    {
        lastPoseIndex = 0;
        poses.clear();
    }

private:
    int lastPoseIndex = 0;
    std::vector<PoseData> poses;
};

