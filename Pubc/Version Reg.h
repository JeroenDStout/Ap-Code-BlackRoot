/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <vector>

#include "BlackRoot/Pubc/Stringstream.h"

namespace BlackRoot {
namespace Repo {

	struct VersionInformation {
		std::string Name;
		std::string Licence;
		std::string Version;
		std::string BranchName;
		std::string BranchTime;
		std::string BuildTool;
	};

    struct Contributor {
        std::string Name;

        Contributor(const char *s)
        : Name(s) { ; }
        Contributor(std::string s)
        : Name(s) { ; }
    };

    struct Library {
        std::string Name;
        
        Library(const char *s)
        : Name(s) { ; }
        Library(std::string s)
        : Name(s) { ; }
    };

    struct ProjectContributors {
        std::string                 Project;
        std::vector<Contributor>    Contibutors;

        void Add(Contributor);
        void Sort();
    };

    struct ProjectLibraries {
        std::string                 Project;
        std::vector<Library>        Libraries;

        void Add(Library);
        void Sort();
    };

    using VersionInformationFunc = const VersionInformation &(__cdecl *)();
    using VersionInformationList = std::vector<VersionInformation>;

    class VersionRegistry {
    protected:
        static VersionRegistry * Registry;
        
        VersionInformationList              PerProjectVersions;
        VersionInformation                  FullProjectVersion;
        std::vector<ProjectContributors>    PerProjectContributors;
        ProjectContributors                 FullProjectContributors;
        std::vector<ProjectLibraries>       PerProjectLibraries;
        ProjectLibraries                    FullProjectLibraries;

    public:
        static VersionRegistry * GetRegistry();
        
        static void SetMainProjectVersion(VersionInformation);

        static void AddVersion(VersionInformation);
        static void AddContributors(std::string, std::vector<Contributor>);
        static void AddLibraries(std::string, std::vector<Library>);

        static VersionInformationList GetVersionList();
        
        static std::string            GetBootString();
        
        static std::string            GetMainProjectString();
        static std::string            GetVersionString();
        static std::string            GetFullContributionString();
        
        ProjectContributors &  GetProjectContributorList(std::string);
        ProjectLibraries &     GetProjectLibraryList(std::string);
    };

    namespace Helper {

        struct RegisterVersion {
            RegisterVersion(VersionInformationFunc x) {
                VersionRegistry::AddVersion(x());
            }
        };

        struct RegisterContributors {
            RegisterContributors(std::string p, std::vector<Contributor> c) {
                VersionRegistry::AddContributors(p, c);
            }
        };

        struct RegisterLibraries {
            RegisterLibraries(std::string p, std::vector<Library> l) {
                VersionRegistry::AddLibraries(p, l);
            }
        };

        struct RegisterMainProject {
            RegisterMainProject(VersionInformationFunc x) {
                VersionRegistry::SetMainProjectVersion(x());
            }
        };

    };
    
#define STRING2(x) #x
#define STRING(x) STRING2(x)

#ifdef _MSC_VER
    #ifdef _WIN64
    #define BR_WINV "x64"
    #elif defined _WIN32
    #define BR_WINV "x86"
    #else
    #error No code for build
    #endif

    #ifdef _DEBUG
    #define BR_RUNV "debug"
    #else
    #define BR_RUNV "release"
    #endif

    #define BR_BUILD_TOOL "MSC v." STRING(_MSC_VER) " " BR_WINV " " BR_RUNV
#else
#error No code for build
#endif

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
		ss << " Time:    " << RepoVersion.BranchTime << std::endl; \
		ss << " Build:   " << RepoVersion.BuildTool; \
		return ss.str();\
	} \
    \
    BlackRoot::Repo::Helper::RegisterVersion __RegisterVersion_##x(&(BR_VERSION_HELPER2(x, _GetRepoVersion)));\
    \
    int Helper::make_sure_compilation_unit_referenced_##x() \
    { \
        return 0; \
    }

#define BR_CONTRIBUTE_DEFINE(x) \
    BlackRoot::Repo::Helper::RegisterContributors __RegisterContributors_##x(__PROJECTSTR__, GEN_CONTRIBUTIONS);\
    BlackRoot::Repo::Helper::RegisterLibraries    __RegisterLibraries_##x(__PROJECTSTR__, GEN_LIBRARIES);

#define BR_PROJECT_DEFINE(x) \
    BlackRoot::Repo::Helper::RegisterMainProject  __RegisterMainProject_##x(&(BR_VERSION_HELPER2(x, _GetRepoVersion)));\
    

}
}