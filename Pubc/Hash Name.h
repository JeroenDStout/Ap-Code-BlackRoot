/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "BlackRoot/-genc/Hash Names.h"
#include "BlackRoot/Pubc/Number Types.h"

namespace BlackRoot {
namespace Identify {

    struct HashName32 {
        char name[
          Tables::Adjective::max_length + 1 +
          Tables::First::max_length + 1 +
          Tables::Family::max_length + 2 +
          Tables::City::max_length + 1
        ];

        constexpr HashName32()
          : name() {}

        constexpr char const * get() {
            return name;
        }
    };

    constexpr HashName32 generate_hash_name_32(uint32 data) {
        HashName32 ret;

        char const * adj    = Tables::Adjective::get( 0xFF & (data >> 24) );
        char const * first  = Tables::First::get(     0xFF & (data >> 16) );
        char const * family = Tables::Family::get(    0xFF & (data >>  8) );
        char const * city   = Tables::City::get(      0xFF & (data >>  0) );

        size_t write_point = 0;
        char const * parcer = nullptr;
        
        parcer = adj;
        while (*parcer != 0) {
            ret.name[write_point++] = *(parcer++);
        }
        ret.name[write_point++] = ' ';
        
        parcer = first;
        while (*parcer != 0) {
            ret.name[write_point++] = *(parcer++);
        }
        ret.name[write_point++] = ' ';
        
        parcer = family;
        while (*parcer != 0) {
            ret.name[write_point++] = *(parcer++);
        }
        ret.name[write_point++] = ',';
        ret.name[write_point++] = ' ';
        
        parcer = city;
        while (*parcer != 0) {
            ret.name[write_point++] = *(parcer++);
        }
        ret.name[write_point++] = 0x0;

        return ret;
    }

}
}