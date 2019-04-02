/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "BlackRoot/Pubc/Stringstream.h"
#include "BlackRoot/Pubc/Version Reg.h"

using namespace BlackRoot::Repo;

VersionRegistry * VersionRegistry::Registry = nullptr;

VersionRegistry * VersionRegistry::GetRegistry()
{
    return Registry ? Registry : (Registry = new VersionRegistry);
}

void VersionRegistry::AddVersion(VersionInformation info)
{
    VersionRegistry::GetRegistry()->Versions.push_back(info);
}

VersionInformationList VersionRegistry::GetVersionList()
{
    return VersionRegistry::GetRegistry()->Versions;
}

std::string VersionRegistry::GetVersionString()
{
    std::stringstream ss;

    bool first = true;

    for (auto & version : VersionRegistry::GetRegistry()->Versions) {
        if (!first) {
            ss << std::endl << std::endl;
        }
        first = false;
	    ss << version.Name << " " << version.Version << " (" << version.BranchName << ")" << std::endl;
	    ss << " " << version.Licence;
    }

	return ss.str();
}