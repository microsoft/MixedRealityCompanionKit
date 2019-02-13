// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "SchemeHandler.h"

using namespace winrt;
using namespace RealtimeStreaming::Media::implementation;
using namespace RealtimeStreaming::Media;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

_Use_decl_annotations_
RTSchemeHandler::RTSchemeHandler()
{
}

// IMediaExtension methods
static std::wstring s_connectionId{ L"Connection" };
void RTSchemeHandler::SetProperties(Windows::Foundation::Collections::IPropertySet const& configuration)
{
    Log(Log_Level_Info, L"RTSchemeHandler::SetProperties()\n");

    m_connection = configuration.Lookup(s_connectionId).as<RealtimeStreaming::Network::Connection>();
}

RealtimeStreaming::Network::Connection RTSchemeHandler::DataConnection()
{
    return m_connection;
}

void RTSchemeHandler::DataConnection(RealtimeStreaming::Network::Connection const& connection)
{
    m_connection = connection;
}

// IMFSchemeHandler
_Use_decl_annotations_
HRESULT RTSchemeHandler::BeginCreateObject(
    LPCWSTR pwszURL, 
    DWORD dwFlags,
    IPropertyStore* pProps,
    IUnknown** ppCancelCookie,
    IMFAsyncCallback* pCallback,
    IUnknown* punkState)
{
    Log(Log_Level_Info, L"RTSchemeHandler::BeginCreateObject()\n");
    
    NULL_CHK(pwszURL);
    NULL_CHK(pCallback);

    if ((dwFlags & MF_RESOLUTION_MEDIASOURCE) == 0)
    {
        IFT(MF_E_UNSUPPORTED_BYTESTREAM_TYPE);
    }

    if (nullptr != ppCancelCookie)
    {
       * ppCancelCookie = nullptr;
    }

    RealtimeMediaSource spMediaSource = make<RealtimeMediaSource>();
    
    com_ptr<IUnknown> spSourceUnk;
    IFT(spMediaSource.As(&spSourceUnk));

    com_ptr<IAsyncAction> initMediaSourceOp;
    IFT(spMediaSource.as<IAsyncAction>(&initMediaSourceOp));

    com_ptr<IMFAsyncResult> spAsyncResult;
    IFT(MFCreateAsyncResult(spSourceUnk.get(), pCallback, punkState, spAsyncResult.put()));

    // until the source gets the stream descriptions, the event will not fire
    return StartAsyncThen(
        initMediaSourceOp.get(),
        [spAsyncResult](HRESULT hr, IAsyncAction *asyncResult, AsyncStatus asyncStatus) -> HRESULT
    {
        LOG_RESULT(spAsyncResult->SetStatus(hr));

        return MFInvokeCallback(spAsyncResult.get());
    });
}

_Use_decl_annotations_
HRESULT RTSchemeHandler::EndCreateObject(
    IMFAsyncResult* pResult,
    MF_OBJECT_TYPE* pObjectType,
    IUnknown** ppObject)
{
    Log(Log_Level_Info, L"RTSchemeHandler::EndCreateObject()\n");

    NULL_CHK(pResult);
    NULL_CHK(pObjectType);
    NULL_CHK(ppObject);

   * pObjectType = MF_OBJECT_INVALID;
   * ppObject = nullptr;

    IFT(pResult->GetStatus());

    com_ptr<IUnknown> spUnkSource;
    IFT(pResult->GetObject(&spUnkSource));
   * pObjectType = MF_OBJECT_MEDIASOURCE;
   * ppObject = spUnkSource.detach();

    return S_OK;
}

_Use_decl_annotations_
HRESULT RTSchemeHandler::CancelObjectCreation(
    IUnknown* pIUnknownCancelCookie)
{
    return E_NOTIMPL;
}
