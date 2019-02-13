// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "pch.h"

#ifndef FAST_FAIL_ON_ERRORS
#define FAST_FAIL_ON_ERRORS 0
#endif

inline void __stdcall FastFail(HRESULT hr)
{
    if (FAILED(hr))
    {
        EXCEPTION_RECORD record = {};
        record.ExceptionCode = EXCEPTION_NONCONTINUABLE_EXCEPTION;
        record.NumberParameters = 1;
        record.ExceptionInformation[0] = (ULONG_PTR)hr;

        if (IsDebuggerPresent())
        {
            __debugbreak();
        }
        else
        {
            RaiseFailFastException(&record, nullptr, 0);
        }
    }
}

// ensure function call
// generates a valid HRESULT
template <typename TLambda>
inline HRESULT __stdcall ExceptionBoundary(_In_ TLambda&& lambda, _In_ boolean fastfail = false) throw()
{
    try
    {
        return lambda();
    }
#ifdef __cplusplus_winrt
    catch (Platform::Exception^ ex)
    {
        return ex->HResult;
    }
#endif __cplusplus_winrt
    catch (const std::bad_alloc&)
    {
        return E_OUTOFMEMORY;
    }
    catch (const std::out_of_range&)
    {
        return MF_E_INVALIDINDEX;
    }
    catch (const std::domain_error&)
    {
        return E_INVALIDARG;
    }
    catch (const std::invalid_argument&)
    {
        return E_INVALIDARG;
    }
    catch (const std::length_error&)
    {
        return E_BOUNDS;
    }
    catch (const std::logic_error&)
    {
        return E_UNEXPECTED;
    }
    catch (const std::overflow_error&)
    {
        return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
    }
    catch (const std::range_error&)
    {
        return E_FAIL;
    }
    catch (const std::underflow_error&)
    {
        return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
    }
    catch (...)
    {
        if (fastfail)
        {
            FastFail(E_FAIL);
        }

        return E_FAIL;
    }
}

typedef enum Log_Level
{
    Log_Level_Any,
    Log_Level_Critical,
    Log_Level_Error,
    Log_Level_Warning,
    Log_Level_Info,
    Log_Level_All
} Log_Level;

#ifndef LOG_LEVEL
#if _DEBUG
#define LOG_LEVEL Log_Level_Warning
#else
#define LOG_LEVEL Log_Level_Error
#endif
#endif

inline void __stdcall Log(
    Log_Level level,
    _In_ _Printf_format_string_ STRSAFE_LPCWSTR pszFormat,
    ...)
{
    if (LOG_LEVEL < level)
    {
		// TODO: Uncomment this
        //return;
    }

    wchar_t szTextBuf[2048];

    va_list args;
    va_start(args, pszFormat);

    StringCchVPrintf(szTextBuf, _countof(szTextBuf), pszFormat, args);

    va_end(args);

    OutputDebugStringW(szTextBuf);
}

#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100

inline const TCHAR * ErrorMessage(HRESULT hr)
{
    TCHAR * pszMsg;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&pszMsg,
        0,
        NULL);
    if (pszMsg != NULL) {
#ifdef UNICODE
        size_t const nLen = wcslen(pszMsg);
#else
        size_t const nLen = strlen(m_pszMsg);
#endif
        if (nLen > 1 && pszMsg[nLen - 1] == '\n') {
            pszMsg[nLen - 1] = 0;
            if (pszMsg[nLen - 2] == '\r') {
                pszMsg[nLen - 2] = 0;
            }
        }
    }

    return pszMsg;
}

inline void __stdcall LogResult(
    _In_ LPWSTR pszFile, //__FILEW__
    _In_ LPWSTR pszFunc, //__FUNCTIONW__
    _In_ long nLine, //__LINE__
    _In_ HRESULT hr,
    _In_ LPCWSTR message = L"")
{
    if (SUCCEEDED(hr))
    {
        return;
    }

    // if filename contains a path, move the ptr to the first char of name
    LPWSTR psz = pszFile + wcsnlen_s(pszFile, 255);
    for (; psz > pszFile; psz--)
    {
        if (*psz == L'\\')
        {
            pszFile = psz + 1;
            break;
        }
    }

    // Gets the plain text error message
    Log(
        Log_Level_Warning,
        L"%sHR: 0x%x - %s\r\n\t%s(%d): %s\n"
        , message, hr, ErrorMessage(hr), pszFile, nLine, pszFunc);

#if FAST_FAIL_ON_ERRORS
    FastFail(hr);
#endif
}

// #define LOG_RESULT_MSG(hr, message, ...)
//LogResult(__FILEW__, __FUNCTIONW__, __LINE__, hr, message, __VA_ARGS__);
// &va_list args -> _In_opt_ va_list* pArgList
#ifndef LOG_RESULT
#define LOG_RESULT_MSG(hrToCheck, message) LogResult(__FILEW__, __FUNCTIONW__, __LINE__, hrToCheck, message);
#define LOG_RESULT(hrToCheck) LOG_RESULT_MSG(hrToCheck, L"");
#endif

#ifndef IFT
#define IFT(result) { HRESULT hrTest = result; if (FAILED(hrTest)) { winrt::throw_hresult(hrTest); } }
#endif

#ifndef IFC 
#define IFC_MSG(hrToCheck, message) if (FAILED(hrToCheck)) { LOG_RESULT_MSG(hrToCheck, message); hr = hrToCheck; goto done; }
#define IFC(hrToCheck) IFC_MSG(hrToCheck, L"CHECK_")
#endif

#ifndef IFR
#define IFR_MSG(hrToCheck, message) if (FAILED(hrToCheck)) { LOG_RESULT_MSG(hrToCheck, message); return hrToCheck; }
#define IFT(hrToCheck) IFR_MSG(hrToCheck, L"RETURN_")
#endif

#ifndef IFRV
#define IFRV_MSG(hrToCheck, message) { if (FAILED(hrToCheck)) LOG_RESULT_MSG(hrToCheck, message); return; } }
#define IFRV(hrToCheck) { IFRV_MSG(hrToCheck, L"IFRV_"); }
#endif

#ifndef IFRN 
#define IFRN_MSG(hrToCheck, message) { if (FAILED(hrToCheck)) LOG_RESULT_MSG(hrToCheck, message); return nullptr; } }
#define IFRN(hrToCheck) { IFRN_MSG(hrToCheck, L"IFRN_"); }
#endif

#ifndef NULL_CHK
#define NULL_CHK_HR_MSG(pointer, hrToCheck, message) if(nullptr == pointer) { IFR_MSG(hrToCheck, message); return hrToCheck; }
#define NULL_CHK_HR(pointer, hrToCheck) NULL_CHK_HR_MSG(pointer, hrToCheck, L"NULL_CHK_")
#define NULL_CHK(pointer) NULL_CHK_HR(pointer, E_INVALIDARG)
#endif

#ifndef ENSURE_HR  
#define ENSURE_HR_MSG(hrToCheck, message) { IFR_MSG(hrToCheck, message); FastFail(hrToCheck); }
#define ENSURE_HR(hrToCheck) { ENSURE_HR_MSG(hrToCheck, L"ERROR!!! "); }
#endif

inline boolean CheckThread(DWORD createdThread)
{
    DWORD currentId = GetCurrentThreadId();
    Log(Log_Level::Log_Level_Any, L"\nCurrent Thread: %d - Created On: %d\n", currentId, createdThread);

    return (currentId == createdThread);
}
