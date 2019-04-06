/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <algorithm>

#include "BlackRoot/Pubc/Stringstream.h"
#include "BlackRoot/Pubc/Version Reg.h"

using namespace BlackRoot::Repo;

VersionRegistry * VersionRegistry::Registry = nullptr;

VersionRegistry * VersionRegistry::GetRegistry()
{
    return Registry ? Registry : (Registry = new VersionRegistry);
}

void VersionRegistry::SetMainProjectVersion(VersionInformation version)
{
    VersionRegistry::GetRegistry()->FullProjectVersion = version;
}

void VersionRegistry::AddVersion(VersionInformation info)
{
    auto & reg  = *VersionRegistry::GetRegistry();

    reg.PerProjectVersions.push_back(info);
    
    std::sort(reg.PerProjectVersions.begin(), reg.PerProjectVersions.end(), [](VersionInformation a, VersionInformation b) {
        return b.Name > a.Name;
    });
}

void VersionRegistry::AddContributors(std::string project, std::vector<Contributor> contributors)
{
    auto & reg  = *VersionRegistry::GetRegistry();
    auto & proj = reg.GetProjectContributorList(project);

    for (auto & e : contributors) {
        proj.Add(e);
        reg.FullProjectContributors.Add(e);
    }
    proj.Sort();
    reg.FullProjectContributors.Sort();
}

void VersionRegistry::AddLibraries(std::string project, std::vector<Library> libraries)
{
    auto & reg  = *VersionRegistry::GetRegistry();
    auto & proj = reg.GetProjectLibraryList(project);

    for (auto & e : libraries) {
        proj.Add(e);
        reg.FullProjectLibraries.Add(e);
    }
    proj.Sort();
    reg.FullProjectContributors.Sort();
}

ProjectContributors & VersionRegistry::GetProjectContributorList(std::string str)
{
    auto & reg  = *VersionRegistry::GetRegistry();

    auto elem = std::find_if(reg.PerProjectContributors.begin(), reg.PerProjectContributors.end(),
      [&str](const ProjectContributors& x) {
        return x.Project == str;
    });

    if (elem != std::end(reg.PerProjectContributors))
        return *elem;

    ProjectContributors con;
    con.Project = str;
    reg.PerProjectContributors.push_back(con);
    return reg.PerProjectContributors.back();
}

ProjectLibraries & VersionRegistry::GetProjectLibraryList(std::string str)
{
    auto & reg  = *VersionRegistry::GetRegistry();

    auto elem = std::find_if(reg.PerProjectLibraries.begin(), reg.PerProjectLibraries.end(),
      [&str](const ProjectLibraries& x) {
        return x.Project == str;
    });

    if (elem != std::end(reg.PerProjectLibraries))
        return *elem;

    ProjectLibraries con;
    con.Project = str;
    reg.PerProjectLibraries.push_back(con);
    return reg.PerProjectLibraries.back();
}

VersionInformationList VersionRegistry::GetVersionList()
{
    return VersionRegistry::GetRegistry()->PerProjectVersions;
}

std::string VersionRegistry::GetMainProjectString()
{
    auto & reg  = *VersionRegistry::GetRegistry();

    if (reg.FullProjectVersion.Name.length() == 0) {
        return "";
    }
    
    std::stringstream ss;

    ss << "***" << std::endl;
    ss << "*** " << reg.FullProjectVersion.Name << std::endl;
    ss << "***  " << reg.FullProjectVersion.Version << std::endl;
    ss << "***";

    return ss.str();
}

std::string VersionRegistry::GetVersionString()
{
    std::stringstream ss;

    bool first = true;

    for (auto & version : VersionRegistry::GetRegistry()->PerProjectVersions) {
        if (!first) {
            ss << std::endl;
        }
        first = false;
	    ss << version.Name << " " << version.Version << " (" << version.BranchName << ")" << std::endl;
	    ss << " " << version.Licence;
    }

	return ss.str();
}

std::string VersionRegistry::GetFullContributionString()
{
    std::stringstream ss;

    bool first = true;

    ss << "This software contains contributions by\n ";
    for (auto & elem : VersionRegistry::GetRegistry()->FullProjectContributors.Contibutors) {
        if (!first) {
            ss << ", ";
        }
        first = false;
	    ss << elem.Name;
    }

    ss << "\nAnd is built using \n ";

    first = true;

    for (auto & elem : VersionRegistry::GetRegistry()->FullProjectLibraries.Libraries) {
        if (!first) {
            ss << ", ";
        }
        first = false;
	    ss << elem.Name;
    }

	return ss.str();
}

std::string VersionRegistry::GetBootString()
{
    std::stringstream ss;
    
    ss << VersionRegistry::GetMainProjectString() << std::endl << std::endl;
    ss << VersionRegistry::GetFullContributionString() << std::endl << std::endl;
    ss << VersionRegistry::GetVersionString();

    return ss.str();
}

void ProjectContributors::Add(Contributor con)
{
    for (auto & prev : this->Contibutors) {
        if (prev.Name != con.Name)
            continue;
        return;
    }
    this->Contibutors.push_back(con);
}

void ProjectContributors::Sort()
{
    std::sort(this->Contibutors.begin(), this->Contibutors.end(), [](Contributor a, Contributor b) {
        return b.Name > a.Name;
    });
}

void ProjectLibraries::Add(Library con)
{
    for (auto & prev : this->Libraries) {
        if (prev.Name != con.Name)
            continue;
        return;
    }
    this->Libraries.push_back(con);
}

void ProjectLibraries::Sort()
{
    std::sort(this->Libraries.begin(), this->Libraries.end(), [](Library a, Library b) {
        return b.Name > a.Name;
    });
}