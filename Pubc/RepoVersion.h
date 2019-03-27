#pragma once

#include <string>
#include <sstream>

namespace BlackRoot {
	namespace Repo {

		struct VersionInformation {
			const char * Name;
			const char * Version;
			const char * BranchName;
			const char * BranchTime;
		};

#define BR_REPO_HELPER(x, y) x ## y
#define BR_REPO_HELPER2(x, y) BR_REPO_HELPER(x, y)

#define BR_REPO_VERSION_DECLARE(x) \
	extern "C" { \
		__declspec(dllexport) const BlackRoot::Repo::VersionInformation & __cdecl BR_REPO_HELPER2(x, _GetRepoVersion)(); \
	} \
    \
    inline const BlackRoot::Repo::VersionInformation & GetRepoVersion() { return BR_REPO_HELPER2(x, _GetRepoVersion)(); } \
	std::string GetVersionString();

#define BR_REPO_VERSION_DEFINE(x) \
    static const BlackRoot::Repo::VersionInformation RepoVersion = REPO_VERSION_DATA; \
    \
	extern "C" { \
		__declspec(dllexport) const BlackRoot::Repo::VersionInformation & __cdecl BR_REPO_HELPER2(x, _GetRepoVersion)() \
		{ \
			return RepoVersion; \
		} \
	} \
	\
	std::string GetVersionString() \
	{ \
        std::stringstream ss; \
		ss << RepoVersion.Name << " " << RepoVersion.Version << std::endl; \
		ss << " Branch: " << RepoVersion.BranchName << std::endl; \
		ss << " Time:   " << RepoVersion.BranchTime; \
		return ss.str();\
	}

}
}