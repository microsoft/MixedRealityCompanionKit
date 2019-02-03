// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include <mfidl.h>

namespace ABI
{
    namespace MixedRemoteViewCompositor
    {
        namespace Media
        {
            interface DECLSPEC_UUID("6f17b4ae-f506-4d6e-a666-893638581b19") DECLSPEC_NOVTABLE
                IMarker : public IUnknown
            {
            public:
                virtual HRESULT STDMETHODCALLTYPE GetMarkerType(
                    /* [out] */ MFSTREAMSINK_MARKER_TYPE* pType) = 0;
                virtual HRESULT STDMETHODCALLTYPE GetMarkerValue(
                    /* [out] */ PROPVARIANT* pVar) = 0;
                virtual HRESULT STDMETHODCALLTYPE GetContext(
                    /* [out] */ PROPVARIANT* pVar) = 0;
            };

            extern const __declspec(selectany) IID & IID_IMarker = __uuidof(IMarker);
        }
    }
}

namespace MixedRemoteViewCompositor
{
    namespace Media
    {
        class MarkerImpl
            : public IMarker
        {
        public:
            static HRESULT Create(
                MFSTREAMSINK_MARKER_TYPE eMarkerType,
                _In_opt_ const PROPVARIANT* pvarMarkerValue,
                _In_opt_ const PROPVARIANT* pvarContextValue,
                _Outptr_ IMarker** ppMarker);

            // IUnknown methods.
            IFACEMETHOD(QueryInterface) (REFIID riid, _COM_Outptr_ void** ppv);
            IFACEMETHOD_(ULONG, AddRef) ();
            IFACEMETHOD_(ULONG, Release) ();

            // Inherited via RuntimeClass
            IFACEMETHOD(GetMarkerType)(
                MFSTREAMSINK_MARKER_TYPE* pType);
            IFACEMETHOD(GetMarkerValue)(
                PROPVARIANT* pVar);
            IFACEMETHOD(GetContext)(
                PROPVARIANT* pVar);

        private:
            MarkerImpl(MFSTREAMSINK_MARKER_TYPE eMarkerType);
            ~MarkerImpl();

        private:
            long _cRef;

            MFSTREAMSINK_MARKER_TYPE _eMarkerType;
            PROPVARIANT _varMarkerValue;
            PROPVARIANT _varContextValue;
        };
    }
}