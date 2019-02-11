// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "SchemeHandler.h"

ActivatableClass(MrvcSchemeHandlerImpl);

_Use_decl_annotations_
MrvcSchemeHandlerImpl::MrvcSchemeHandlerImpl()
    : _connection(nullptr)
{
}

_Use_decl_annotations_
MrvcSchemeHandlerImpl::~MrvcSchemeHandlerImpl()
{
}

_Use_decl_annotations_
HRESULT MrvcSchemeHandlerImpl::RuntimeClassInitialize()
{
    return S_OK;
}


// IMediaExtension methods
_Use_decl_annotations_
HRESULT MrvcSchemeHandlerImpl::SetProperties(
    IPropertySet* propertySet)
{
    NULL_CHK(propertySet);

    com_ptr<IPropertySet> spProperties(propertySet);

    com_ptr<IMap<HSTRING, IInspectable*>> spMap;
    check_hresult(spProperties.As(&spMap));

    com_ptr<IConnection> spConnection;
    check_hresult(spMap->Lookup(Wrappers::HStringReference(L"Connection").get(), &spConnection));

    HRESULT hr = E_INVALIDARG;

    if (nullptr != spConnection)
    {
        hr = spConnection.As(&_connection);
    }

    return hr;
}


// IMrvcSchemeHandler
_Use_decl_annotations_
HRESULT MrvcSchemeHandlerImpl::get_Connection(
    _Out_ IConnection** ppConnection)
{
    NULL_CHK_HR(_connection, E_NOT_SET);

    return _connection.CopyTo(ppConnection);
}

_Use_decl_annotations_
HRESULT MrvcSchemeHandlerImpl::put_Connection(
    _In_ IConnection *connection) 
{
    NULL_CHK(connection);

    com_ptr<IConnection> spConnection(connection);

    return spConnection.As(&_connection);
}


// IMFSchemeHandler
_Use_decl_annotations_
HRESULT MrvcSchemeHandlerImpl::BeginCreateObject(
    LPCWSTR pwszURL, 
    DWORD dwFlags,
    IPropertyStore* pProps,
    IUnknown** ppCancelCookie,
    IMFAsyncCallback* pCallback,
    IUnknown* punkState)
{
    Log(Log_Level_Info, L"MrvcSchemeHandler::BeginCreateObject()\n");
    
    NULL_CHK(pwszURL);
    NULL_CHK(pCallback);

    if ((dwFlags & MF_RESOLUTION_MEDIASOURCE) == 0)
    {
        check_hresult(MF_E_UNSUPPORTED_BYTESTREAM_TYPE);
    }

    if (nullptr != ppCancelCookie)
    {
       * ppCancelCookie = nullptr;
    }

    com_ptr<IRealtimeMediaSource> spMediaSource;
    check_hresult(MakeAndInitialize<RealtimeMediaSource>(&spMediaSource, _connection.get()));

    com_ptr<IUnknown> spSourceUnk;
    check_hresult(spMediaSource.As(&spSourceUnk));

    com_ptr<IAsyncAction> initMediaSourceOp;
    check_hresult(spMediaSource.As(&initMediaSourceOp));

    com_ptr<IMFAsyncResult> spAsyncResult;
    check_hresult(MFCreateAsyncResult(spSourceUnk.get(), pCallback, punkState, &spAsyncResult));

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
HRESULT MrvcSchemeHandlerImpl::EndCreateObject(
    IMFAsyncResult* pResult,
    MF_OBJECT_TYPE* pObjectType,
    IUnknown** ppObject)
{
    Log(Log_Level_Info, L"MrvcSchemeHandler::EndCreateObject()\n");

    NULL_CHK(pResult);
    NULL_CHK(pObjectType);
    NULL_CHK(ppObject);

   * pObjectType = MF_OBJECT_INVALID;
   * ppObject = nullptr;

    check_hresult(pResult->GetStatus());

    com_ptr<IUnknown> spUnkSource;
    check_hresult(pResult->GetObject(&spUnkSource));
   * pObjectType = MF_OBJECT_MEDIASOURCE;
   * ppObject = spUnkSource.detach();

    return S_OK;
}

_Use_decl_annotations_
HRESULT MrvcSchemeHandlerImpl::CancelObjectCreation(
    IUnknown* pIUnknownCancelCookie)
{
    return E_NOTIMPL;
}
