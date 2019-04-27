/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <string>

#include "BlackRoot/Pubc/Files Types.h"

namespace BlackRoot {
namespace Strings {

    std::string Wide_To_UTF8(const wchar_t *, size_t length);
    std::string Wide_To_UTF8(const std::wstring);

}
}
