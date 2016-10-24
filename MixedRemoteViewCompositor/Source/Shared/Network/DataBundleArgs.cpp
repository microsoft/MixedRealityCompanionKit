// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "DataBundleArgs.h"

DataBundleArgsImpl::~DataBundleArgsImpl()
{
    Log(Log_Level_Info, L"DataBundleArgsImpl::~DataBundleArgsImpl()\n");
}

_Use_decl_annotations_
HRESULT DataBundleArgsImpl::ActivateInstance(
    IInspectable** ppvObject)
{
    NULL_CHK(ppvObject);

    // No default activation allowed for PluginManager
    // use get_Instance(), .Instance
   * ppvObject = nullptr;

    return E_NOTIMPL;
}

_Use_decl_annotations_
HRESULT DataBundleArgsImpl::RuntimeClassInitialize(
    PayloadType operation,
    IConnection *connection,
    IDataBundle *bundle,
    IUriRuntimeClass *uri)
{
    NULL_CHK(connection);
    NULL_CHK(bundle);
    NULL_CHK(uri);

    _connection = connection;
    _bundle = bundle;
    _uri = uri;
    _payloadType = operation;

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBundleArgsImpl::get_PayloadType(
    PayloadType* payloadType)
{
    NULL_CHK(payloadType);

   * payloadType = _payloadType;

    return S_OK;
}

_Use_decl_annotations_
HRESULT DataBundleArgsImpl::get_Connection(
    IConnection** ppConnection)
{
    NULL_CHK_HR(_connection, E_NOT_SET);

    return _connection.CopyTo(ppConnection);
}

_Use_decl_annotations_
HRESULT DataBundleArgsImpl::get_DataBundle(
    IDataBundle** ppDataBundle)
{
    NULL_CHK_HR(_bundle, E_NOT_SET);

    return _bundle.CopyTo(ppDataBundle);
}

_Use_decl_annotations_
HRESULT DataBundleArgsImpl::get_RemoteUri(
    ABI::Windows::Foundation::IUriRuntimeClass** uri)
{
    NULL_CHK_HR(_uri, E_NOT_SET);

    return _uri.CopyTo(uri);
}
