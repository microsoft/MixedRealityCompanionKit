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

#include "Media.RealtimeMediaSource.g.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mferror.h>

namespace winrt::RealtimeStreaming::Media::implementation
{
    struct RealtimeMediaSource : RealtimeMediaSourceT<RealtimeMediaSource>
    {

    public:
        // Contructor
        RealtimeMediaSource(){};
        virtual ~RealtimeMediaSource();

        Windows::Foundation::IAsyncAction InitAsync(
            _In_ RealtimeStreaming::Network::Connection connection);

        // IRealtimeMediaSource
        Windows::Media::Core::MediaStreamSource MediaStreamSource();
        Windows::Media::MediaProperties::VideoEncodingProperties VideoProperties();

        void SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration) {};

    protected:
        void OnDataReceived(
            _In_ Network::Connection const& sender,
            _In_ Network::DataBundleArgs const& args);

    private:
        void OnStarting(_In_ Windows::Media::Core::MediaStreamSource const& sender,
            _In_ Windows::Media::Core::MediaStreamSourceStartingEventArgs const& args);
        
        void OnSampleRequested(_In_ Windows::Media::Core::MediaStreamSource const& sender,
            _In_ Windows::Media::Core::MediaStreamSourceSampleRequestedEventArgs const& args);

        void OnClosed(_In_ Windows::Media::Core::MediaStreamSource const& sender,
            _In_ Windows::Media::Core::MediaStreamSourceClosedEventArgs const& args);

        // Network calls
        void SendDescribeRequest();
        void SendStartRequest();
        void SendStopRequest();

        // Network handlers
        HRESULT ProcessCaptureReady();
        HRESULT ProcessMediaDescription(_In_ Network::DataBundle const& dataBundle);
        HRESULT ProcessMediaSample(_In_ Network::DataBundle const& dataBundle);
        HRESULT ProcessMediaFormatChange(_In_ Network::DataBundle const& dataBundle);

        Windows::Media::MediaProperties::VideoEncodingProperties CreatePropertiesFromMediaDescription(
            _In_ Common::MediaTypeDescription pStreamDescription,
            _In_ Network::DataBundle attributesBuffer);

        HRESULT SetSampleAttributes(
            _In_ Common::MediaSampleHeader const& pSampleHeader,
            _In_ IMFSample* pSample);

        inline HRESULT CheckShutdown() const
        {
            if (m_eSourceState == SourceStreamState::Shutdown)
            {
                return MF_E_SHUTDOWN;
            }
            else
            {
                return S_OK;
            }
        }

    private:
        //Wrappers::CriticalSection _lock;
        slim_mutex m_lock;
        handle m_signal{ nullptr };

        Windows::Media::Core::MediaStreamSource m_mediaStreamSource{ nullptr };
        winrt::event_token m_startingRequestedToken;
        winrt::event_token m_sampleRequestedToken;
        winrt::event_token m_closeRequestedToken;

        com_ptr<IMFSample> m_latestSample;

        LONGLONG m_lastTimeStamp;

        Windows::Media::MediaProperties::VideoEncodingProperties m_spVideoEncoding{ nullptr };

        Network::Connection m_connection; // Network sender
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