/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "BlackRoot/Pubc/JSON.h"
#include "BlackRoot/Pubc/Files.h"

namespace BlackRoot {
namespace Util {

    struct JSONMerge {
        BlackRoot::IO::IFileSource * FileManager;

        BlackRoot::Format::JSON * JSON;
        
        JSONMerge(BlackRoot::IO::IFileSource *, BlackRoot::Format::JSON *);

        void MergeRecursively();
    };

}
}