#include <Windows.h>
#include "NuiSensorLib.h"

#include "Helpers.h"

namespace Xbox {
namespace Kinect {
namespace Internal {

DWORD g_LastError;

DWORD GetLastError()
{
    return g_LastError;
}

BOOL SetLastError(BOOL success)
{
    g_LastError = ::GetLastError();
    return success;
}

BOOL
WINAPI
Initialize(
    __inout PNUISENSOR_HANDLE pNuiSensorHandle
    )
{
    return SetLastError(NuiSensor_Initialize(pNuiSensorHandle));
}

BOOL
WINAPI
Shutdown(
    __in NUISENSOR_HANDLE NuiSensorHandle
    )
{
    return SetLastError(NuiSensor_Shutdown(NuiSensorHandle));
}

BOOL
WINAPI
ColorChangeCameraSettings(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _Out_writes_(ScratchBufferSize) void* ScratchBuffer,
    _In_ DWORD ScratchBufferSize,
    _In_reads_bytes_(SettingsSizeInBytes) NUISENSOR_RGB_CHANGE_STREAM_SETTING* pSettings,
    _In_ DWORD SettingsSizeInBytes,
    _Out_writes_bytes_(ReplySizeInBytes) NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY* pReply,
    _In_ DWORD ReplySizeInBytes,
    _In_ LPOVERLAPPED pOverlapped
    )
{
    return SetLastError(NuiSensor_ColorChangeCameraSettings(
        NuiSensorHandle,
        ScratchBuffer,
        ScratchBufferSize,
        pSettings,
        SettingsSizeInBytes,
        pReply,
        ReplySizeInBytes,
        pOverlapped));
}

BOOL
WINAPI
ColorChangeCameraSettingsSync(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _Out_writes_(ScratchBufferSize) void* ScratchBuffer,
    _In_ DWORD ScratchBufferSize,
    _In_reads_bytes_(SettingsSizeInBytes) ::NUISENSOR_RGB_CHANGE_STREAM_SETTING* pSettings,
    _In_ DWORD SettingsSizeInBytes,
    _Out_writes_bytes_(ReplySizeInBytes) ::NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY* pReply,
    _In_ DWORD ReplySizeInBytes
    )
{
    OVERLAPPED overlapped = {0};

    overlapped.hEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    BOOL success = NuiSensor_ColorChangeCameraSettings(
        NuiSensorHandle,
        ScratchBuffer,
        ScratchBufferSize,
        pSettings,
        SettingsSizeInBytes,
        pReply,
        ReplySizeInBytes,
        &overlapped);

    if (!success && ::GetLastError() == ERROR_IO_PENDING)
    {
        DWORD bytesTransferred;
        success = GetOverlappedResult(
            NuiSensorHandle,
            &overlapped,
            &bytesTransferred,
            TRUE);
    }

    CloseHandle(overlapped.hEvent);

    return SetLastError(success);
}


}
}
}
