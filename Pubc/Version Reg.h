/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <vector>

#include "BlackRoot/Pubc/Stringstream.h"

namespace BlackRoot {
namespace Repo {

	struct VersionInformation {
		const char * Name;
		const char * Licence;
		const char * Version;
		const char * BranchName;
		const char * BranchTime;
	};

    using VersionInformationFunc = const VersionInformation &(__cdecl *)();
    using VersionInformationList = std::vector<VersionInformation>;

    class VersionRegistry {
    protected:
        static VersionRegistry * Registry;
        
        VersionInformationList  Versions;

    public:
        static VersionRegistry * GetRegistry();

        static void AddVersion(VersionInformation);

        static VersionInformationList GetVersionList();
        static std::string            GetVersionString();
    };

    namespace Helper {

        struct RegisterVersion {
            RegisterVersion(VersionInformationFunc x) {
                VersionRegistry::AddVersion(x());
            }
        };

    };


#define BR_VERSION_HELPER(x, y) x ## y
#define BR_VERSION_HELPER2(x, y) BR_VERSION_HELPER(x, y)

#define BR_VERSION_DECLARE(x) \
	extern "C" { \
		__declspec(dllexport) const BlackRoot::Repo::VersionInformation & __cdecl BR_VERSION_HELPER2(x, _GetRepoVersion)(); \
	} \
    \
    inline const BlackRoot::Repo::VersionInformation & GetRepoVersion() { return BR_VERSION_HELPER2(x, _GetRepoVersion)(); } \
	std::string GetVersionString(); \
    \
    extern BlackRoot::Repo::Helper::RegisterVersion __RegisterVersion_##x; \
    \
    namespace Helper { \
    extern int make_sure_compilation_unit_referenced_##x(); \
    static int never_actually_used = make_sure_compilation_unit_referenced_##x(); \
    }

#define BR_VERSION_DEFINE(x) \
    static const BlackRoot::Repo::VersionInformation RepoVersion = REPO_VERSION_DATA; \
    \
	extern "C" { \
		__declspec(dllexport) const BlackRoot::Repo::VersionInformation & __cdecl BR_VERSION_HELPER2(x, _GetRepoVersion)() \
		{ \
			return RepoVersion; \
		} \
	} \
	\
	std::string GetVersionString() \
	{ \
        std::stringstream ss; \
		ss << RepoVersion.Name << " " << RepoVersion.Version << std::endl; \
		ss << " Licence: " << RepoVersion.Licence << std::endl; \
		ss << " Branch:  " << RepoVersion.BranchName << std::endl; \
		ss << " Time:    " << RepoVersion.BranchTime; \
		return ss.str();\
	} \
    \
    BlackRoot::Repo::Helper::RegisterVersion __RegisterVersion_##x(&(BR_VERSION_HELPER2(x, _GetRepoVersion)));\
    \
    int Helper::make_sure_compilation_unit_referenced_##x() \
    { \
        return 0; \
    }

}
}