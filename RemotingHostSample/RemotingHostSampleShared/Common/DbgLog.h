
static void DebugLog(_In_z_ LPCWSTR format, ...)
{
    wchar_t buffer[1024];
    LPWSTR bufEnd = nullptr;

    va_list args;
    va_start(args, format);
    HRESULT hr = StringCchVPrintfExW(buffer, _countof(buffer), &bufEnd, nullptr, STRSAFE_FILL_BEHIND_NULL | STRSAFE_FILL_ON_FAILURE, format, args);

    if (SUCCEEDED(hr))
    {
        if (*bufEnd != L'\n')
        {
            StringCchCatW(buffer, _countof(buffer), L"\r\n");
        }

        OutputDebugStringW(buffer);
    }

    va_end(args);
}