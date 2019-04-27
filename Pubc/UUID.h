/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <uuid.h>

namespace BlackRoot {
namespace Identify {

    using UUID          = uuids::uuid;
    using UUIDGenerator = uuids::uuid_system_generator;

}
}