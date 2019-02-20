#include "pch.h"
#include "Media.RTSchemeHandler.h"

namespace winrt::RealtimeStreaming::Media::implementation
{
    RealtimeStreaming::Network::Connection RTSchemeHandler::DataConnection()
    {
        throw hresult_not_implemented();
    }

    void RTSchemeHandler::DataConnection(RealtimeStreaming::Network::Connection const& value)
    {
        throw hresult_not_implemented();
    }

    void RTSchemeHandler::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
    {
        throw hresult_not_implemented();
    }
}
