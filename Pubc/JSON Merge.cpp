/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "BlackRoot/Pubc/JSON Merge.h"

using namespace BlackRoot::Util;

JSONMerge::JSONMerge(BlackRoot::IO::IFileSource *fm, BlackRoot::Format::JSON * json)
: JSON(json), FileManager(fm)
{
}

void JSONMerge::MergeRecursively()
{
    // ...
}