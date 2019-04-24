// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

//TODO: If you change any of these definitions, recompile UnityCompositorInterface and copy DLL's 
//      to your Unity project.

// Performance flags, set these to FALSE to improve performance for lower end hardware.
// Note setting these to false may reduce image quality or hologram stability.

// FrameProvider type - Exactly 1 of these should be true:
//TODO: Set this to true if using a USB 3 external BlackMagic Shuttle capture card.
#define USE_DECKLINK_SHUTTLE    FALSE
//TODO: Set this to true if using OpenCV to get frames from a camera or capture card.
#define USE_OPENCV              FALSE

// Audio
//TODO: Set this to true to encode audio with captured video.
//NOTE: If you do not have Audio data, set this to false or the video may encode incorrectly.
#define ENCODE_AUDIO TRUE

// These should match Game Engine's audio settings.  Size is in bytes.
// These values should also be valid data values for H.264 encoding:
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd742785(v=vs.85).aspx
// NOTE: Audio bits per sample must be 16.
// This should match size (in bytes) of audio data from Engine.
// Note: Audio from Engine must be scaled to short.MinValue..short.MaxValue and converted to a byte array.
// In Unity, audio is retrieved as a float array -1..1, but the channel size is:
// sizeof(short) * (arrayLength / numChannels) instead of sizeof(float)
// since we need to convert the floats to shorts for audio encoding.
#define AUDIO_CHANNEL_SIZE  2048
// This must be 1, 2, or 6 (if Win10)
#define AUDIO_CHANNELS      2
// This must be 44100 or 48000
#define AUDIO_SAMPLE_RATE   48000
// Output audio resolution - Average bytes per second.
// This must be: 12000, 16000, 20000, or 24000.
#define AUDIO_BPS           24000


// Frame Dimensions and buffer lengths
//TODO: change this to match video dimensions from your tethered camera.
#define FRAME_WIDTH    1920
#define FRAME_HEIGHT   1080

#define FRAME_BPP      4            // RGBA
#define FRAME_BPP_RAW  2            // YUV

#define HOLOGRAM_WIDTH              FRAME_WIDTH
#define HOLOGRAM_HEIGHT             FRAME_HEIGHT

// Color camera buffer size.
#define FRAME_BUFSIZE               (FRAME_WIDTH * FRAME_HEIGHT * FRAME_BPP)
#define FRAME_BUFSIZE_RAW           (FRAME_WIDTH * FRAME_HEIGHT * FRAME_BPP_RAW)

// Hologram buffer size.
#define HOLOGRAM_BUFSIZE            (HOLOGRAM_WIDTH * HOLOGRAM_HEIGHT * FRAME_BPP)

#define HOLOGRAM_BUFSIZE_HIRES      (HOLOGRAM_WIDTH_HIRES * HOLOGRAM_HEIGHT_HIRES * FRAME_BPP)

// Return timestamps in HNS.  Do not change this value.
#define QPC_MULTIPLIER 10000000

#define VIDEO_FPS 30

// Default number of frames to offset Color timestamp to account for latency between
// capturing a frame on the camera and retrieving from the capture card.
#define INITIAL_FRAME_OFFSET 0.0f

#define HARDWARE_ENCODE_VIDEO TRUE
