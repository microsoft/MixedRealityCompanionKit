// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace RealtimeStreaming
{
    namespace Media
    {
        class MrvcSchemeHandlerImpl
            : public RuntimeClass<RuntimeClassFlags<WinRtClassicComMix>
            , ABI::Windows::Media::IMediaExtension
            , IMFSchemeHandler
            , IMrvcSchemeHandler>
        {
            InspectableClass(RuntimeClass_RealtimeStreaming_Media_MrvcSchemeHandler, BaseTrust)

        public:
            MrvcSchemeHandlerImpl();
            ~MrvcSchemeHandlerImpl();

            HRESULT RuntimeClassInitialize();

            // IMediaExtension
            IFACEMETHOD(SetProperties) (
                _In_opt_ ABI::Windows::Foundation::Collections::IPropertySet* pConfiguration);

            // IMrvcSchemeHandler
            IFACEMETHOD(get_Connection)(
                _Out_ ABI::RealtimeStreaming::Network::IConnection** connection);

            IFACEMETHOD(put_Connection)(
                _In_ ABI::RealtimeStreaming::Network::IConnection *connection);

            // IMFSchemeHandler
            IFACEMETHOD(BeginCreateObject) (
                _In_ LPCWSTR pwszURL,
                _In_ DWORD dwFlags,
                _In_ IPropertyStore* pProps,
                _COM_Outptr_opt_  IUnknown** ppIUnknownCancelCookie,
                _In_ IMFAsyncCallback* pCallback,
                _In_ IUnknown* punkState);

            IFACEMETHOD(EndCreateObject) (
                _In_ IMFAsyncResult* pResult,
                _Out_ MF_OBJECT_TYPE* pObjectType,
                _COM_Outptr_ IUnknown** ppObject);

            IFACEMETHOD(CancelObjectCreation) (
                _In_ IUnknown* pIUnknownCancelCookie);

        private:
            com_ptr<ABI::RealtimeStreaming::Network::IConnection> m_connection;
        };
    }
}
