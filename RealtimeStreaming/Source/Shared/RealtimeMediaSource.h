// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

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
        
        RealtimeStreaming::Media::SourceStreamState CurrentState();

    protected:
        void OnDataReceived(
            //_In_ Network::Connection const& sender,
            _In_ IInspectable const& sender,
            _In_ Network::DataBundleArgs const& args);

    private:
        void OnStarting(_In_ Windows::Media::Core::MediaStreamSource const& sender,
            _In_ Windows::Media::Core::MediaStreamSourceStartingEventArgs const& args);
        
        void OnSampleRequested(_In_ Windows::Media::Core::MediaStreamSource const& sender,
            _In_ Windows::Media::Core::MediaStreamSourceSampleRequestedEventArgs const& args);

        void OnPaused(_In_ Windows::Media::Core::MediaStreamSource const& sender,
            _In_ IInspectable const args);

        void OnClosed(_In_ Windows::Media::Core::MediaStreamSource const& sender,
            _In_ Windows::Media::Core::MediaStreamSourceClosedEventArgs const& args);

        void OnSampleRendered(Windows::Media::Core::MediaStreamSource sender,
            Windows::Media::Core::MediaStreamSourceSampleRenderedEventArgs args);

        // Network calls
        winrt::fire_and_forget RealtimeMediaSource::SendNetworkRequest(_In_ RealtimeStreaming::Common::PayloadType type);

        // Network handlers
        Windows::Foundation::IAsyncAction ProcessNetworkRequestAsync(_In_ RealtimeStreaming::Common::PayloadType type,
            _In_ RealtimeStreaming::Network::DataBundle data);
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
            if (m_SourceState == SourceStreamState::Shutdown)
            {
                return MF_E_SHUTDOWN;
            }
            else
            {
                return S_OK;
            }
        }

    private:
        slim_mutex m_lock;
        handle m_signal{ nullptr };

        Windows::Media::Core::MediaStreamSource m_mediaStreamSource{ nullptr };
        winrt::event_token m_startingRequestedToken;
        winrt::event_token m_sampleRequestedToken;
        winrt::event_token m_pausedToken;
        winrt::event_token m_closeRequestedToken;

        com_ptr<IMFSample> m_latestSample;

        LONGLONG m_lastTimeStamp;

        Windows::Media::MediaProperties::VideoEncodingProperties m_spVideoEncoding{ nullptr };

        Network::Connection m_connection{ nullptr };
        winrt::event_token m_evtReceivedToken;

        Windows::Media::Core::MediaStreamSourceSampleRequest m_spRequest{ nullptr };
        Windows::Media::Core::MediaStreamSourceSampleRequestDeferral m_deferral{ nullptr };
        SourceStreamState m_SourceState; // Flag to indicate if Shutdown() method was called.
    };
}

namespace winrt::RealtimeStreaming::Media::factory_implementation
{
    struct RealtimeMediaSource : RealtimeMediaSourceT<RealtimeMediaSource, implementation::RealtimeMediaSource>
    {
    };
}