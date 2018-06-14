// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "stdafx.h"
#include <locale> 
#include <codecvt>
#include <Wincrypt.h>

#pragma comment(lib, "crypt32") // for CryptBinaryToString

class StringHelper
{
public:
    // Convert a string to a wstring.
    static std::wstring s2ws(const std::string& str)
    {
        typedef std::codecvt_utf8<wchar_t> convertCodec;
        std::wstring_convert<convertCodec, wchar_t> converter;

        return converter.from_bytes(str);
    }

    // Convert a wstring to a string.
    static std::string ws2s(const std::wstring& wstr)
    {
        typedef std::codecvt_utf8<wchar_t> convertCodec;
        std::wstring_convert<convertCodec, wchar_t> converter;

        return converter.to_bytes(wstr);
    }

    // Encode a string as base64.
    static bool base64_encode(std::string source, LPTSTR& encodedString)
    {
        DWORD length = (DWORD)source.length();
        DWORD encodedSize;

        // Get the size of the destination encoded string.
        if (CryptBinaryToString((BYTE*)(source.c_str()), length, CRYPT_STRING_BASE64_ANY, nullptr, &encodedSize))
        {
            // Encode the string.
            encodedString = (LPTSTR)(HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, encodedSize * sizeof(TCHAR)));
            if (encodedString)
            {
                CryptBinaryToString((BYTE*)(source.c_str()), length, CRYPT_STRING_BASE64_ANY, encodedString, &encodedSize);
                return true;
            }
        }

        return false;
    }
};