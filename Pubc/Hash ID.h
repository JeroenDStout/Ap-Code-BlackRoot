/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "BlackRoot/Pubc/UUID.h"
#include "BlackRoot/Pubc/Number Types.h"

namespace BlackRoot {
namespace Identify {

    struct HashSugar128 {
        char data0[19];
        char data1[19];
        char data2[19];
        char data3[19];

        constexpr HashSugar128(uint128 hash)
        : data0(), data1(), data2(), data3() {
            this->set_data(0, uint32(hash >>  0));
            this->set_data(1, uint32(hash >> 32));
            this->set_data(2, uint32(hash >> 64));
            this->set_data(3, uint32(hash >> 96));
        }

        constexpr void set_data(size_t i, uint32 value) {
            char * data = (*this)[i];
            data[0] = '0';
            data[1] = 'x';
            for (int d = 0; d < 16; d++) {
                data[2+d] = '0' + (uint8(value >> (d*4)) & 0xF);
            }
            data[18] = 0;
        }

        constexpr char * operator[](size_t i) {
            switch (i) {
              default: return nullptr;
              case 0: return data0;
              case 1: return data1;
              case 2: return data2;
              case 3: return data3;
            };
        }
    };

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

    inline namespace Literals {
        inline namespace String_Literals {

            constexpr uint128 operator "" _fnv1a_128(const char * s, std::size_t n)
            {
                return fnv1a_128(s, n);
            }

        }
    }

}
}