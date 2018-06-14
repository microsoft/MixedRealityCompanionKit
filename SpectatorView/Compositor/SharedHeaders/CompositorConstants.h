// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

// FrameProvider type - Exactly 1 of these should be true:
//TODO: Set this to true if using a BlackMagic DeckLink capture card.
#define USE_DECKLINK            TRUE
//TODO: Set this to true if using a USB 3 external BlackMagic Shuttle capture card.
#define USE_DECKLINK_SHUTTLE    FALSE
//TODO: Set this to true if using an Elgato capture card.
#define USE_ELGATO              FALSE
//TODO: Set this to true if using OpenCV to get frames from a camera or capture card.
#define USE_OPENCV              FALSE

static_assert((USE_ELGATO + USE_DECKLINK + USE_DECKLINK_SHUTTLE + USE_OPENCV == 1),
    "Exactly 1 FrameProvider must be set");

// Frame Dimensions and buffer lengths
//TODO: change this to match video dimensions from your camera.
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

#define INVALID_TIMESTAMP -1


// Timestamp conversion factors.  
// Do not change these values.
#define S2NS   1000000000
#define S2HNS  10000000
#define MS2NS  1000000
#define MS2HNS 10000
#define HNS2NS 100


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

#define AUDIO_BUFSIZE (AUDIO_CHANNEL_SIZE * AUDIO_CHANNELS)

// Polling rate for engine audio.
// This is used to replace periods of no audio with empty audio (eg: engine has stopped, listeners have been disabled)
#define AUDIO_POLLING_RATE_HNS ((LONGLONG)((((float)AUDIO_SAMPLE_RATE * (16.0f /*bits per sample*/ / 8.0f /*bits per byte*/)) / (float)AUDIO_BUFSIZE) * MS2HNS))


#define VIDEO_FPS 30

#define MAX_NUM_CACHED_BUFFERS 20