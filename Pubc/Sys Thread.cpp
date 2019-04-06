/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifdef _WIN32
#include <windows.h>
#endif

#include "BlackRoot/Pubc/Sys Thread.h"
#include "BlackRoot/Pubc/Assert.h"

using namespace BlackRoot::System;

void BlackRoot::System::SetCurrentThreadPriority(ThreadPriority::Type p) {
#ifdef _WIN32
    int priority = THREAD_PRIORITY_NORMAL;
    switch (p) {
    default:
        DbAssertMsgFatal(0, "Unknown priority type");
    case ThreadPriority::Lowest:
        priority = THREAD_PRIORITY_LOWEST; break;
    case ThreadPriority::BelowNormal:
        priority = THREAD_PRIORITY_BELOW_NORMAL; break;
    case ThreadPriority::Normal:
        priority = THREAD_PRIORITY_NORMAL; break;
    case ThreadPriority::AboveNormal:
        priority = THREAD_PRIORITY_ABOVE_NORMAL; break;
    case ThreadPriority::High:
        priority = THREAD_PRIORITY_HIGHEST; break;
    };
    ::SetThreadPriority(::GetCurrentThread(), priority);
#else
#error No code for current build
#endif
    }