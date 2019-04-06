/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "BlackRoot/Pubc/Stringstream.h"

#include "BlackRoot/Pubc/Version Reg.h"
#include "BlackRoot/Pubc/Version.h"

#include "BlackRoot/.genc/def_repo_version.h"
#include "BlackRoot/.genc/def_contribute.h"

namespace BlackRoot {
namespace Core {

	BR_VERSION_DEFINE(BlackRoot);
    BR_CONTRIBUTE_DEFINE(BlackRoot);

}
}