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
#include <queue>
#include <mutex>

namespace MixedRemoteViewCompositor
{
    namespace Media
    {
        typedef ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::Media::Core::MediaStreamSource*, ABI::Windows::Media::Core::MediaStreamSourceStartingEventArgs*> IStartingEventHandler;
        typedef ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::Media::Core::MediaStreamSource*, ABI::Windows::Media::Core::MediaStreamSourceSampleRequestedEventArgs*> ISampleRequestedEventHandler;
        typedef ABI::Windows::Foundation::ITypedEventHandler<ABI::Windows::Media::Core::MediaStreamSource*, ABI::Windows::Media::Core::MediaStreamSourceClosedEventArgs*> IMediaClosedEventHandler;
        // TODO: This IClosedEventHandler conflicts with one in idl?

        DECLARE_INTERFACE_IID_(IRealtimeMediaSource, IUnknown, "1afaad8b-b15a-4944-aa06-e16a511199b2")
        {
            STDMETHOD(get_MediaStreamSource)(_COM_Outptr_ ABI::Windows::Media::Core::IMediaStreamSource** ppMediaStreamSource) PURE;
            STDMETHOD(get_StreamResolution)(_Out_ UINT32* pWidth, _Out_ UINT32* pHeight) PURE;
        };

        class RealtimeMediaSourceImpl
            : public Microsoft::WRL::RuntimeClass
            //< Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>
            <RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>
            , IRealtimeMediaSource
            , ABI::Windows::Foundation::IAsyncAction
            , AsyncBase<ABI::Windows::Foundation::IAsyncActionCompletedHandler>
            , Microsoft::WRL::FtmBase>
        {
        public:
            // Contructor
            RealtimeMediaSourceImpl();
            virtual ~RealtimeMediaSourceImpl();

            HRESULT RuntimeClassInitialize(
                _In_ ABI::MixedRemoteViewCompositor::Network::IConnection *pConnection);

            // IRealtimeMediaSource
            IFACEMETHOD(get_MediaStreamSource)(_COM_Outptr_ ABI::Windows::Media::Core::IMediaStreamSource** ppMediaStreamSource);
            IFACEMETHOD(get_StreamResolution)(_Out_ UINT32* pWidth, _Out_ UINT32* pHeight);

            // IAsyncAction
            IFACEMETHOD(put_Completed)(
                _In_ ABI::Windows::Foundation::IAsyncActionCompletedHandler *handler)
            {
                return PutOnComplete(handler);
            }
            IFACEMETHOD(get_Completed)(
                _Out_ ABI::Windows::Foundation::IAsyncActionCompletedHandler** handler)
            {
                return GetOnComplete(handler);
            }
            IFACEMETHOD(GetResults)(void)
            {
                return AsyncBase::CheckValidStateForResultsCall();
            }

            // AsyncBase
            virtual HRESULT OnStart(void) { return S_OK; }
            virtual void OnClose(void)
            {
                HandleError(MF_E_SHUTDOWN);
            }
            virtual void OnCancel(void)
            {
                HandleError(E_ABORT);
            }

        protected:
            HRESULT OnDataReceived(
                _In_ IConnection *sender,
                _In_ IBundleReceivedArgs *args);

        private:
            HRESULT OnStarting(_In_ ABI::Windows::Media::Core::IMediaStreamSource* sender, _In_ ABI::Windows::Media::Core::IMediaStreamSourceStartingEventArgs* args);
            HRESULT OnSampleRequested(_In_ ABI::Windows::Media::Core::IMediaStreamSource* sender, _In_ ABI::Windows::Media::Core::IMediaStreamSourceSampleRequestedEventArgs* args);
            HRESULT OnClosed(_In_ ABI::Windows::Media::Core::IMediaStreamSource* sender, _In_ ABI::Windows::Media::Core::IMediaStreamSourceClosedEventArgs* args);

            // Async action helpers
            HRESULT HandleError(HRESULT hResult);
            HRESULT CompleteAsyncAction(HRESULT hResult);

            // Network calls
            HRESULT SendDescribeRequest();
            HRESULT SendStartRequest();
            HRESULT SendStopRequest();

            // Network handlers
            HRESULT ProcessCaptureReady();
            HRESULT ProcessMediaDescription(_In_ IDataBundle* pBundle);
            HRESULT ProcessMediaSample(_In_ IDataBundle* pBundle);
            HRESULT ProcessMediaFormatChange(_In_ IDataBundle* pBundle);

            HRESULT CreateMediaSource(_In_ IVideoEncodingProperties* pVideoEncodingProperties);
            HRESULT RealtimeMediaSourceImpl::GetPropertiesFromMediaDescription(
                _In_ MediaTypeDescription* pStreamDescription,
                _In_ IDataBundle* pAttributesBuffer,
                _COM_Outptr_ IVideoEncodingProperties** ppVideoEncodingProperties);

            inline HRESULT CheckShutdown() const
            {
                if (_eSourceState == SourceStreamState_Shutdown)
                {
                    return MF_E_SHUTDOWN;
                }
                else
                {
                    return S_OK;
                }
            }

        private:
            Microsoft::WRL::Wrappers::SRWLock m_lock;

            Microsoft::WRL::ComPtr<ABI::Windows::Media::Core::IMediaStreamSource> m_mediaStreamSource;
            EventRegistrationToken m_startingRequestedToken;
            EventRegistrationToken m_sampleRequestedToken;
            EventRegistrationToken m_closeRequestedToken;

            ComPtr<IMFSample> m_latestSample;

            ComPtr<IVideoEncodingProperties> m_spVideoEncoding;

            ComPtr<IConnection> _spConnection; // Network sender
            EventRegistrationToken _evtReceivedToken;

            ComPtr<ABI::Windows::Media::Core::IMediaStreamSourceSampleRequest> m_spRequest;
            ComPtr<ABI::Windows::Media::Core::IMediaStreamSourceSampleRequestDeferral> m_deferral;
            SourceStreamState _eSourceState; // Flag to indicate if Shutdown() method was called.
        };
    }
}