// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "Generated Files\Media\RealtimeServer.g.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RealtimeServer : RealtimeServerT<RealtimeServer, Plugin::Module>
    {
        public:
            RealtimeServer(_In_ Network::Connection connection,
                _In_ GUID inputMediaType,
                _In_ MediaEncodingProfile mediaEncodingProperties);

            virtual ~RealtimeServer();

            // IModule
            void Shutdown();
            
            void WriteFrame(
                _In_ UINT32 bufferSize,
                __in_ecount(bufferSize) BYTE* pBuffer);

            Windows::Media::MediaProperties::VideoEncodingProperties VideoProperties();

        private:
            //Wrappers::CriticalSection _lock;
            slim_mutex m_lock;

            MediaEncodingProfile m_spMediaEncodingProfile;
            NetworkMediaSink m_spNetworkMediaSink{ nullptr };

            winrt::com_ptr<IMFSinkWriter> m_spSinkWriter;
            DWORD m_sinkWriterStream;

            GUID m_mediaInputFormat;

            LONGLONG rtStart = 0;

            const UINT32 VIDEO_FPS = 30;
            const UINT64 VIDEO_FRAME_DURATION = 10 * 1000 * 1000 / VIDEO_FPS;
    };
}
