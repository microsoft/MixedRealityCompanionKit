// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

namespace MixedRemoteViewCompositor
{
    namespace Network
    {
        class DataBundleArgsImpl
            : public Microsoft::WRL::RuntimeClass
            < Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::RuntimeClassType::WinRtClassicComMix>
            , ABI::MixedRemoteViewCompositor::Network::IBundleReceivedArgs
            , FtmBase >
        {
            InspectableClass(RuntimeClass_MixedRemoteViewCompositor_Network_BundleReceivedArgs, BaseTrust);

        public:
            ~DataBundleArgsImpl();

            // Activation
            IFACEMETHODIMP ActivateInstance(
                _COM_Outptr_opt_result_maybenull_ IInspectable** ppvObject);

            // RuntimeClass
            STDMETHODIMP RuntimeClassInitialize(
                _In_ PayloadType payloadType,
                _In_ ABI::MixedRemoteViewCompositor::Network::IConnection *connection,
                _In_ ABI::MixedRemoteViewCompositor::Network::IDataBundle *bundle,
                _In_ ABI::Windows::Foundation::IUriRuntimeClass *uri);

            // IDataBundleArgs
            IFACEMETHOD(get_PayloadType)(
                _Out_ PayloadType* payloadType);
            IFACEMETHOD(get_Connection)(
                _COM_Outptr_ ABI::MixedRemoteViewCompositor::Network::IConnection** connection);
            IFACEMETHOD(get_DataBundle)(
                _COM_Outptr_ ABI::MixedRemoteViewCompositor::Network::IDataBundle** bundle);
            IFACEMETHOD(get_RemoteUri)(
                _COM_Outptr_ ABI::Windows::Foundation::IUriRuntimeClass** uri);

        private:
            PayloadType   _payloadType;
            ComPtr<ABI::MixedRemoteViewCompositor::Network::IConnection>    _connection;
            ComPtr<ABI::MixedRemoteViewCompositor::Network::IDataBundle>    _bundle;
            ComPtr<ABI::Windows::Foundation::IUriRuntimeClass>              _uri;
        };
    }
}
