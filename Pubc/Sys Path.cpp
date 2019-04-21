/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#if _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#error No code for build
#endif

#include "BlackRoot/Pubc/Assert.h"
#include "BlackRoot/Pubc/Exception.h"
#include "BlackRoot/Pubc/Sys Path.h"

const char * BlackRoot::System::DirSeperator =
#ifdef _WIN32
    "\\";
#else
    "/";
#endif

BlackRoot::IO::FilePath BlackRoot::System::GetExecutablePath()
{
    // TODO: this wouldn't work for every long file names of course
    
#ifdef _WIN32
    wchar_t outFile[2048];

    DWORD length = ::GetModuleFileNameW(nullptr, outFile, 2048);
    DbAssert(length < 2048);

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    BlackRoot::IO::FilePath path = converter.to_bytes(outFile).c_str();
    path = std::experimental::filesystem::canonical(path);

    return path;
#else
#error No code for build
#endif
}

BlackRoot::IO::FilePath BlackRoot::System::GetRoamingPath()
{
#ifdef _WIN32
    wchar_t * outFile;

    if (!SUCCEEDED(::SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, 0, &outFile))) {
        throw new BlackRoot::Debug::Exception("Cannot get roaming path", BRGenDbgInfo);
    }
    
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    BlackRoot::IO::FilePath path = converter.to_bytes(outFile).c_str();
    path = std::experimental::filesystem::canonical(path);

    return path;
#else
#error No code for build
#endif
}