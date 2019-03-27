/*
 *
 *  © Stout Games 2019
 *
 */

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