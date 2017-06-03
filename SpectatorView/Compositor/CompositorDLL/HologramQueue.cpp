// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "stdafx.h"
#include "HologramQueue.h"

// These methods must all be called from the same thread.
// Otherwise, a lock will need to be added which will substantially slow down the render thread.
HologramQueue::HologramQueue()
{
    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        m_holographicFrameQueue[i].timeStamp = INVALID_TIMESTAMP;
        m_holographicFrameQueue[i].m_id = i;
    }
}

FrameMessage* HologramQueue::GetNextFrame(LONGLONG timeStamp)
{
    auto curIndex = m_nextIndex;
    m_nextIndex = (m_nextIndex + 1) % MAX_QUEUE_SIZE;

    auto nextFrame = &m_holographicFrameQueue[curIndex];
    nextFrame->timeStamp = timeStamp;

    return nextFrame;
}

FrameMessage* HologramQueue::FindClosestFrame(LONGLONG timeStamp, LONGLONG frameOffset)
{
    FrameMessage* closestFrameMessage = NULL;
    LONGLONG smallestDelta = std::numeric_limits<LONGLONG>::max();

    for (int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        LONGLONG frameTime = m_holographicFrameQueue[i].timeStamp;
        LONGLONG delta = timeStamp - frameTime - frameOffset;

        if (delta >= 0 && delta < smallestDelta)
        {
            smallestDelta = delta;
            closestFrameMessage = &m_holographicFrameQueue[i];
        }
    }

    if (closestFrameMessage == nullptr)
    {
        // Didn't find a match, give the last known frame
        auto lastIndex = (((m_nextIndex - 1) % MAX_QUEUE_SIZE) + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
        closestFrameMessage = &m_holographicFrameQueue[lastIndex];
    }

    return closestFrameMessage;
}
