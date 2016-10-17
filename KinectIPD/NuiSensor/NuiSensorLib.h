//--------------------------------------------------------------------------
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//
//--------------------------------------------------------------------------
#pragma once
#ifndef __NUISENSORLIB_H__
#define __NUISENSORLIB_H__


#ifdef __cplusplus
extern "C" {
#endif

//
// ----------------------------------------------------------------------------
// General Sensor Information
// ----------------------------------------------------------------------------
//
#define NUISENSOR_IR_FRAME_ROW_COUNT                424
#define NUISENSOR_IR_FRAME_COL_COUNT                512
#define NUISENSOR_IR_FREQUENCY_COUNT                3

#define NUISENSOR_CALIBRATION_ENUM_IR               0
#define NUISENSOR_CALIBRATION_ENUM_RGB_REG          1
#define NUISENSOR_CALIBRATION_ENUM_POST_DATA        2
#define NUISENSOR_CALIBRATION_ENUM_MIC_EQ           3
#define NUISENSOR_CALIBRATION_ENUM_MIC_EQ_MS        4

#define NUISENSOR_CALIBRATION_POST_DATA_MAX_SIZE    16360
#define NUISENSOR_CALIBRATION_MIC_EQ_MAX_SIZE       (15*1024)
#define NUISENSOR_CALIBRATION_MIC_EQ_MS_MAX_SIZE    (15*1024)

#define NUISENSOR_SEND_SCRATCH_SPACE_REQUIRED       1024
#define NUISENSOR_RECEIVE_SCRATCH_SPACE_REQUIRED      16

//
// ----------------------------------------------------------------------------
// USB & Communication enumerations.
// ----------------------------------------------------------------------------
//
#define NUISENSOR_MAX_USB_COMMAND_SIZE              512
#define NUISENSOR_MAX_USB_RESPONSE_SIZE             512

//  Timeouts (in ms).  Specify maximum time the firmware takes to execute commands.
#define NUISENSOR_USB_TIMEOUT_MS_STANDARD           1000
#define NUISENSOR_USB_TIMEOUT_MS_FIRMWARE_UPDATE    10000

#define NUISENSOR_USB_PACKET_TYPE_COMMAND           0x06022009
#define NUISENSOR_USB_PACKET_TYPE_RESPONSE          0x0A6FE000

//
// ----------------------------------------------------------------------------
// USB Commands
// ----------------------------------------------------------------------------
//
enum
{
    NUISENSOR_USB_COMMAND_RESET                     = 0,
    NUISENSOR_USB_COMMAND_GET_STATUS                = 1,
    NUISENSOR_USB_COMMAND_GET_VERSION               = 2,
    NUISENSOR_USB_COMMAND_GET_APP_INFO              = 3,
    NUISENSOR_USB_COMMAND_GET_UPDATE_IMAGE          = 4,
    
    // Must be last.
    NUISENSOR_USB_COMMAND_MAX
};

//
// ----------------------------------------------------------------------------
// Other Symbolic constants
// ----------------------------------------------------------------------------
//
#define NUISENSOR_PARAM_INSTR_BOOT_COUNT            0x00020000
#define NUISENSOR_PARAM_INSTR_WATCHDOG_COUNT        0x00020001
#define NUISENSOR_PARAM_INSTR_POWER_ON_MINUTES      0x00020002
#define NUISENSOR_PARAM_INSTR_PMC_WATCHDOG_COUNT    0x00020004
#define NUISENSOR_PARAM_INSTR_PMC_SHUTDOWN_COUNT    0x00020005


//
// ----------------------------------------------------------------------------
// USB Structures
// ----------------------------------------------------------------------------
//
typedef struct
{
    UINT32                  Version;                // Major << 16 | Minor
    UINT32                  Build;
    UINT32                  Qfe;
    UINT32                  Reserved;
} NUISENSOR_IMAGE_VERSION_INFO, *PNUISENSOR_IMAGE_VERSION_INFO;


#define NUISENSOR_MANIFEST_MAGIC                    0x4D414E49
#define NUISENSOR_MANIFEST_FORMAT_VERSION           1

typedef struct
{
    UINT32                  Magic;
    UINT32                  Format_Version;
    UINT32                  EntryTableOffset;
    UINT32                  EntryTableCount;
    UINT32                  FileChecksum;           // Simple checksum
    UINT32                  Reserved[3];
} NUISENSOR_MANIFEST_HEADER, *PNUISENSOR_MANIFEST_HEADER;


#define NUISENSOR_IMAGE_SELA_1BL                    0
#define NUISENSOR_IMAGE_SELA_2BL_0                  1
#define NUISENSOR_IMAGE_SELA_2BL_1                  2
#define NUISENSOR_IMAGE_SELA_APP                    3
#define NUISENSOR_IMAGE_PMC_BL                      4
#define NUISENSOR_IMAGE_PMC_APP_0                   5
#define NUISENSOR_IMAGE_PMC_APP_1                   6
#define NUISENSOR_IMAGE_COUNT                       7

#define NUISENSOR_VERSION_INVALID_IMAGE             0

typedef struct
{
    UINT32                  Tag;                    // NUISENSOR_IMAGE_xxx
    UINT32                  Offset;
    UINT32                  Size;
    UINT16                  Flags;                  // NUISENSOR_MANIFEST_FLAGS_xxx
    UINT16                  Reserved1;
    NUISENSOR_IMAGE_VERSION_INFO    Version;
    UINT32                  Reserved2[4];
} NUISENSOR_MANIFEST_ENTRY, *PNUISENSOR_MANIFEST_ENTRY;


typedef struct
{
    BYTE                    Data[128];
} NUISENSOR_SERIAL_NUMBER, *PNUISENSOR_SERIAL_NUMBER;


#define NUISENSOR_USB_VERSION_INFO_ENTRY_COUNT      32

#define NUISENSOR_HW_VERSION_EV1                    1
#define NUISENSOR_HW_VERSION_EV2                    2
#define NUISENSOR_HW_VERSION_EV3                    3
#define NUISENSOR_HW_VERSION_EV4                    4
#define NUISENSOR_HW_VERSION_EV5                    5
#define NUISENSOR_HW_VERSION_DV                     6
#define NUISENSOR_HW_VERSION_PV                     7
#define NUISENSOR_HW_VERSION_UNKNOWN                0

typedef struct
{
    NUISENSOR_IMAGE_VERSION_INFO    Info[NUISENSOR_USB_VERSION_INFO_ENTRY_COUNT];
    ULONG                           BasicHwVersion;
    ULONG                           RawHwVersion;
} NUISENSOR_USB_VERSION_INFO, *PNUISENSOR_USB_VERSION_INFO;

typedef struct
{
    UINT32                  Tag;
} NUISENSOR_USB_UPDATE_IMAGE_COMMAND, *PNUISENSOR_USB_UPDATE_IMAGE_COMMAND;

typedef struct
{
    UINT32                  ResetIndication;
} NUISENSOR_USB_UPDATE_IMAGE_RESPONSE, *PNUISENSOR_USB_UPDATE_IMAGE_RESPONSE;

typedef struct
{
    BYTE                    AppName[64];
    NUISENSOR_IMAGE_VERSION_INFO    Version;
    UINT32                  AppTag;
    UINT32                  BL2Tag;
    UINT32                  Reserved[6];
} NUISENSOR_USB_APP_INFO, *PNUISENSOR_USB_APP_INFO;

typedef struct
{
    UINT32                  Tag;                    // NUISENSOR_INSTRUMENTATION_TAG_xxx?
    UINT32                  Type;                   // NUISENSOR_INSTRUMENTATION_TYPE_xxx
    LARGE_INTEGER           Value;
} NUISENSOR_INSTRUMENTATION_ENTRY, *PNUISENSOR_INSTRUMENTATION_ENTRY;

typedef struct
{
    UINT32                  PacketType;             // NUISENSOR_USB_PACKET_TYPE_COMMAND
    UINT32                  PacketTransactionID;
    UINT32                  PayloadSize;
    UINT32                  Command;
    UINT32                  Reserved;               // Must be zero
} NUISENSOR_USB_COMMAND_HEADER, *PNUISENSOR_USB_COMMAND_HEADER;

typedef union
{
    BYTE                    Data[1];
    NUISENSOR_USB_UPDATE_IMAGE_COMMAND  UpdateImage;
} NUISENSOR_USB_COMMAND_DATA, *PNUISENSOR_USB_COMMAND_DATA;

typedef struct
{
    NUISENSOR_USB_COMMAND_HEADER    Header;
    NUISENSOR_USB_COMMAND_DATA      Data;
} NUISENSOR_USB_PACKET_COMMAND, *PNUISENSOR_USB_PACKET_COMMAND;

typedef struct
{
    UINT32                  PacketType;
    UINT32                  PacketTransactionID;
    UINT32                  Status;
    UINT32                  Reserved;               // Will be zero
} NUISENSOR_USB_PACKET_RESPONSE, *PNUISENSOR_USB_PACKET_RESPONSE;

//
// Async structure for use with asynchronous functions in NuiSensorLib.
//
typedef struct
{
    BYTE            SendScratchSpace[NUISENSOR_SEND_SCRATCH_SPACE_REQUIRED];
    BYTE            ReceiveScratchSpace[NUISENSOR_RECEIVE_SCRATCH_SPACE_REQUIRED];
} NUISENSOR_ASYNC, *PNUISENSOR_ASYNC;

//
// ----------------------------------------------------------------------------
// NuiSensorLib enumerations & constants
// ----------------------------------------------------------------------------
//
#define NUISENSOR_UPDATE_STATE_IDLE                 0
#define NUISENSOR_UPDATE_STATE_DONE_RESETTING       1
#define NUISENSOR_UPDATE_STATE_STARTING             2
#define NUISENSOR_UPDATE_STATE_PROGRAMMING          3
#define NUISENSOR_UPDATE_STATE_VERIFYING            4

typedef struct
{
    UINT32                  State;
    UINT32                  Percent;
    UINT32                  CompletionStatus;
} NUISENSOR_UPDATE_STATUS, *PNUISENSOR_UPDATE_STATUS;

typedef struct
{
    DWORD                   StatusBits;
    DWORD                   ReasonBits;
} NUISENSOR_DEVICE_ERROR_INFO, *PNUISENSOR_DEVICE_ERROR_INFO;

typedef struct
{
    WCHAR                   DevicePath[MAX_PATH];
    union
    {
        NUISENSOR_DEVICE_ERROR_INFO     ErrorData;
    } SpecificData;
} NUISENSOR_DEVICE_INFO, *PNUISENSOR_DEVICE_INFO;


//
// Used by IOCTL_NUISENSOR_IR_RECEIVE, IOCTL_NUISENSOR_IR_RECEIVE_RETURN_BUFS
//
#define NUISENSOR_MAX_IR_LINES                      424
#define NUISENSOR_IR_ROW_STRIDE                     ((512 * 11) / 8)
#define NUISENSOR_IR_CAPTURES_PER_FRAME             10
#define NUISENSOR_MIN_IR_RECEIVE_COUNT              3
#define NUISENSOR_MAX_IR_CHUNKS                     16

//
// Treat this as an opaque structure for driver / library use.
// 
typedef struct
{
    LARGE_INTEGER           Timestamp;
    LARGE_INTEGER           DriverTimestamp;
    ULONG                   LinesPerChunk;
    ULONG                   ChunkOffsets[NUISENSOR_MAX_IR_CHUNKS];
} NUISENSOR_IR_SUB_RECEIVE;

typedef struct
{
    ULONG                       StructVersion;          // Filled by NuiSensorLib
    ULONG                       Status;                 // Frame receive status
    ULONG                       FrameSetNumber;         // Frame sequence number
    NUISENSOR_IR_SUB_RECEIVE    Captures[NUISENSOR_IR_CAPTURES_PER_FRAME];
} NUISENSOR_IR_RECEIVE, *PNUISENSOR_IR_RECEIVE;

//
// The following structures and flags are used by the NuiSensor_IrDecodeReceive
// function.
// 
typedef struct
{
    ULONG                       Status;                 // Frame receive status
    LARGE_INTEGER               Timestamp;              // Time when captured
    LARGE_INTEGER               DriverTimestamp;        // Time when driver received data.
    ULONG                       FrameSetNumber;         // Frame sequence number
    ULONG                       CaptureIndex;           // Capture index, 0-9
    ULONG                       BufferOffsets[NUISENSOR_MAX_IR_LINES];  // Line offsets
} NUISENSOR_IR_CAPTURE, *PNUISENSOR_IR_CAPTURE;

typedef struct
{
    ULONG                   FrameSetNumber;
    NUISENSOR_IR_CAPTURE    Captures[NUISENSOR_IR_CAPTURES_PER_FRAME];
    NUISENSOR_IR_RECEIVE    DriverData;
} NUISENSOR_IR_FRAME, *PNUISENSOR_IR_FRAME;

#define NUISENSOR_IR_DECODE_FLAG_INORDER_OFFSETS        (1UL <<  0)

typedef struct
{
    ULONG                   Flags;
} NUISENSOR_IR_DECODE_OPTIONS, *PNUISENSOR_IR_DECODE_OPTIONS;


//
// Used by IOCTL_NUISENSOR_COLOR_RECEIVE, IOCTL_NUISENSOR_COLOR_RECEIVE_RETURN_BUFS
//      IOCTL_NUISENSOR_COLOR_RECEIVE_RETURN_UNCOMP
//
typedef struct
{
    ULONG                   StructVersion;
    ULONG                   Status;                 // frame receive status
    LARGE_INTEGER           Timestamp;              // time when received
    ULONG                   FrameSetNumber;         // frame set sequence #
    ULONG                   FrameSize;              // expected total transfer size (from firmware)
    float                   IntegrationTime;        // integration time from ISP
    float                   DigitalGain;            // digital gain from ISP
    ULONG                   FrameStartOffset;
    ULONG                   FrameLength;            // total size returned to caller (incl sentinels in JPEG mode)
    ULONG                   UncompressedOffset;
    float                   AnalogGain;             // analog gain from ISP
    ULONG                   CommandSequence;
} NUISENSOR_COLOR_RECEIVE, *PNUISENSOR_COLOR_RECEIVE;


//
//  Used by IOCTL_NUISENSOR_IR_STREAM_CONTROL
//
typedef struct
{
    BOOLEAN                 On;
} NUISENSOR_IR_STREAM_CONTROL, *PNUISENSOR_IR_STREAM_CONTROL;

//
//  Used by IOCTL_NUISENSOR_RGB_STREAM_CONTROL
//
typedef enum
{
    NUISENSOR_COLOR_FORMAT_LEGACY                   = 0,    // TODO - remove ?
    NUISENSOR_COLOR_FORMAT_MJPEG                    = 1,    // Camera in MJPEG, send as-is to user
    NUISENSOR_COLOR_FORMAT_MJPEG_SW_YUV             = 2,    // Camera in MJPEG, SW decompress to YUV (2 BPP)
    NUISENSOR_COLOR_FORMAT_MJPEG_SW_RGB888          = 3,    // Camera in MJPEG, SW decompress to RGB (3 BPP)
    NUISENSOR_COLOR_FORMAT_MJPEG_HW_YUV             = 4,    // Camera in MJPEG, HW decompress to YUV (2 BPP)
    NUISENSOR_COLOR_FORMAT_YUV_422                  = 5,    // Camera in YUV422, send as-is to user
    
    // Must be last.
    NUISENSOR_COLOR_FORMAT_MAX
} NUISENSOR_COLOR_FORMAT;


typedef struct
{
    BOOLEAN                 On;
    BOOLEAN                 SetCameraMode;          // TODO - have driver issue the PETRA_USB_COMMAND_RGB_SET_STREAM_MODE
    ULONG                   ColorFormat;            // NUISENSOR_COLOR_FORMAT_*
    ULONG                   UncompressedFrameSize;
    ULONG                   UncompressedFrameCount;
    ULONG                   Pitch;                      // currently not set.
} NUISENSOR_COLOR_STREAM_CONTROL, *PNUISENSOR_COLOR_STREAM_CONTROL;


//
//  used by IOCTL_NUISENSOR_GET_DRIVER_STAT
//
#define NUISENSOR_STAT_REQUEST_READ_ONLY            0
#define NUISENSOR_STAT_REQUEST_READ_AND_CLEAR       1

typedef struct
{
    ULONG                   RequestReadType;        // NUISENSOR_STAT_REQUEST_*
} NUISENSOR_DRIVER_STAT_REQUEST, *PNUISENSOR_DRIVER_STAT_REQUEST;

typedef struct
{
    ULONG                   Count;
    ULONG                   _pad;
    // LONGLONG     Value[Count]
} NUISENSOR_DRIVER_STAT_RESPONSE, *PNUISENSOR_DRIVER_STAT_RESPONSE;

//
// Used by IOCTL_NUISENSOR_STREAM_SYNC_STATS
//
typedef struct
{
    ULONG               ClearGpu;                       // Set to 1 if GPU history should be cleared.
    ULONG               ClearIdeal;                     // Set to 1 if "ideal" history should be cleared.
    ULONG               ClearCamera;                    // Set to 1 if camera history should be cleared.
} NUISENSOR_STREAM_SYNC_STAT_REQUEST, *PNUISENSOR_STREAM_SYNC_STAT_REQUEST;

typedef struct
{
    LONGLONG           Value;                           // QPC ticks.
    LONGLONG           Count;                           // Number of samples.
    LONGLONG           Diff2;                           // (Sample diffs)**2 -- QPC ticks.
} NUISENSOR_STREAM_SYNC_STAT, *PNUISENSOR_STREAM_SYNC_STAT;

typedef struct
{
    NUISENSOR_STREAM_SYNC_STAT      GpuPeriod;          // GPU scheduler stats.
    NUISENSOR_STREAM_SYNC_STAT      IdealPeriod;        // "Ideal" period stats.
    NUISENSOR_STREAM_SYNC_STAT      TargetPeriod;       // The period used to calculate phase offset (GPU or ideal).
    NUISENSOR_STREAM_SYNC_STAT      CameraPeriod;       // Camera stats.
    NUISENSOR_STREAM_SYNC_STAT      Phase;              // Calculated phase.
    NUISENSOR_STREAM_SYNC_STAT      PhaseVelocity;      // Calculated phase velocity.
} NUISENSOR_STREAM_SYNC_STAT_RESULT, *PNUISENSOR_STREAM_SYNC_STAT_RESULT;

//
// Calibration data format.
//
#pragma pack(push, 1)

typedef struct
{
    UINT16                  CalibFileVersion;
    UINT16                  CalibVersion;
    UINT16                  SizeOfCalibHeader;
} NUISENSOR_CALIBRATION_FILE_HEADER;

typedef struct
{
    UINT16                  ChipID;
    UINT32                  USBDriver_version;
    UINT8                   config_version[256];
    UINT32                  uc_code_version;
    UINT32                  calibration_date;
    UINT32                  calibration_time;
    UINT16                  n_calib_rows;
    UINT16                  n_calib_cols;
    UINT32                  FirmwareVersion;
    UINT16                  Geometric_Parameters_Size;
    UINT16                  N_Freqs;
    UINT16                  Freq_Table_Size;
    UINT32                  P0_Table_Size;
    UINT16                  T0_Table_Size;
    UINT16                  Tempco_Table_Size;
    UINT16                  Depth_Engine_Registers_Size;
    UINT32                  CCC_Version;
    UINT32                  P0_CAL_Version;
    UINT32                  XY_CAL_Version;
    UINT32                  REG_CAL_Version;
    UINT8                   serial_num2[16];
    UINT8                   Chip_unique_id[32];
    UINT32                  Gain_Correction_Block_Size;
    UINT16                  ABMultiplier_Table_Size;
    UINT8                   Pad[56];
} NUISENSOR_CALIBRATION_HEADER;

typedef struct
{
    float                   Fc1;
    float                   Fc2;
    float                   AlphaC;
    float                   cc1;
    float                   cc2;
    float                   Kc1;
    float                   Kc2;
    float                   Kc3;
    float                   Kc4;
    float                   Kc5;
    float                   R[9];
    float                   T[3];
    float                   ZOffset;
} NUISENSOR_CALIBRATION_TABLE_BLOCK;

typedef struct
{
    USHORT                  freq;
    USHORT                  P0Table[NUISENSOR_IR_FRAME_ROW_COUNT * NUISENSOR_IR_FRAME_COL_COUNT];
    USHORT                  reserved_0;             // TempOffset;
    USHORT                  reserved_1;             // TempCoefficient;
    USHORT                  abMultiplier;
} NUISENSOR_CALIBRATION_FREQUENCY_DATA;

typedef struct
{
    DWORD                   Gain_Correction_Block_Ver;
    USHORT                  Global_Gain_Top;
    USHORT                  Global_Gain_Bottom;
    USHORT                  Crease_Corr_Coeff_Top;
    USHORT                  Crease_Coor_Coeff_Bottom;
} NUISENSOR_CALIBRATION_GAIN_CORRECTION_DATA;

typedef struct
{
    UINT16                  row;
    UINT16                  col;
} NUISENSOR_PIXEL;

#define NUISENSOR_MAX_PIXEL_DEFECTS     70

typedef struct
{
    UINT32                  Pixel_Defect_Block_Ver;
    UINT16                  nBadPixels;
    NUISENSOR_PIXEL         Pixels[NUISENSOR_MAX_PIXEL_DEFECTS];
} NUISENSOR_CALIBRATION_PIXEL_DEFECT_BLOCK;

typedef struct
{
    BYTE                    LDAsymmetry;
    BYTE                    HorizontalOrientation;
    UINT16                  DetectorToCenterLaser;
    float                   CenterLaserTemp;
} NUISENSOR_CALIBRATION_DEVICE_CONFIG_BLOCK;

typedef struct
{
    UINT32                  SizeOfPetraP0Header;
    UINT32                  NumberOfFreqs;
    UINT32                  FreqTableSize;
    UINT32                  P0TableSize;
    UINT32                  ABMultiplierTableSize;
    UINT32                  GainCorrectionBlockSize;
    UINT32                  PixelDefectBlockSize;
    UINT32                  DeviceConfigBlockSize;
} NUISENSOR_CALIBRATION_P0_HEADER;

typedef struct
{
    NUISENSOR_CALIBRATION_FILE_HEADER           FileHeader;
    NUISENSOR_CALIBRATION_HEADER                CalibrationHeader;
    NUISENSOR_CALIBRATION_TABLE_BLOCK           CalibrationTableBlock;
    NUISENSOR_CALIBRATION_FREQUENCY_DATA        P0Data[NUISENSOR_IR_FREQUENCY_COUNT];
    NUISENSOR_CALIBRATION_GAIN_CORRECTION_DATA  GainCorrectionData;
    NUISENSOR_CALIBRATION_PIXEL_DEFECT_BLOCK    PixelDefectBlock;
    NUISENSOR_CALIBRATION_DEVICE_CONFIG_BLOCK   DeviceConfigBlock;
} NUISENSOR_CALIBRATION_INFO;

//
// Color registration information.
//
#define NUISENSOR_CALIBRATION_RGB_POLY_COUNT        10
#define NUISENSOR_CALIBRATION_RGB_CENT_ROWS         23
#define NUISENSOR_CALIBRATION_RGB_CENT_COLS         28

//Defintion of EV3 and earlier cameras
typedef struct
{
    float                   NominalDepth;
    float                   PolynomialCoefficients[2][NUISENSOR_CALIBRATION_RGB_POLY_COUNT];
    float                   IrCentroids[NUISENSOR_CALIBRATION_RGB_CENT_ROWS][NUISENSOR_CALIBRATION_RGB_CENT_COLS][2];
    float                   ColorCentroids[NUISENSOR_CALIBRATION_RGB_CENT_ROWS][NUISENSOR_CALIBRATION_RGB_CENT_COLS][2];
    float                   CentroidDepths[NUISENSOR_CALIBRATION_RGB_CENT_ROWS][NUISENSOR_CALIBRATION_RGB_CENT_COLS];   
} NUISENSOR_CALIBRATION_RGB_REGISTRATION_V0;

//Same as MTE spec, except Version is extended to a DWORD
typedef struct
{
    DWORD                   Version;
    float                   RgbFocalLength;
    float                   RgbPrinciplePointX;
    float                   RgbPrinciplePointY;
    float                   NominalDepth;
    float                   IrToRgbDisplacement;
    float                   PolynomialCoefficients[2][NUISENSOR_CALIBRATION_RGB_POLY_COUNT];
    float                   IrCentroids[NUISENSOR_CALIBRATION_RGB_CENT_ROWS][NUISENSOR_CALIBRATION_RGB_CENT_COLS][2];
    float                   ColorCentroids[NUISENSOR_CALIBRATION_RGB_CENT_ROWS][NUISENSOR_CALIBRATION_RGB_CENT_COLS][2];
    float                   CentroidDepths[NUISENSOR_CALIBRATION_RGB_CENT_ROWS][NUISENSOR_CALIBRATION_RGB_CENT_COLS];
} NUISENSOR_CALIBRATION_RGB_REGISTRATION;

#pragma pack(pop)


typedef struct
{
    float                   X;
    float                   Y;
    float                   Z;
} NUISENSOR_ACCELEROMETER_DATA;

//
// Used by NuiSensor_ResetDevice
//
#define NUISENSOR_RESET_POWER                       0

//
// Used by NuiSensor_GetInstrumentationData
//
#define NUISENSOR_INSTRUMENTATION_CHUNK_SENSOR_INSTR    0
#define NUISENSOR_INSTRUMENTATION_CHUNK_DRIVER          1
#define NUISENSOR_INSTRUMENTATION_CHUNK_XVIO_CLIENT     2
#define NUISENSOR_INSTRUMENTATION_CHUNK_XVIO_PROVIDER   3
#define NUISENSOR_INSTRUMENTATION_CHUNK_SENSOR_ADC      4
#define NUISENSOR_INSTRUMENTATION_CHUNK_SENSOR_FILTER   5

typedef struct
{
    ULONG                   Key;
    UCHAR                   Chunk;          // NUISENSOR_INSTRUMENTATION_CHUNK_*
    UCHAR                   ValueLength;    // in bytes
    USHORT                  _pad;
    LONGLONG                Value;
} NUISENSOR_INSTRUMENTATION_KEY_VALUE_PAIR;

//
// Device Notification callback used by NuiSensor_RegisterDeviceCallbacks().
//
typedef enum
{
    NUISENSOR_DEVICE_NOTIFY_ADD = 0,
    NUISENSOR_DEVICE_NOTIFY_REMOVE,
    NUISENSOR_DEVICE_NOTIFY_ERROR,
} NUISENSOR_DEVICE_NOTIFY_STATE;

typedef void (WINAPI *NUISENSOR_DEVICE_NOTIFY_CALLBACK)(
        void*                               pUserContext,
        const NUISENSOR_DEVICE_INFO*        pDevInfo,
        NUISENSOR_DEVICE_NOTIFY_STATE       DeviceState
        );

//
// RGB color setting structures and constants -- must be kept in sync with
// PetraUsbCommandStructures.h.
// 
typedef enum
{
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_MODE                         = 0,
    NUISENSOR_RGB_COMMAND_SET_INTEGRATION_TIME                      = 1,
    NUISENSOR_RGB_COMMAND_GET_INTEGRATION_TIME                      = 2,

    NUISENSOR_RGB_COMMAND_SET_WHITE_BALANCE_MODE                    = 10,
    NUISENSOR_RGB_COMMAND_SET_RED_CHANNEL_GAIN                      = 11,
    NUISENSOR_RGB_COMMAND_SET_GREEN_CHANNEL_GAIN                    = 12,
    NUISENSOR_RGB_COMMAND_SET_BLUE_CHANNEL_GAIN                     = 13,
    NUISENSOR_RGB_COMMAND_GET_RED_CHANNEL_GAIN                      = 14,
    NUISENSOR_RGB_COMMAND_GET_GREEN_CHANNEL_GAIN                    = 15,
    NUISENSOR_RGB_COMMAND_GET_BLUE_CHANNEL_GAIN                     = 16,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_TIME_MS                      = 17,
    NUISENSOR_RGB_COMMAND_GET_EXPOSURE_TIME_MS                      = 18,
    NUISENSOR_RGB_COMMAND_SET_DIGITAL_GAIN                          = 19,
    NUISENSOR_RGB_COMMAND_GET_DIGITAL_GAIN                          = 20,
    NUISENSOR_RGB_COMMAND_SET_ANALOG_GAIN                           = 21,
    NUISENSOR_RGB_COMMAND_GET_ANALOG_GAIN                           = 22,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_COMPENSATION                 = 23,
    NUISENSOR_RGB_COMMAND_GET_EXPOSURE_COMPENSATION                 = 24,
    NUISENSOR_RGB_COMMAND_SET_ACS                                   = 25,
    NUISENSOR_RGB_COMMAND_GET_ACS                                   = 26,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_MODE                = 27,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONES               = 28,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_0_WEIGHT       = 29,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_1_WEIGHT       = 30,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_2_WEIGHT       = 31,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_3_WEIGHT       = 32,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_4_WEIGHT       = 33,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_5_WEIGHT       = 34,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_6_WEIGHT       = 35,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_7_WEIGHT       = 36,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_8_WEIGHT       = 37,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_9_WEIGHT       = 38,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_10_WEIGHT      = 39,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_11_WEIGHT      = 40,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_12_WEIGHT      = 41,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_13_WEIGHT      = 42,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_14_WEIGHT      = 43,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_15_WEIGHT      = 44,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_16_WEIGHT      = 45,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_17_WEIGHT      = 46,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_18_WEIGHT      = 47,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_19_WEIGHT      = 48,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_20_WEIGHT      = 49,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_21_WEIGHT      = 50,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_22_WEIGHT      = 51,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_23_WEIGHT      = 52,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_24_WEIGHT      = 53,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_25_WEIGHT      = 54,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_26_WEIGHT      = 55,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_27_WEIGHT      = 56,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_28_WEIGHT      = 57,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_29_WEIGHT      = 58,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_30_WEIGHT      = 59,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_31_WEIGHT      = 60,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_32_WEIGHT      = 61,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_33_WEIGHT      = 62,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_34_WEIGHT      = 63,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_35_WEIGHT      = 64,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_36_WEIGHT      = 65,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_37_WEIGHT      = 66,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_38_WEIGHT      = 67,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_39_WEIGHT      = 68,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_40_WEIGHT      = 69,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_41_WEIGHT      = 70,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_42_WEIGHT      = 71,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_43_WEIGHT      = 72,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_44_WEIGHT      = 73,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_45_WEIGHT      = 74,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_46_WEIGHT      = 75,
    NUISENSOR_RGB_COMMAND_SET_EXPOSURE_METERING_ZONE_47_WEIGHT      = 76,
    NUISENSOR_RGB_COMMAND_SET_MAX_ANALOG_GAIN_CAP                   = 77,
    NUISENSOR_RGB_COMMAND_SET_MAX_DIGITAL_GAIN_CAP                  = 78,
    NUISENSOR_RGB_COMMAND_SET_FLICKER_FREE_FREQUENCY                = 79,    


    // This range is reserved.  Do not re-use.
    NUISENSOR_RGB_COMMAND_RESERVED_BASE                   = 400,
    NUISENSOR_RGB_COMMAND_RESERVED_END                    = NUISENSOR_RGB_COMMAND_RESERVED_BASE + 200,
    // End of reserved region

} NUISENSOR_RGB_COMMAND_TYPE;

#define NUISENSOR_RGB_CHANGE_SETTING_MAX_NUM_CMD  63

typedef struct
{
    UINT32      Cmd;
    UINT32      Arg;
} NUISENSOR_RGB_CHANGE_SETTING_CMD;

typedef struct
{
    UINT32                              NumCommands;
    UINT32                              SequenceId;
    NUISENSOR_RGB_CHANGE_SETTING_CMD    Commands[1];
} NUISENSOR_RGB_CHANGE_STREAM_SETTING;

typedef struct
{
    NUISENSOR_RGB_CHANGE_STREAM_SETTING     ChangeStreamSetting;
    NUISENSOR_RGB_CHANGE_SETTING_CMD        ExtraCommands[NUISENSOR_RGB_CHANGE_SETTING_MAX_NUM_CMD - 1];
} NUISENSOR_RGB_CHANGE_STREAM_SETTING_MAX;

typedef struct
{
    UINT32                          Status;
    UINT32                          Data;
} NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY_STATUS;

// Caller needs to fill in the NumStatus field in the reply structure.  This needs to
// match the NumCommands field in the corresponding PETRA_RGB_CHANGE_STREAM_SETTING structure.
typedef struct
{
    UINT32                                              NumStatus;
    UINT32                                              CommandListStatus;
    NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY_STATUS    Status[1];
} NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY;

#define NUISENSOR_RGB_SET_MAX_ANALOG_GAIN_CODED_VALUE_2X        0x10
#define NUISENSOR_RGB_SET_MAX_ANALOG_GAIN_CODED_VALUE_4X        0x20
#define NUISENSOR_RGB_SET_MAX_ANALOG_GAIN_CODED_VALUE_4P571X    0x24
#define NUISENSOR_RGB_SET_MAX_ANALOG_GAIN_CODED_VALUE_5P333X    0x28
#define NUISENSOR_RGB_SET_MAX_ANALOG_GAIN_CODED_VALUE_6P4X      0x2C
#define NUISENSOR_RGB_SET_MAX_ANALOG_GAIN_CODED_VALUE_8X        0x30
#define NUISENSOR_RGB_SET_MAX_ANALOG_GAIN_CODED_VALUE_10P667X   0x38


//
// LED constants.
//
#define NUISENSOR_LED_INDEX_NEXUS       0
#define NUISENSOR_LED_INDEX_PRIVACY     1

typedef struct
{
    DWORD                   Index;
    ULONG                   Intensity;
} NUISENSOR_LED_SETTING;

// 2e11e8bc-d463-4160-9b09-0dcf4cfea67f
DEFINE_GUID(GUID_CLASS_NUISENSOR,
    0x2e11e8bc, 0xd463, 0x4160, 0x9b, 0x09, 0x0d, 0xcf, 0x4c, 0xfe, 0xa6, 0x7f);

#define IOCTL_NUISENSOR_GENERIC_IN_COMMAND          CTL_CODE(FILE_DEVICE_UNKNOWN, 0,  METHOD_NEITHER,       FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_GENERIC_OUT_COMMAND         CTL_CODE(FILE_DEVICE_UNKNOWN, 1,  METHOD_NEITHER,       FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_INITIALIZE_IR_BUFFERS       CTL_CODE(FILE_DEVICE_UNKNOWN, 2,  METHOD_OUT_DIRECT,    FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_IR_RECEIVE                  CTL_CODE(FILE_DEVICE_UNKNOWN, 3,  METHOD_OUT_DIRECT,    FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_IR_RECEIVE_RETURN_BUFS      CTL_CODE(FILE_DEVICE_UNKNOWN, 4,  METHOD_IN_DIRECT,     FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_COLOR_RECEIVE               CTL_CODE(FILE_DEVICE_UNKNOWN, 5,  METHOD_OUT_DIRECT,    FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_RAW_IR_READ                 CTL_CODE(FILE_DEVICE_UNKNOWN, 6,  METHOD_OUT_DIRECT,    FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_RAW_IR_READ_RETURN_BUFS     CTL_CODE(FILE_DEVICE_UNKNOWN, 7,  METHOD_IN_DIRECT,     FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_GET_DRIVER_STAT             CTL_CODE(FILE_DEVICE_UNKNOWN, 8,  METHOD_BUFFERED,      FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_RAW_IN_COMMAND              CTL_CODE(FILE_DEVICE_UNKNOWN, 9,  METHOD_NEITHER,       FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_RAW_OUT_COMMAND             CTL_CODE(FILE_DEVICE_UNKNOWN, 10, METHOD_NEITHER,       FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_INITIALIZE_COLOR_BUFFERS    CTL_CODE(FILE_DEVICE_UNKNOWN, 11, METHOD_OUT_DIRECT,    FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_INITIALIZE_UNCOMP_BUFFERS   CTL_CODE(FILE_DEVICE_UNKNOWN, 12, METHOD_OUT_DIRECT,    FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_IR_STREAM_CONTROL           CTL_CODE(FILE_DEVICE_UNKNOWN, 13, METHOD_BUFFERED,      FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_COLOR_STREAM_CONTROL        CTL_CODE(FILE_DEVICE_UNKNOWN, 14, METHOD_BUFFERED,      FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_SET_FRAME_ARRIVAL_TIME      CTL_CODE(FILE_DEVICE_UNKNOWN, 15, METHOD_BUFFERED,      FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_COLOR_RECEIVE_RETURN_BUFS   CTL_CODE(FILE_DEVICE_UNKNOWN, 16, METHOD_IN_DIRECT,     FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_RESET_PIPE                  CTL_CODE(FILE_DEVICE_UNKNOWN, 17, METHOD_IN_DIRECT,     FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_DEINITIALIZE_IR_BUFFERS     CTL_CODE(FILE_DEVICE_UNKNOWN, 18, METHOD_BUFFERED,      FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_INTERRUPT_IN_READ           CTL_CODE(FILE_DEVICE_UNKNOWN, 19, METHOD_BUFFERED,      FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_COLOR_RECEIVE_RETURN_UNCOMP CTL_CODE(FILE_DEVICE_UNKNOWN, 20, METHOD_IN_DIRECT,     FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_ARM_FOR_WAKE                CTL_CODE(FILE_DEVICE_UNKNOWN, 21, METHOD_BUFFERED,      FILE_ANY_ACCESS)
#define IOCTL_NUISENSOR_STREAM_SYNC_STATS           CTL_CODE(FILE_DEVICE_UNKNOWN, 23, METHOD_BUFFERED,      FILE_ANY_ACCESS)

#if defined(ENABLE_ALTAIR_COMPAT)
#define IOCTL_NUISENSOR_BOOT_SIM_PETRA              CTL_CODE(FILE_DEVICE_UNKNOWN, 999, METHOD_BUFFERED, FILE_ANY_ACCESS)
#endif


//
// Opaque forward declaration of NUISENSOR_HANDLE type.
//
typedef struct _NUISENSOR_HANDLE *NUISENSOR_HANDLE, **PNUISENSOR_HANDLE;

BOOL
WINAPI
NuiSensor_Initialize(
    __inout PNUISENSOR_HANDLE pNuiSensorHandle
    );

BOOL
WINAPI
NuiSensor_InitializeEx(
    _Inout_ PNUISENSOR_HANDLE pNuiSensorHandle,
    _In_opt_z_ const WCHAR* DeviceName
    );

ULONG
WINAPI
NuiSensor_FindAllDevices(
    __inout NUISENSOR_DEVICE_INFO* pInfoArray,
    __in ULONG MaxInfoCount
    );

BOOL
WINAPI
NuiSensor_SetIoCompletionPort(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in HANDLE ExistingCompletionPort,
    __in ULONG_PTR CompletionKey
    );

BOOL
WINAPI
NuiSensor_RegisterNotifyCallback(
    _In_opt_ void* pUserContext,
    _In_ NUISENSOR_DEVICE_NOTIFY_CALLBACK pCallback
    );

BOOL
WINAPI
NuiSensor_UnregisterNotifyCallback(
    );

BOOL
WINAPI
NuiSensorGenericSendAndReceive(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ ULONG Command,
    _In_reads_(CommandSize) PBYTE CommandBytes,
    _In_ ULONG CommandSize,
    _In_reads_opt_(SendBufferSize) PBYTE SendBuffer,
    _In_ ULONG SendBufferSize,
    _Out_writes_opt_(ReceiveBufferSize) PBYTE ReceiveBuffer,
    _In_ ULONG ReceiveBufferSize,
    _Out_opt_ PULONG BytesTransferred
    );

BOOL
WINAPI
NuiSensorGenericSendAndReceiveAsync(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _Out_writes_(ScratchBufferSize) void* ScratchBuffer,        // At least NUISENSOR_SEND_SCRATCH_SPACE_REQUIRED
    _In_ DWORD ScratchBufferSize,
    _In_ ULONG Command,
    _In_reads_(CommandSize) PBYTE CommandBytes,
    _In_ ULONG CommandSize,
    _In_reads_opt_(SendBufferSize) PBYTE SendBuffer,
    _In_ ULONG SendBufferSize,
    _Out_writes_opt_(ReceiveBufferSize) PBYTE ReceiveBuffer,
    _In_ ULONG ReceiveBufferSize,
    _Out_opt_ PULONG BytesTransferred,
    _In_ LPOVERLAPPED pOverlapped
    );

BOOL
WINAPI
NuiSensor_Shutdown(
    __in NUISENSOR_HANDLE NuiSensorHandle
    );

BOOL
WINAPI
NuiSensor_ResetPipeId(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in ULONG PipeId
    );

BOOL
WINAPI
NuiSensor_ResetDevice(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in ULONG ResetType
    );

BOOL
WINAPI
NuiSensor_InitializeIrBuffers(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in PVOID Memory,
    __in size_t MemorySize
    );

BOOL
WINAPI
NuiSensor_TeardownIrBuffers(
    __in NUISENSOR_HANDLE NuiSensorHandle
    );

BOOL
WINAPI
NuiSensor_InitializeColorBuffers(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in PVOID Memory,
    __in size_t MemorySize
    );

BOOL
WINAPI
NuiSensor_InitializeColorBuffersEx(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_reads_bytes_(CompressedMemorySize) PVOID CompressedMemory,
    _In_ size_t CompressedMemorySize,
    _In_reads_bytes_opt_(UncompressedFrameSize*UncompressedFrameCount) PVOID UncompressedMemory,
    _In_opt_ ULONG UncompressedFrameSize,
    _In_opt_ ULONG UncompressedFrameCount
    );

BOOL
WINAPI
NuiSensor_TeardownColorBuffers(
    __in NUISENSOR_HANDLE NuiSensorHandle
    );

BOOL
WINAPI
NuiSensor_StartIrStream(
    __in NUISENSOR_HANDLE NuiSensorHandle
    );

BOOL
WINAPI
NuiSensor_StopIrStream(
    __in NUISENSOR_HANDLE NuiSensorHandle
    );

BOOL
WINAPI
NuiSensor_Does10CapturesPerFrame(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out BOOL* pDoes10CapturesPerFrame
    );

BOOL
WINAPI
NuiSensor_StartColorStream(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in BOOL UseCompression,
    __in BOOL DoDecompression,
    __in BOOL UseIsochronous
    );

BOOL
WINAPI
NuiSensor_StopColorStream(
    __in NUISENSOR_HANDLE NuiSensorHandle
    );

BOOL
WINAPI
NuiSensor_IrFrameReceive(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out PNUISENSOR_IR_RECEIVE pFrameInfo,
    __in LPOVERLAPPED pOverlapped
    );

_Success_(return)
BOOL
WINAPI
NuiSensor_IrFrameDecode(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ PNUISENSOR_IR_RECEIVE pReceiveInfo,
    _In_ const PNUISENSOR_IR_DECODE_OPTIONS pDecodeOptions,
    _Out_ PNUISENSOR_IR_FRAME pDecodeFrame
    );

BOOL
WINAPI
NuiSensor_IrFrameReturn(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in PNUISENSOR_IR_RECEIVE pFrameInfo
    );

BOOL
WINAPI
NuiSensor_IrFrameReturnEx(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ PNUISENSOR_IR_FRAME pDecodeFrame
    );

BOOL
WINAPI
NuiSensor_ColorFrameReceive(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out PNUISENSOR_COLOR_RECEIVE pFrameInfo,
    __in LPOVERLAPPED pOverlapped
    );

BOOL
WINAPI
NuiSensor_ColorFrameReturn(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ PNUISENSOR_COLOR_RECEIVE pFrameInfo
    );

BOOL
WINAPI
NuiSensor_ColorFrameReturnAsync(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ PNUISENSOR_COLOR_RECEIVE pFrameInfo,
    _In_ LPOVERLAPPED pOverlapped
    );

BOOL
WINAPI
NuiSensor_ColorFrameReturnUncompressed(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ PNUISENSOR_COLOR_RECEIVE pFrameInfo
    );

BOOL
WINAPI
NuiSensor_ColorFrameReturnUncompressedAsync(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ PNUISENSOR_COLOR_RECEIVE pFrameInfo,
    _In_ LPOVERLAPPED pOverlapped
    );

BOOL
WINAPI
NuiSensor_SetFrameArrivalTime(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in LARGE_INTEGER IrTime,
    __in LARGE_INTEGER IrAllowableJitter,
    __in LARGE_INTEGER IrFrameRate,
    __in LARGE_INTEGER RgbTime,
    __in LARGE_INTEGER RgbAllowableJitter,
    __in LARGE_INTEGER RgbFrameRate
    );

BOOL
WINAPI
NuiSensor_GetCalibrationData(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in ULONG CalibrationDataType,
    __in ULONG BufferSize,
    __in PVOID Buffer,
    __out PULONG RxLength
    );

BOOL
WINAPI
NuiSensor_SetCalibrationData(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ ULONG CalibrationDataType,
    _In_ ULONG BufferSize,
    _In_ PVOID Buffer
    );

BOOL
WINAPI
NuiSensor_GetVersionInfo(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out PNUISENSOR_USB_VERSION_INFO VersionInfo
    );

BOOL
WINAPI
NuiSensor_GetAppInfo(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out PNUISENSOR_USB_APP_INFO AppInfo
    );

BOOL
WINAPI
NuiSensor_GetSerialNumber(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out PNUISENSOR_SERIAL_NUMBER SerialNumber
    );

BOOL
WINAPI
NuiSensor_UpdateFirmware(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in HANDLE Event,
    __in ULONG Component,
    __in ULONG UpdateBlobSize,
    __in PVOID UpdateBlob
    );

BOOL
WINAPI
NuiSensor_UpdateStatusGet(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out PNUISENSOR_UPDATE_STATUS  pUpdataStatus
    );

BOOL
WINAPI
NuiSensor_ParameterGetAsync(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ ULONG Parameter,
    _Out_ PULONG pValue,
    _Out_writes_(ScratchBufferSize) void* ScratchBuffer,        // At least NUISENSOR_SEND_SCRATCH_SPACE_REQUIRED
    _In_ DWORD ScratchBufferSize,
    _In_ LPOVERLAPPED pOverlapped
    );
    
BOOL
WINAPI
NuiSensor_ParameterGet(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in ULONG Parameter,
    __out PULONG pValue
    );

BOOL
WINAPI
NuiSensor_ParameterSet(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in DWORD Parameter,
    __in DWORD Value
    );

BOOL
WINAPI
NuiSensor_GetInterruptData(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in PUCHAR pBuffer, 
    __in ULONG  BufferSize, 
    __out PDWORD BytesTransferred
    );

BOOL
WINAPI
NuiSensor_GetAccelerometerDataAsync(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ OVERLAPPED* pOverlapped,
    _In_ PNUISENSOR_ASYNC pAsync
    );

BOOL
WINAPI
NuiSensor_ProcessAccelerometerData(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ PNUISENSOR_ASYNC pAsync,
    _Out_ NUISENSOR_ACCELEROMETER_DATA* pData
    );

BOOL
WINAPI
NuiSensor_GetAccelerometerData(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out NUISENSOR_ACCELEROMETER_DATA* pData
    );
    
BOOL
WINAPI
NuiSensor_UpdateRequired(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out PBOOL pUpdateRequired
    );

BOOL
WINAPI
NuiSensor_GetInstrumentationData(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ DWORD InstrumentationType,
    _Out_writes_bytes_(*pBytesRead) NUISENSOR_INSTRUMENTATION_KEY_VALUE_PAIR* pKeyValuePairs,
    _In_ ULONG BufferSize,
    _Out_ PULONG pBytesRead
    );

BOOL
WINAPI
NuiSensor_GetInstrumentationDataAsync(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ DWORD InstrumentationType,
    _In_ PVOID pBuffer,
    _In_ DWORD BufferSizeBytes,
    _In_ LPOVERLAPPED pOverlapped
    );

BOOL
WINAPI
NuiSensor_ProcessInstrumentationData(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ PVOID pBuffer,
    _In_ DWORD BufferSizeBytes,
    _Out_writes_bytes_(*pKeyValueBytesRead) NUISENSOR_INSTRUMENTATION_KEY_VALUE_PAIR* pKeyValuePairs,
    _In_ DWORD KeyValueSizeBytes,
    _Out_opt_ PULONG pKeyValueBytesRead
    );

BOOL
WINAPI
NuiSensor_SetWatchDogTimeout(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in DWORD Milliseconds
    );

BOOL
WINAPI
NuiSensor_GetWatchDogTimeout(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out PDWORD pMilliseconds
    );

BOOL
WINAPI
NuiSensor_UpdateFirmwareAll(
    __inout PNUISENSOR_HANDLE pNuiSensorHandle,
    __in_opt void (*StatusCallback)(DWORD PercentComplete, DWORD ComponentPercentComplete, DWORD CurrentComponent)
    );

BOOL
WINAPI
NuiSensor_ArmForWake(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in LPOVERLAPPED overlapped,
    __out_opt LPDWORD lpBytesReturned
    );

BOOL
WINAPI
NuiSensor_DisArmFromWake(
    __in NUISENSOR_HANDLE NuiSensorHandle
    );

BOOL
WINAPI
NuiSensor_WovNvDataWrite(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __in PVOID pBuffer,
    __in size_t BufferLength
    );

BOOL
WINAPI
NuiSensor_WovNvDataRead(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out PVOID pBuffer,
    __in size_t BufferLength,
    __out PDWORD pRxLength
    );

BOOL
WINAPI
NuiSensor_NvStatusGet(
    __in NUISENSOR_HANDLE NuiSensorHandle,
    __out PNUISENSOR_UPDATE_STATUS pUpdateStatus
    );

BOOL
WINAPI
NuiSensor_ColorChangeCameraSettings(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _Out_writes_(ScratchBufferSize) void* ScratchBuffer,        // At least NUISENSOR_SEND_SCRATCH_SPACE_REQUIRED
    _In_ DWORD ScratchBufferSize,
    _In_reads_bytes_(SettingsSizeInBytes) NUISENSOR_RGB_CHANGE_STREAM_SETTING* pSettings,
    _In_ DWORD SettingsSizeInBytes,
    _Out_writes_bytes_(ReplySizeInBytes) NUISENSOR_RGB_CHANGE_STREAM_SETTING_REPLY* pReply,
    _In_ DWORD ReplySizeInBytes,
    _In_ LPOVERLAPPED pOverlapped
    );

BOOL
WINAPI
NuiSensor_SetDesiredVSyncPhaseOffset(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ LONGLONG OffsetNanoseconds,
    _In_ LONGLONG ColorOffsetNanoseconds
    );

BOOL
WINAPI
NuiSensor_GetActualVSyncPhaseOffset(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _Out_ LONGLONG* pOffsetNanoseconds,
    _Out_ LONGLONG* pStandardDeviation
    );

BOOL
WINAPI
NuiSensor_SetLedStatus(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ NUISENSOR_LED_SETTING* pSetting
    );

BOOL
WINAPI
NuiSensor_SetLedStatusAsync(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _In_ NUISENSOR_LED_SETTING* pSetting,
    _In_ OVERLAPPED* pOverlapped,
    _In_ PNUISENSOR_ASYNC pNuiSensorOverlapped
    );

BOOL
WINAPI
NuiSensor_GetAvgLaserTemp(
    _In_ NUISENSOR_HANDLE NuiSensorHandle,
    _Out_ PULONG pTemperature,                              // in 0.1C (from sensor firmware)
    _Out_writes_(ScratchBufferSize) void* ScratchBuffer,    // At least NUISENSOR_SEND_SCRATCH_SPACE_REQUIRED
    _In_ DWORD ScratchBufferSize,
    _In_ LPOVERLAPPED pOverlapped
    );


// Status codes from sensor formatted for NT_STATUS.
#define NUISENSOR_SEVERITY_ERROR    0xC0000000
#define NUISENSOR_CUSTOMER_FLAG     0x20000000

#define NUISENSOR_NTSTATUS(code)  ((code)   |  (NUISENSOR_SEVERITY_ERROR | NUISENSOR_CUSTOMER_FLAG))
#define NUISENSOR_STATUS(ntcode)  ((ntcode) & ~(NUISENSOR_SEVERITY_ERROR | NUISENSOR_CUSTOMER_FLAG))



#ifdef __cplusplus
}
#endif

#endif //__NUISENSORLIB_H__
