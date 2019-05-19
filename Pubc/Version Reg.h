/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <vector>

#include "BlackRoot/Pubc/Stringstream.h"
#include "BlackRoot/Pubc/Number Types.h"

namespace BlackRoot {
namespace Repo {

	struct VersionInformation {
		std::string Name;
		std::string Licence;
		std::string Version;
		std::string BranchName;
		std::string BranchTime;
		std::string BuildTool;
        uint128 VersionHash;

        void create_hash();
	};

    struct Contributor {
        std::string Name;

        Contributor(const char *s)
        : Name(s) { ; }
        Contributor(std::string s)
        : Name(s) { ; }
    };

    struct Library {
        std::string Name, Creator, Url;
        
        Library(const char *name, const char *creator, const char *url)
        : Name(name), Creator(creator), Url(url) { ; }
        Library(std::string name, std::string creator, std::string url)
        : Name(name), Creator(creator), Url(url) { ; }
    };

    struct ProjectContributors {
        std::string                 Project;
        std::vector<Contributor>    Contibutors;

        void add(Contributor);
        void sort();
    };

    struct ProjectLibraries {
        std::string                 Project;
        std::vector<Library>        Libraries;

        void add(Library);
        void sort();
    };

    using VersionInformationFunc = const VersionInformation &(__cdecl *)();
    using VersionInformationList = std::vector<VersionInformation>;

    class VersionRegistry {
    protected:
        static VersionRegistry * Registry;
        
        VersionInformationList              Per_Project_Versions;
        std::vector<ProjectContributors>    Per_Project_Contributors;
        std::vector<ProjectLibraries>       Per_Project_Libraries;

        VersionInformation                  App_Project_Version;

        VersionInformation                  Full_Project_Version;
        ProjectContributors                 Full_Project_Contributors;
        ProjectLibraries                    Full_Project_Libraries;
        
        ProjectContributors &  get_project_contributor_list(std::string);
        ProjectLibraries &     get_project_library_list(std::string);

        void update_full_project_hash();
    public:
        static VersionRegistry * get_registry();
        
        static void set_main_project_version(VersionInformation);

        static void add_version(VersionInformation);
        static void add_contributors(std::string, std::vector<Contributor>);
        static void add_libraries(std::string, std::vector<Library>);

        static VersionInformation     get_app_project_version();

        static ProjectContributors    get_full_project_contributors();
        static ProjectLibraries       get_full_project_libraries();

        static VersionInformationList get_version_list();
        
        static std::string            get_boot_string();
        
        static std::string            get_app_project_string();
        static std::string            get_version_string();
        static std::string            get_full_contribution_string();
    };

    namespace Helper {

        struct RegisterVersion {
            RegisterVersion(VersionInformationFunc x) {
                VersionRegistry::add_version(x());
            }
        };

        struct RegisterContributors {
            RegisterContributors(std::string p, std::vector<Contributor> c) {
                VersionRegistry::add_contributors(p, c);
            }
        };

        struct RegisterLibraries {
            RegisterLibraries(std::string p, std::vector<Library> l) {
                VersionRegistry::add_libraries(p, l);
            }
        };

        struct RegisterMainProject {
            RegisterMainProject(VersionInformationFunc x) {
                VersionRegistry::set_main_project_version(x());
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