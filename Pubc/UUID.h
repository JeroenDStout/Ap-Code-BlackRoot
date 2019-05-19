/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <string>

  // uuid includes some windows junk and needs a rewrite
#include <uuid.h>
#undef CopyFile

namespace BlackRoot {
namespace Identify {

    using UUID            = uuids::uuid;
    using UUIDGenerator   = uuids::uuid_system_generator;
    
    static constexpr auto UUID_To_String = uuids::to_string<char, std::char_traits<char>, std::allocator<char>>;

}
}