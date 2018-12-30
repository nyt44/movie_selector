#ifdef _WIN32
#include "windowsrunner.h"

#include <Windows.h>
#include <shellapi.h>
#include <locale>
#include <codecvt>

WindowsRunner::WindowsRunner(std::string path) : path_{std::move(path)}
{
}

void WindowsRunner::run()
{
    LPCWSTR mode = L"open";

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring w_sel_path = converter.from_bytes(path_);

    LPCWSTR file = w_sel_path.c_str();

    ShellExecute(nullptr, mode, file, nullptr, nullptr, SW_MAXIMIZE);
}
#endif
