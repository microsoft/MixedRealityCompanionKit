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
            void Shutdown();
            
            void WriteFrame(
                _In_ UINT32 bufferSize,
                __in_ecount(bufferSize) BYTE* pBuffer);

            Windows::Media::MediaProperties::VideoEncodingProperties VideoProperties();

        private:
            //Wrappers::CriticalSection _lock;
            slim_mutex m_lock;

            Windows::Media::MediaProperties::MediaEncodingProfile m_spMediaEncodingProfile;
            Media::NetworkMediaSink m_spNetworkMediaSink{ nullptr };

            winrt::com_ptr<IMFSinkWriter> m_spSinkWriter;
            uint8_t m_sinkWriterStream;

            GUID m_mediaInputFormat;

            LONGLONG rtStart = 0;

            static const UINT32 VIDEO_FPS = 30;
            static const UINT64 VIDEO_FRAME_DURATION = 10 * 1000 * 1000 / VIDEO_FPS;
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RealtimeServer : RealtimeServerT<RealtimeServer, implementation::RealtimeServer>
    {
    };
}