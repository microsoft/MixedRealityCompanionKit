// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "pch.h"
#include <wrl.h>
#include <wrl\async.h>
#include <Windows.System.Threading.h>
#include <functional>
#include "ErrorHandling.h"

// T: Type of the parent object
template<class T>
class AsyncCallback
    : public RuntimeClass
    < RuntimeClassFlags<ClassicCom>
    , IMFAsyncCallback >
{
public:
    typedef HRESULT(T::*InvokeFn)(IMFAsyncResult* pAsyncResult);

    AsyncCallback(T* pParent, InvokeFn fn) 
        : _spParent(pParent)
        , _pInvokeFn(fn)
    {
    }

    // IMFAsyncCallback methods
    STDMETHODIMP GetParameters(DWORD*, DWORD*)
    {
        // Implementation of this method is optional.
        return E_NOTIMPL;
    }

    STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult)
    {
        return (_spParent.get()->*_pInvokeFn)(pAsyncResult);
    }

private:
    com_ptr<T> _spParent;
    InvokeFn _pInvokeFn;
};

template <typename TDelegate, typename TOperation, typename TLambda>
HRESULT StartAsyncThen(_In_ TOperation* pOperation, _In_ TLambda&& tFunc)
{
    if (nullptr == pOperation)
    {
        return E_INVALIDARG;
    }

    auto spCallback = Callback<TDelegate>(
        [tFunc](_In_ TOperation* pOperation, _In_ AsyncStatus status) -> HRESULT
        {
            HRESULT hr = S_OK;

            // wrap the operation
            if (status != AsyncStatus::Completed)
            {
                com_ptr<TOperation> spOperation(pOperation);
                com_ptr<IAsyncInfo> spAsyncInfo;
                hr = spOperation.As(&spAsyncInfo);
                if (SUCCEEDED(hr))
                {
                    spAsyncInfo->get_ErrorCode(&hr);
                }
            }

            return tFunc(hr, pOperation, status);
        });

    // start
    return (nullptr != spCallback) ? pOperation->put_Completed(spCallback.get()) : E_OUTOFMEMORY;
}
template <typename TLambda>
HRESULT StartAsyncThen(_In_ ABI::Windows::Foundation::IAsyncAction* pOperation, _In_ TLambda&& tFunc)
{
    return StartAsyncThen<ABI::Windows::Foundation::IAsyncActionCompletedHandler, ABI::Windows::Foundation::IAsyncAction>(pOperation, static_cast<TLambda&&>(tFunc));
}
template <typename TProgress, typename TLambda>
HRESULT StartAsyncThen(_In_ ABI::Windows::Foundation::IAsyncActionWithProgress<TProgress>* pOperation, _In_ TLambda&& tFunc)
{
    return StartAsyncThen<ABI::Windows::Foundation::IAsyncActionWithProgressCompletedHandler<TProgress>, Windows::Foundation::IAsyncActionWithProgress<TProgress>>(pOperation, static_cast<TLambda&&>(tFunc));
}
template <typename TResult, typename TLambda>
HRESULT StartAsyncThen(_In_ ABI::Windows::Foundation::IAsyncOperation<TResult>* pOperation, _In_ TLambda&& tFunc)
{
    return StartAsyncThen<ABI::Windows::Foundation::IAsyncOperationCompletedHandler<TResult>, ABI::Windows::Foundation::IAsyncOperation<TResult>>(pOperation, static_cast<TLambda&&>(tFunc));
}
template <typename TResult, typename TProgress, typename TLambda>
HRESULT StartAsyncThen(_In_ ABI::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>* pOperation, _In_ TLambda&& tFunc)
{
    return StartAsyncThen<ABI::Windows::Foundation::IAsyncOperationWithProgressCompletedHandler<TResult, TProgress>, ABI::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>>(pOperation, static_cast<TLambda&&>(tFunc));
}


// eg. TOperation   = IAsyncOperationWithProgress<UINT32, UINT32>
// eg. THandler     = IAsyncOperationWithProgressCompletedHandler<UINT, UINT>
template<typename TOperation, typename THandler>
class AsyncEventDelegate
    : public RuntimeClass
    < RuntimeClassFlags<RuntimeClassType::Delegate>
    , THandler
    , FtmBase >
{
public:
    AsyncEventDelegate()
        : _completedEvent(CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS))
    {
        com_ptr<AsyncEventDelegate> spThis(this);
        auto lambda = ([this, spThis](_In_ HRESULT hr, _In_ TOperation* pOperation)
        {
            SetEvent(_completedEvent.get());
        });
        _func = std::move(lambda);
    }

    STDMETHOD(Invoke)(
        _In_ TOperation* pOperation,
        _In_ AsyncStatus status)
    {
        HRESULT hr = S_OK;

        // if we completed successfully, then there is no need for getting hresult
        if (status != AsyncStatus::Completed)
        {
            com_ptr<TOperation> spOperation(pOperation);
            com_ptr<IAsyncInfo> spAsyncInfo;
            if (SUCCEEDED(spOperation.As(&spAsyncInfo)))
            {
                spAsyncInfo->get_ErrorCode(&hr);
            }
        }

        _func(hr, pOperation);

        return S_OK;
    }

    STDMETHOD(SyncWait)(_In_ TOperation* pOperation, _In_ DWORD dwMilliseconds)
    {
        HRESULT hr = pOperation->put_Completed(this);
        if (FAILED(hr))
        {
            return hr;
        }

        DWORD dwWait = WaitForSingleObjectEx(_completedEvent.get(), dwMilliseconds, TRUE);
        if (WAIT_IO_COMPLETION == dwWait || WAIT_OBJECT_0 == dwWait)
            return S_OK;
        
        return HRESULT_FROM_WIN32(GetLastError());
    }

private:
    std::function<void(HRESULT, TOperation*)> _func;
    Wrappers::Event _completedEvent;
};
template <typename TOperation, typename THandler>
HRESULT SyncWait(_In_ TOperation* pOperation, _In_ DWORD dwMilliseconds)
{
    auto spCallback = Make<AsyncEventDelegate<TOperation, THandler>>();

    return spCallback->SyncWait(pOperation, dwMilliseconds);
}
template <typename TResult>
HRESULT SyncWait(_In_ ABI::Windows::Foundation::IAsyncAction* pOperation, _In_ DWORD dwMilliseconds = INFINITE)
{
    return SyncWait<ABI::Windows::Foundation::IAsyncAction, ABI::Windows::Foundation::IAsyncActionCompletedHandler>(pOperation, dwMilliseconds);
}
template <typename TResult>
HRESULT SyncWait(_In_ ABI::Windows::Foundation::IAsyncOperation<TResult>* pOperation, _In_ DWORD dwMilliseconds = INFINITE)
{
    return SyncWait<ABI::Windows::Foundation::IAsyncOperation<TResult>, ABI::Windows::Foundation::IAsyncOperationCompletedHandler<TResult>>(pOperation, dwMilliseconds);
}
template <typename TResult, typename TProgress>
HRESULT SyncWait(_In_ ABI::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>* pOperation, _In_ DWORD dwMilliseconds = INFINITE)
{
    return SyncWait<ABI::Windows::Foundation::IAsyncOperationWithProgress<TResult, TProgress>, ABI::Windows::Foundation::IAsyncOperationWithProgressCompletedHandler<TResult, TProgress>>(pOperation, dwMilliseconds);
}
