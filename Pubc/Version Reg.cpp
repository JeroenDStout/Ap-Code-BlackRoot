/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <algorithm>

#include "BlackRoot/Pubc/Stringstream.h"
#include "BlackRoot/Pubc/Version Reg.h"
#include "BlackRoot/Pubc/Hash ID.h"
#include "BlackRoot/Pubc/Hash Name.h"

using namespace BlackRoot::Repo;

VersionRegistry * VersionRegistry::Registry = nullptr;

VersionRegistry * VersionRegistry::get_registry()
{
    return Registry ? Registry : (Registry = new VersionRegistry);
}

void VersionRegistry::set_app_project_version(VersionInformation version)
{
    VersionRegistry::get_registry()->App_Project_Version = version;
}

void VersionRegistry::add_version(VersionInformation info)
{
    auto & reg = *VersionRegistry::get_registry();

    info.create_hash();
    reg.Per_Project_Versions.push_back(info);
    
    std::sort(reg.Per_Project_Versions.begin(), reg.Per_Project_Versions.end(), [](VersionInformation a, VersionInformation b) {
        return b.Name > a.Name;
    });

    reg.update_full_project_hash();
}

void VersionRegistry::update_full_project_hash()
{
    std::vector<uint128> hash;
    hash.reserve(this->Per_Project_Versions.size());

    for (auto & it : this->Per_Project_Versions)
      hash.push_back(it.VersionHash);

    this->Full_Project_Hash = BlackRoot::Identify::fnv1a_128((char*)(hash.data()), sizeof(uint128) * hash.size());
}

void VersionRegistry::add_contributors(std::string project, std::vector<Contributor> contributors)
{
    auto & reg  = *VersionRegistry::get_registry();
    auto & proj = reg.get_project_contributor_list(project);

    for (auto & e : contributors) {
        proj.add(e);
        reg.Full_Project_Contributors.add(e);
    }
    proj.sort();
    reg.Full_Project_Contributors.sort();
}

void VersionRegistry::add_libraries(std::string project, std::vector<Library> libraries)
{
    auto & reg  = *VersionRegistry::get_registry();
    auto & proj = reg.get_project_library_list(project);

    for (auto & e : libraries) {
        proj.add(e);
        reg.Full_Project_Libraries.add(e);
    }
    proj.sort();
    reg.Full_Project_Libraries.sort();
}

ProjectContributors & VersionRegistry::get_project_contributor_list(std::string str)
{
    auto & reg  = *VersionRegistry::get_registry();

    auto elem = std::find_if(reg.Per_Project_Contributors.begin(), reg.Per_Project_Contributors.end(),
      [&str](const ProjectContributors& x) {
        return x.Project == str;
    });

    if (elem != std::end(reg.Per_Project_Contributors))
        return *elem;

    ProjectContributors con;
    con.Project = str;
    reg.Per_Project_Contributors.push_back(con);
    return reg.Per_Project_Contributors.back();
}

ProjectLibraries & VersionRegistry::get_project_library_list(std::string str)
{
    auto & reg  = *VersionRegistry::get_registry();

    auto elem = std::find_if(reg.Per_Project_Libraries.begin(), reg.Per_Project_Libraries.end(),
      [&str](const ProjectLibraries& x) {
        return x.Project == str;
    });

    if (elem != std::end(reg.Per_Project_Libraries))
        return *elem;

    ProjectLibraries con;
    con.Project = str;
    reg.Per_Project_Libraries.push_back(con);
    return reg.Per_Project_Libraries.back();
}

VersionInformationList VersionRegistry::get_version_list()
{
    return VersionRegistry::get_registry()->Per_Project_Versions;
}

VersionInformation VersionRegistry::get_app_project_version()
{
    return VersionRegistry::get_registry()->App_Project_Version;
}

uint128 VersionRegistry::get_full_project_hash()
{
    return VersionRegistry::get_registry()->Full_Project_Hash;
}

