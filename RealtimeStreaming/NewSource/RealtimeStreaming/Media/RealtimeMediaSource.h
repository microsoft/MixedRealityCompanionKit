//*****************************************************************************
//
//	Copyright 2015 Microsoft Corporation
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
//
//	http ://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
//*****************************************************************************

#pragma once

#include "RealtimeStreaming.g.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RealtimeMediaSource : RealtimeMediaSourceT<RealtimeMediaSource>
    {

    public:
        // Contructor
        RealtimeMediaSource();
        virtual ~RealtimeMediaSource();

        IAsyncAction InitAsync(
            _In_ RealtimeStreaming::Network::Connection connection);

        // IRealtimeMediaSource
        Windows::Media::Core::MediaStreamSource GetMediaStreamSource();
        Windows::Media::Core::IVideoEncodingProperties GetVideoProperties();
        //IFACEMETHOD(get_StreamResolution)(_Out_ UINT32* pWidth, _Out_ UINT32* pHeight);

    protected:
        void OnDataReceived(
            _In_ Connection sender,
            _In_ BundleReceivedArgs args);

    private:
        void OnStarting(_In_ Windows::Media::Core::IMediaStreamSource const& sender,
            _In_ Windows::Media::Core::MediaStreamSourceStartingEventArgs const& args);
        void OnSampleRequested(_In_ Windows::Media::Core::MediaStreamSource const& sender,
            _In_ Windows::Media::Core::IMediaStreamSourceSampleRequestedEventArgs const& args);
        void OnClosed(_In_ Windows::Media::Core::MediaStreamSource const& sender,
            _In_ Windows::Media::Core::IMediaStreamSourceClosedEventArgs const& args);

        // Network calls
        HRESULT SendDescribeRequest();
        HRESULT SendStartRequest();
        HRESULT SendStopRequest();

        // Network handlers
        HRESULT ProcessCaptureReady();
        HRESULT ProcessMediaDescription(_In_ DataBundle dataBundle);
        HRESULT ProcessMediaSample(_In_ DataBundle dataBundle);
        HRESULT ProcessMediaFormatChange(_In_ DataBundle dataBundle);

        Windows::Media::MediaProperties::IVideoEncodingProperties CreatePropertiesFromMediaDescription(
            _In_ MediaTypeDescription pStreamDescription,
            _In_ DataBundle attributesBuffer);

        HRESULT SetSampleAttributes(
            _In_ MediaSampleHeader const& pSampleHeader,
            _In_ IMFSample* pSample);

        inline HRESULT CheckShutdown() const
        {
            if (m_eSourceState == SourceStreamState_Shutdown)
            {
                return MF_E_SHUTDOWN;
            }
            else
            {
                return S_OK;
            }
        }

    private:
        //Wrappers::SRWLock m_lock;
        Wrappers::CriticalSection _lock;
        slim_mutex m_lock;

        Windows::Media::Core::MediaStreamSource m_mediaStreamSource{ nullptr };
        winrt::event_token m_startingRequestedToken;
        winrt::event_token m_sampleRequestedToken;
        winrt::event_token m_closeRequestedToken;

        com_ptr<IMFSample> m_latestSample;

        LONGLONG m_lastTimeStamp;

        VideoEncodingProperties m_spVideoEncoding{ nullptr };

        Connection m_connection; // Network sender
        winrt::event_token m_evtReceivedToken;

        Windows::Media::Core::MediaStreamSourceSampleRequest m_spRequest{ nullptr };
        Windows::Media::Core::MediaStreamSourceSampleRequestDeferral m_deferral{ nullptr };
        SourceStreamState m_eSourceState; // Flag to indicate if Shutdown() method was called.
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RealtimeMediaSource : RealtimeMediaSourceT<RealtimeMediaSource, implementation::RealtimeMediaSource>
    {
    };
}