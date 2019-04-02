/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

namespace BlackRoot {
namespace Util {

#define BR_U_TYPE_CHECK(name, type) \
    template <typename... Ts> using void_t = void; \
    template <typename T, typename = void> \
    struct has_##name : std::false_type {}; \
    template <typename T> \
    struct has_##name<T, void_t<typename T::type>> : std::true_type {};

}
}