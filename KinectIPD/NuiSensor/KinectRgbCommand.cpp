//*******************************************************************
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*******************************************************************
#include "Stdafx.h"

#include "KinectRgbCommand.h"

#define NUISENSOR_DISABLE_IOCP(x)               ((HANDLE)((UINT_PTR)(x) | 0x0001))

#define GET_NUISENSOR_RGB_CHANGE_STREAM_SETTING_BYTE_COUNT(Setting) \
    ( \
    sizeof(NUISENSOR_RGB_CHANGE_STREAM_SETTING) \
    + ( (Setting).NumCommands - 1) * sizeof(NUISENSOR_RGB_CHANGE_SETTING_CMD) \
    )

#define GET_NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY_BYTE_COUNT(Reply) \
    ( \
    sizeof(NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY) \
    + ( (Reply).NumStatus - 1) * sizeof(NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY_STATUS) \
    )

using namespace Xbox::Kinect;

//
// KinectRgbCommandList
//

UINT32 KinectRgbCommandList::_sequenceId = 0;

KinectRgbCommandList::KinectRgbCommandList()
    : _settings(nullptr)
    , _replies(nullptr)
    , _isPendingCompletion(FALSE)
{
    _settings = reinterpret_cast<NUISENSOR_RGB_CHANGE_STREAM_SETTING*>(_settingBuffer.data());
    _replies = reinterpret_cast<NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY*>(_replyBuffer.data());

    Clear();
}

KinectRgbCommandList::~KinectRgbCommandList()
{
    _settings = nullptr;
    _replies = nullptr;
}

void KinectRgbCommandList::Clear()
{
    _isPendingCompletion = FALSE;

    _settings->NumCommands = 0;
    _settings->SequenceId = InterlockedIncrement(&_sequenceId);

    _replies->NumStatus = 0;
    _replies->CommandListStatus = 0;
}

HRESULT KinectRgbCommandList::Add(NUISENSOR_RGB_COMMAND_TYPE command)
{
    UINT32 argument = 0;
    return AddInt(command, argument);
}

HRESULT KinectRgbCommandList::AddFloat(NUISENSOR_RGB_COMMAND_TYPE command, float argument)
{
    union { float f; UINT32 i; } floatToInt;
    floatToInt.f = argument;
    return AddInt(command, floatToInt.i);
}

HRESULT KinectRgbCommandList::AddInt(NUISENSOR_RGB_COMMAND_TYPE command, UINT32 argument)
{
    if (_isPendingCompletion) return E_FAIL;
    if (_settings->NumCommands >= NUISENSOR_RGB_CHANGE_SETTING_MAX_NUM_CMD) return E_FAIL;

    _settings->Commands[_settings->NumCommands].Cmd = command;
    _settings->Commands[_settings->NumCommands].Arg = argument;
    _settings->NumCommands++;

    _replies->Status[_replies->NumStatus].Status = 0;
    _replies->Status[_replies->NumStatus].Data = 0;
    _replies->NumStatus++;

    return S_OK;
}

HRESULT KinectRgbCommandList::Submit(NUISENSOR_HANDLE sensorHandle)
{
    if (_isPendingCompletion) return E_FAIL;

    HRESULT hr = S_OK;
    OVERLAPPED overlapped = {0};

    overlapped.hEvent = NUISENSOR_DISABLE_IOCP(CreateEvent(NULL, FALSE, FALSE, NULL));

    UINT32 settingsSize = GET_NUISENSOR_RGB_CHANGE_STREAM_SETTING_BYTE_COUNT(*_settings);
    UINT32 repliesSize = GET_NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY_BYTE_COUNT(*_replies);

    BOOL success = NuiSensor_ColorChangeCameraSettings(
        sensorHandle,
        _scratchBuffer.data(),
        (DWORD)_scratchBuffer.size(),
        _settings,
        settingsSize,
        _replies,
        repliesSize,
        &overlapped);

    if (!success && GetLastError() == ERROR_IO_PENDING)
    {
        DWORD bytesTransferred;
        success = GetOverlappedResult(
            sensorHandle,
            &overlapped,
            &bytesTransferred,
            TRUE);
    }

    if (!success)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }

    CloseHandle(overlapped.hEvent);

    return hr;
}

HRESULT KinectRgbCommandList::SubmitAsync(NUISENSOR_HANDLE sensorHandle, OVERLAPPED* pOverlapped)
{
    if (_isPendingCompletion) return E_FAIL;

    UINT32 settingsSize = GET_NUISENSOR_RGB_CHANGE_STREAM_SETTING_BYTE_COUNT(*_settings);
    UINT32 repliesSize = GET_NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY_BYTE_COUNT(*_replies);

    BOOL success = NuiSensor_ColorChangeCameraSettings(
        sensorHandle,
        _scratchBuffer.data(),
        (DWORD)_scratchBuffer.size(),
        _settings,
        settingsSize,
        _replies,
        repliesSize,
        pOverlapped);

    if (!success)
    {
        DWORD err = GetLastError();
        if (err != ERROR_IO_PENDING)
        {
            return HRESULT_FROM_WIN32(err);
        }

        _isPendingCompletion = TRUE;
    }

    return S_OK;
}

BOOL KinectRgbCommandList::IsPendingCompletion()
{
    return _isPendingCompletion;
}

void KinectRgbCommandList::Complete()
{
    _isPendingCompletion = FALSE;
}

const NUISENSOR_RGB_CHANGE_STREAM_SETTING* KinectRgbCommandList::GetSettings()
{
    return _settings;
}

const NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY* KinectRgbCommandList::GetReplies()
{
    return _replies;
}
