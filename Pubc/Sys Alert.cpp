/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#if _WIN32
#include <windows.h>
#else
#error No code for build
#endif

#include "BlackRoot/Pubc/Sys Alert.h"

void BlackRoot::System::FlashCurrentWindow()
{
    FLASHWINFO fi;
    fi.cbSize = sizeof(FLASHWINFO);
    fi.hwnd = ::GetActiveWindow();
    if (fi.hwnd == nullptr) {
        fi.hwnd = ::GetConsoleWindow();
    }
    fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
    fi.uCount = 3;
    fi.dwTimeout = 250;
    ::FlashWindowEx(&fi);
}