ProjectContributors VersionRegistry::get_full_project_contributors()
{
    return VersionRegistry::get_registry()->Full_Project_Contributors;
}

ProjectLibraries VersionRegistry::get_full_project_libraries()
{
    return VersionRegistry::get_registry()->Full_Project_Libraries;
}

std::string VersionRegistry::get_app_project_string()
{
    auto & reg  = *VersionRegistry::get_registry();

    if (reg.App_Project_Version.Name.length() == 0) {
        return "";
    }
    
    std::stringstream ss;

    ss << "***" << std::endl;
    ss << "*** " << reg.App_Project_Version.Name << std::endl;
    ss << "***  " << reg.App_Project_Version.Version << std::endl;
    ss << "***  " << Identify::generate_hash_name_32(uint32(reg.get_full_project_hash())).get() << std::endl;
    ss << "***  " << reg.App_Project_Version.BuildTool << std::endl;
    ss << "***";

    return ss.str();
}

std::string VersionRegistry::get_version_string()
{
    std::stringstream ss;

    bool first = true;

    for (auto & version : VersionRegistry::get_registry()->Per_Project_Versions) {
        if (!first) {
            ss << std::endl;
        }

        first = false;
	    ss << version.Name << " " << version.Version << " (" << version.BranchName << ")" << std::endl;
	    ss << " " << Identify::generate_hash_name_32(uint32(version.VersionHash)).get() << std::endl;
	    ss << " " << version.Licence << std::endl;
	    ss << " " << version.BuildTool;
    }

	return ss.str();
}

std::string VersionRegistry::get_full_contribution_string()
{
    std::stringstream ss;

    bool first = true;

    ss << "This software contains contributions by\n ";
    for (auto & elem : VersionRegistry::get_registry()->Full_Project_Contributors.Contibutors) {
        if (!first) {
            ss << ", ";
        }
        first = false;
	    ss << elem.Name;
    }

    ss << "\nAnd is built using \n ";

    first = true;

    for (auto & elem : VersionRegistry::get_registry()->Full_Project_Libraries.Libraries) {
        if (!first) {
            ss << ", ";
        }
        first = false;
	    ss << elem.Name;
        if (elem.Creator.size() > 0) {
	        ss << " (" << elem.Creator << ")";
        }
    }

	return ss.str();
}

std::string VersionRegistry::get_boot_string()
{
    std::stringstream ss;
    
    ss << VersionRegistry::get_app_project_string() << std::endl << std::endl;
    ss << VersionRegistry::get_full_contribution_string() << std::endl << std::endl;
    ss << VersionRegistry::get_version_string();

    return ss.str();
}

void VersionInformation::create_hash()
{
    std::string combined;
    combined.append(this->Name);
    combined.append(this->Licence);
    combined.append(this->Version);
    combined.append(this->BranchName);
    combined.append(this->BranchTime);
    combined.append(this->BuildTool);
    this->VersionHash = BlackRoot::Identify::fnv1a_128(combined.c_str(), combined.size());
}

void ProjectContributors::add(Contributor con)
{
    for (auto & prev : this->Contibutors) {
        if (prev.Name != con.Name)
            continue;
        return;
    }
    this->Contibutors.push_back(con);
}

void ProjectContributors::sort()
{
    std::sort(this->Contibutors.begin(), this->Contibutors.end(), [](Contributor a, Contributor b) {
        return b.Name > a.Name;
    });
}

void ProjectLibraries::add(Library con)
{
    for (auto & prev : this->Libraries) {
        if (prev.Name != con.Name && prev.Creator != con.Creator)
            continue;
        return;
    }
    this->Libraries.push_back(con);
}

void ProjectLibraries::sort()
{
    std::sort(this->Libraries.begin(), this->Libraries.end(), [](Library a, Library b) {
        auto cmp = b.Name.compare(a.Name);
        if (cmp == 0) {
            return b.Creator > a.Creator;
        }
        return cmp > 0;
    });
}