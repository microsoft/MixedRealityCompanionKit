// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "Marker.h"

_Use_decl_annotations_
MarkerImpl::MarkerImpl(MFSTREAMSINK_MARKER_TYPE eMarkerType)
    : _cRef(1)
    , _eMarkerType(eMarkerType)
{
    ZeroMemory(&_varMarkerValue, sizeof(_varMarkerValue));
    ZeroMemory(&_varContextValue, sizeof(_varContextValue));
}

MarkerImpl::~MarkerImpl()
{
    PropVariantClear(&_varMarkerValue);
    PropVariantClear(&_varContextValue);
}

_Use_decl_annotations_
HRESULT MarkerImpl::Create(
    MFSTREAMSINK_MARKER_TYPE eMarkerType,
    const PROPVARIANT* pvarMarkerValue,     // Can be NULL.
    const PROPVARIANT* pvarContextValue,    // Can be NULL.
    IMarker** ppMarker)
{
    if (nullptr == ppMarker)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;

    Microsoft::WRL::ComPtr<MarkerImpl> spMarker;
    spMarker.Attach(new (std::nothrow) MarkerImpl(eMarkerType));
    if (spMarker == nullptr)
    {
        hr = E_OUTOFMEMORY;
    }

    // Copy the marker data.
    if (SUCCEEDED(hr))
    {
        if (pvarMarkerValue)
        {
            hr = PropVariantCopy(&spMarker->_varMarkerValue, pvarMarkerValue);
        }
    }

    if (SUCCEEDED(hr))
    {
        if (pvarContextValue)
        {
            hr = PropVariantCopy(&spMarker->_varContextValue, pvarContextValue);
        }
    }

    if (SUCCEEDED(hr))
    {
       * ppMarker = spMarker.Detach();
    }

    return hr;
}

// IUnknown methods.
_Use_decl_annotations_
IFACEMETHODIMP_(ULONG) MarkerImpl::AddRef()
{
    return InterlockedIncrement(&_cRef);
}

_Use_decl_annotations_
IFACEMETHODIMP_(ULONG) MarkerImpl::Release()
{
    ULONG cRef = InterlockedDecrement(&_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

_Use_decl_annotations_
IFACEMETHODIMP MarkerImpl::QueryInterface(REFIID riid, void** ppv)
{
    if (ppv == nullptr)
    {
        return E_POINTER;
    }

    (*ppv) = nullptr;

    HRESULT hr = S_OK;

    if (riid == IID_IUnknown || riid == __uuidof(IMarker))
    {
        (*ppv) = static_cast<IMarker*>(this);
        AddRef();
    }
    else
    {
        hr = E_NOINTERFACE;
    }

    return hr;
}

_Use_decl_annotations_
HRESULT MarkerImpl::GetMarkerType(
    MFSTREAMSINK_MARKER_TYPE* pType)
{
    NULL_CHK(pType);

   * pType = _eMarkerType;

    return S_OK;
}

_Use_decl_annotations_
HRESULT MarkerImpl::GetMarkerValue(PROPVARIANT* pVar)
{
    NULL_CHK(pVar);

    return PropVariantCopy(pVar, &_varMarkerValue);
}

_Use_decl_annotations_
HRESULT MarkerImpl::GetContext(PROPVARIANT* pVar)
{
    NULL_CHK(pVar);

    return PropVariantCopy(pVar, &_varContextValue);
}
