// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Media.RealtimeServer.g.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RealtimeServer : RealtimeServerT<RealtimeServer>
    {
        public:
            RealtimeServer(_In_ Network::Connection connection,
                _In_ GUID inputMediaType,
                _In_ Windows::Media::MediaProperties::MediaEncodingProfile mediaEncodingProperties);

            virtual ~RealtimeServer();

            // IRTModule
            void Shutdown() { };

            void WriteFrame(_In_ uint32_t bufferSize,
                __in_ecount(bufferSize) array_view<uint8_t const> bufferArrayView);

            Windows::Media::MediaProperties::VideoEncodingProperties VideoProperties();

        private:
            int GetBytesPerPixel(_In_ GUID inputMediaType);

        private:
            slim_mutex m_lock;

            Windows::Media::MediaProperties::MediaEncodingProfile m_spMediaEncodingProfile;
            Media::NetworkMediaSink m_spNetworkMediaSink{ nullptr };

            winrt::com_ptr<IMFSinkWriter> m_spSinkWriter;
            uint8_t m_sinkWriterStream;

            GUID m_mediaInputFormat;

            LONGLONG rtStart = 0;

            static const UINT32 VIDEO_FPS = 30;
            static const UINT64 VIDEO_FRAME_DURATION = 10 * 1000 * 1000 / VIDEO_FPS;

            struct GUIDComparer
            {
                bool operator()(const GUID & Left, const GUID & Right) const
                {
                    return memcmp(&Left, &Right, sizeof(Right)) < 0;
                }
            };
            const std::map<GUID, int, GUIDComparer> MF_GUIDs_to_BPP = {
                {MFVideoFormat_RGB8, 1}, {MFVideoFormat_RGB555, 2}, {MFVideoFormat_RGB565, 2},
                {MFVideoFormat_RGB24, 3}, {MFVideoFormat_RGB32, 4}, {MFVideoFormat_ARGB32, 4},
                {MFVideoFormat_A16B16G16R16F, 2} };
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RealtimeServer : RealtimeServerT<RealtimeServer, implementation::RealtimeServer>
    {
    };
}