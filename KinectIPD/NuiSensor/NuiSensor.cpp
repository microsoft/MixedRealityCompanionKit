// This is the main DLL file.

#include "stdafx.h"

#include <vector>

#include "NuiSensor.h"
#include "Helpers.h"

#define GET_NUISENSOR_RGB_CHANGE_STREAM_SETTING_BYTE_COUNT(Setting) \
    ( \
    sizeof(::NUISENSOR_RGB_CHANGE_STREAM_SETTING) \
    + ( (Setting).NumCommands - 1) * sizeof(::NUISENSOR_RGB_CHANGE_SETTING_CMD) \
    )

#define GET_NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY_BYTE_COUNT(Reply) \
    ( \
    sizeof(::NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY) \
    + ( (Reply).NumStatus - 1) * sizeof(::NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY_STATUS) \
    )

namespace Xbox {
namespace Kinect {

NuiSensor::NuiSensor() : _sensor(nullptr), _isDisposed(false)
{
    pin_ptr<NUISENSOR_HANDLE> pSensor = &_sensor;
    TestResult(Internal::Initialize(pSensor));
}

NuiSensor::~NuiSensor()
{
    if (_isDisposed)
        return;

    this->!NuiSensor();
    _isDisposed = true;
}

NuiSensor::!NuiSensor()
{
    if (_sensor)
    {
        Internal::Shutdown(_sensor);
        _sensor = nullptr;
    }
}

void NuiSensor::Shutdown()
{
    this->~NuiSensor();
}

void NuiSensor::ColorChangeCameraSettings(NUISENSOR_RGB_CHANGE_STREAM_SETTING% settings, [Out] NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY% reply)
{
    TestNotDisposed();

    if (settings.Commands->Length > NUISENSOR_RGB_CHANGE_SETTING_MAX_NUM_CMD)
        throw gcnew ArgumentOutOfRangeException();

    std::vector<BYTE> scratchBuffer(NUISENSOR_SEND_SCRATCH_SPACE_REQUIRED);
    std::vector<BYTE> settingBuffer(NUISENSOR_MAX_USB_COMMAND_SIZE);
    std::vector<BYTE> replyBuffer(NUISENSOR_MAX_USB_RESPONSE_SIZE);

    auto pSettings = reinterpret_cast<::NUISENSOR_RGB_CHANGE_STREAM_SETTING*>(settingBuffer.data());
    auto pReplies = reinterpret_cast<::NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY*>(settingBuffer.data());

    pSettings->NumCommands = settings.Commands->Length;
    pSettings->SequenceId = settings.SequenceId;

    for (int i = 0; i < settings.Commands->Length; i++)
    {
        pSettings->Commands[i].Cmd = settings.Commands[i].Cmd;
        pSettings->Commands[i].Arg = settings.Commands[i].Arg;
    }

    pReplies->NumStatus = settings.Commands->Length;

    DWORD settingsSize = GET_NUISENSOR_RGB_CHANGE_STREAM_SETTING_BYTE_COUNT(*pSettings);
    DWORD replySize = GET_NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY_BYTE_COUNT(*pReplies);

    TestResult(Internal::ColorChangeCameraSettingsSync(
        _sensor,
        scratchBuffer.data(),
        (DWORD)scratchBuffer.size(),
        pSettings,
        settingsSize,
        pReplies,
        replySize));

    reply.CommandListStatus = pReplies->CommandListStatus;
    reply.Status = gcnew array<NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY_STATUS>(pReplies->NumStatus);

    for (int i = 0; i < reply.Status->Length; i++)
    {
        reply.Status[i].Status = pReplies->Status[i].Status;
        reply.Status[i].Data = pReplies->Status[i].Data;
    }
}

void NuiSensor::TestNotDisposed()
{
    if (_isDisposed)
        throw gcnew ObjectDisposedException(nullptr);
}

void NuiSensor::TestResult(BOOL success)
{
    if (!success)
        Marshal::ThrowExceptionForHR(HRESULT_FROM_WIN32(Internal::GetLastError()));
}

}
}
