/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <sstream>

template<typename Type>
std::stringstream& operator<<(std::stringstream& ss, const Type& type)
{
    static_cast<std::ostream&>(ss) << type;
    return ss;
}

template<typename Type>
std::stringstream& operator<<(std::stringstream&& ss, const Type& type)
{
    static_cast<std::ostream&>(ss) << type;
    return ss;
}

template<typename Type>
std::stringstream& operator>>(std::stringstream& ss, Type& type)
{
    static_cast<std::istream&>(ss) >> type;
    return ss;
}

template<typename Type>
std::stringstream& operator>>(std::stringstream&& ss, Type& type)
{
    static_cast<std::istream&>(ss) >> type;
    return ss;
}