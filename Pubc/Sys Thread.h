/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "BlackRoot/Pubc/Number Types.h"

namespace BlackRoot {
namespace System {

    struct ThreadPriority {
        using Type = uint8;
        enum {
            Lowest,
            BelowNormal,
            Normal,
            AboveNormal,
            High
        };
    };

    void SetCurrentThreadPriority(ThreadPriority::Type);

}
}