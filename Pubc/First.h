/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

namespace BlackRoot {
namespace Util {

        // Simple utility to be used in loops to
        // keep track of whether an item is the first

    struct First {
    protected:
        bool    _First;

    public:
        First()
         : _First(true) { ; }

        inline void reset() {
            _First = true;
        }

        inline operator bool() {
            bool value = _First;
            _First = false;
            return value;
        }
    };

}
}