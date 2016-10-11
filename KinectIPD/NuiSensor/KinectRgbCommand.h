//*******************************************************************
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*******************************************************************
#pragma once

#include <array>

namespace Xbox
{
namespace Kinect
{
    class KinectRgbCommandList
    {
    public:
        KinectRgbCommandList();
        ~KinectRgbCommandList();

        void Clear();

        HRESULT Add(::NUISENSOR_RGB_COMMAND_TYPE command);
        HRESULT AddFloat(::NUISENSOR_RGB_COMMAND_TYPE command, float argument);
        HRESULT AddInt(::NUISENSOR_RGB_COMMAND_TYPE command, UINT32 argument);

        HRESULT Submit(::NUISENSOR_HANDLE sensorHandle);
        HRESULT SubmitAsync(::NUISENSOR_HANDLE sensorHandle, OVERLAPPED* pOverlapped);

        BOOL IsPendingCompletion();
        void Complete();

        const ::NUISENSOR_RGB_CHANGE_STREAM_SETTING* GetSettings();
        const ::NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY* GetReplies();

    private:
        std::array<BYTE, NUISENSOR_SEND_SCRATCH_SPACE_REQUIRED> _scratchBuffer;
        std::array<BYTE, NUISENSOR_MAX_USB_COMMAND_SIZE> _settingBuffer;
        std::array<BYTE, NUISENSOR_MAX_USB_RESPONSE_SIZE> _replyBuffer;

        ::NUISENSOR_RGB_CHANGE_STREAM_SETTING* _settings;
        ::NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY* _replies;

        BOOL _isPendingCompletion;

        static UINT32 _sequenceId;
    };

} // Kinect
} // Xbox
