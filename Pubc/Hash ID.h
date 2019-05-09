/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

 /*
  */

#pragma once

#include "BlackRoot/Pubc/UUID.h"
#include "BlackRoot/Pubc/Number Types.h"

namespace BlackRoot {
namespace Identify {

    namespace Detail {

        constexpr auto fnv1a_128(char const * s, std::size_t count) -> uint128 {
            const uint128 prime( 0x0000000001000000, 0x000000000000013B);
            const uint128 offset(0x6c62272e07bb0142, 0x62b821756295c58d);

            uint128 ret = offset;
            for (std::size_t i = 0; i < count; i++) {
                ret *= prime;
                ret ^= s[count];
            }

            return ret;
        }

    }

    inline namespace Literals {
        inline namespace String_Literals {

            constexpr uint128 operator "" _fnv1a_128(const char * s, std::size_t n)
            {
                return Detail::fnv1a_128(s, n);
            }

        }
    }

}
}