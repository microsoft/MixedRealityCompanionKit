#pragma once

namespace Xbox {
namespace Kinect {
namespace Internal {

DWORD
WINAPI
GetLastError();

BOOL
WINAPI
Initialize(
    __inout PNUISENSOR_HANDLE pNuiSensorHandle
    );

BOOL
WINAPI
Shutdown(
    __in NUISENSOR_HANDLE NuiSensorHandle
    );

BOOL
WINAPI
ColorChangeCameraSettings(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _Out_writes_(ScratchBufferSize) void* ScratchBuffer,
    _In_ DWORD ScratchBufferSize,
    _In_reads_bytes_(SettingsSizeInBytes) ::NUISENSOR_RGB_CHANGE_STREAM_SETTING* pSettings,
    _In_ DWORD SettingsSizeInBytes,
    _Out_writes_bytes_(ReplySizeInBytes) ::NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY* pReply,
    _In_ DWORD ReplySizeInBytes,
    _In_ LPOVERLAPPED pOverlapped
    );

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
    );

}
}
}
