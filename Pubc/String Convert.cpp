/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#if _WIN32
#include <windows.h>
#else
#error No code for build
#endif

#include "Pubc/String Convert.h"

std::string BlackRoot::Strings::Wide_To_UTF8(const wchar_t *str, size_t length)
{
    std::string ret;

    int utf8_len = ::WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    ret.resize(utf8_len);

    ::WideCharToMultiByte(CP_UTF8, 0, str, -1, &ret.front(), utf8_len, NULL, NULL);
    return ret;
}

std::string BlackRoot::Strings::Wide_To_UTF8(const std::wstring wstr)
{
    return Wide_To_UTF8(wstr.c_str(), wstr.length());
}