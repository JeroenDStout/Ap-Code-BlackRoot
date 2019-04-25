/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#if _WIN32
#include <windows.h>
#else
#error No code for build
#endif

#include "BlackRoot/Pubc/Assert.h"
#include "BlackRoot/Pubc/Sys Sound.h"

void BlackRoot::System::PlayAdHocSound(const BlackRoot::IO::FilePath path)
{
    namespace fs = std::experimental::filesystem;

    DbAssert(fs::is_regular_file(path));
    
#if _WIN32
    std::wstring sound_path = L"\\\\?\\";
    sound_path += path;
    
    ::PlaySound(sound_path.c_str(), NULL, SND_FILENAME | SND_ASYNC);
#else
#error No code for build
#endif
}