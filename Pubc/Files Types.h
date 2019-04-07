/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <chrono>
#include <experimental/filesystem>

namespace BlackRoot {
namespace IO {

    class IFileSource;
    class IFileStream;

    class BaseFileSource;
    class BaseFile;

    using FileSize      = std::uintmax_t;
    using FileManipLength = std::size_t;
    using FileTime      = std::chrono::system_clock::time_point;

    using FilePath      = std::experimental::filesystem::path;

}
}