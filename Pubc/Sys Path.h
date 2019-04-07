/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <experimental/filesystem>

#include "BlackRoot/Pubc/Files Types.h"

namespace BlackRoot {
namespace System {

    extern const char * DirSeperator;

    inline BlackRoot::IO::FilePath GetCurrentPath()
    {
        return std::experimental::filesystem::current_path();
    }

    inline BlackRoot::IO::FilePath MakePathCanonical(BlackRoot::IO::FilePath path)
    {
        return std::experimental::filesystem::canonical(path);
    }

}
}
