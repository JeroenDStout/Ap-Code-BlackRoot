/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include "BlackRoot/Pubc/File Wildcard.h"

using namespace BlackRoot::Util;
namespace fs = std::experimental::filesystem;

    //  Check
    // --------------------

bool SmartFileWildcard::Check(FileSrc *fs)
{
    std::vector<Path> potential;

    std::string check = this->CheckPath.relative_path().string();

    Path initial{};
    initial.FoundPath = this->CheckPath.root_path();
    potential.push_back(initial);
            
        // We want to track the check results differ from the last
    bool anyChanges = false;

        // If two wildcards appear inside a single path piece, we
        // gracefully will check for this outrageous behaviour
    bool midwayCheck = false;

        // We keep looping as long as we have potentially correct paths
    while (potential.size() > 0) {
        auto wild = check.find(this->Wildcard);
        
            // We still have a wildcard to process
        if (wild != check.npos) {
                
                // Check if there is a directory underneath our wildcard; we can
                // first ensure the potential paths can match up until here
            auto prevDir = check.rfind(BlackRoot::System::DirSeperator, wild);
            if (prevDir != check.npos) {
                auto sub = check.substr(0, prevDir+1);

                    // Check if our paths lead to a rela direcotry
                auto & pot = potential.begin();
                while (pot != potential.end()) {
                    pot->FoundPath /= sub;

                    if (!fs->DirectoryExists(pot->FoundPath)) {
                        pot = potential.erase(pot);
                        continue;
                    }
                    ++pot;
                }

                    // Remove the directory from our check path
                check.erase(0, prevDir+1);
                continue;
            }
            
            size_t wildcardEnd = wild + this->Wildcard.length();

                // Check if this wildcard has a name; the opening delim should
                // appear directly after the wildcard
            std::string name;
            bool named = (0 == check.compare(wild + this->Wildcard.length(), this->DelimOpen.length(), this->DelimOpen));
            if (named) {
                size_t nameStart = wild + this->Wildcard.length() + this->DelimOpen.length();

                    // Find the closing character
                auto close = check.find(this->DelimClose.c_str(), nameStart, this->DelimClose.length());
                if (close == check.npos) {
                    throw new BlackRoot::Debug::Exception("Malformed wildcard: delimiter not closed", BRGenDbgInfo);
                }

                    // Grab the name and change the wildcard end so it is removed entirely
                name = check.substr(nameStart, close - nameStart);
                wildcardEnd = close + this->DelimClose.length();
            }
            
                // Remember the pre-wildcard check and erase it and the wildcard from the ceck
            std::string subCheckStart = check.substr(0, wild);
            check.erase(0, wildcardEnd);

                // Remember if we are still in a directory or not
            auto nextDir = check.find(BlackRoot::System::DirSeperator);
            
                // The remainder of our check has to be until either the end of the entire string,
                // the next directory, or the next wildcard
            auto endSearch = check.length();
            endSearch = std::min(endSearch, nextDir);
            endSearch = std::min(endSearch, check.find(this->Wildcard));
            
                // Grab the post-wildcard check and erase it; we are now looking for a match to
                // [subCheckStart] * [subCheckStart]
            std::string subCheckEnd = check.substr(0, endSearch);
            check.erase(0, endSearch);

                // Check all our existing paths
            int potCheckCount = potential.size();
            for (int potI = 0; potI < potCheckCount; potI++) {
                auto & potPath = potential[potI];
                std::string preCheck = "";

                    // If we're doing a midway check, we check the parent path
                    // TODO: do this more optimally
                BlackRoot::IO::DirectoryContents cont;
                if (midwayCheck) {
                    cont = fs->GetDirectoryContents(potPath.FoundPath.parent_path());
                    preCheck = potPath.FoundPath.filename().string();
                }
                else {
                    cont = fs->GetDirectoryContents(potPath.FoundPath);
                }

                    // We look for [precheck][subcheck] * [endcheck]
                preCheck += subCheckStart;

                    // For now we just always add new paths; this is not optimal
                    // but works for now
                Path newPath = potPath;

                for (auto & file : cont) {
                        // Get string with only direct path
                    auto strFile = file.Path.string();
                    auto strFileSep = strFile.rfind(BlackRoot::System::DirSeperator);
                    strFile.erase(0, strFileSep + 1);

                        // Check for start
                    if (preCheck.length() != 0 && 0 != strFile.compare(0, preCheck.length(), preCheck.c_str()))
                        continue;

                        // Loop through all the matches
                    size_t fileFindEnd = 0;
                    do {
                            // If our check ends in a string, check for it; otherwise
                            // we replace the entirety of what-ever we get
                        if (subCheckEnd.length() > 0) {
                            fileFindEnd = strFile.find(subCheckEnd, fileFindEnd);
                            if (fileFindEnd == strFile.npos)
                                break;
                        }
                        else {
                            fileFindEnd = strFile.length();
                        }

                        auto matchString = strFile.substr(wild, fileFindEnd);

                            // Modify this path, and if needed add the replaced value
                            // to the replacement key-value set
                        newPath.FoundPath += subCheckStart;
                        newPath.FoundPath += matchString;
                        newPath.FoundPath += subCheckEnd;
                        if (named) {
                            newPath.Replacements[name] = matchString;
                        }
                        potential.push_back(std::move(newPath));
                        newPath = potential[potI];

                            // We matched the entire string; no need to be clever
                        if (subCheckEnd.length() == 0)
                            break;
                        fileFindEnd += 1;
                    } while(true);
                }
            }

                // Sub-optimal; remove the potentials we used to spawn new potentials
            potential.erase(potential.begin(), potential.begin() + potCheckCount);

                // If our search ended before the directory did, we were looking at
                // a sub-section of a folder, so next time we do a midway check
            if (nextDir != check.npos && nextDir > endSearch) {
                midwayCheck = true;
            }

                // We found a wildcard; restart the whole process
            continue;
        }
        
            // Check if our paths lead to something with a real file
        auto & pot = potential.begin();
        while (pot != potential.end()) {
            pot->FoundPath /= check;

            if (!fs->FileExists(pot->FoundPath)) {
                pot = potential.erase(pot);
                continue;
            }

            ++pot;
        }

            // Check whether previously found paths are all still there
        auto & prev = this->FoundPaths.begin();
        while (prev != this->FoundPaths.end()) {
            auto & it = std::find(potential.begin(), potential.end(), *prev);
            if (it != potential.end()) {
                ++prev;
                continue;
            }
            prev = this->FoundPaths.erase(prev);
            anyChanges = true;
        }

            // Check whether new paths are already there
        for (auto & nw : potential) {
            auto &it = std::find(this->FoundPaths.begin(), this->FoundPaths.end(), nw);
            if (it != this->FoundPaths.end())
                continue;
            this->FoundPaths.push_back(std::move(nw));
            anyChanges = true;
        }

        return anyChanges;
    }

        // If no paths are possible we of course make sure the found
        // paths has a zero length; and we check if that already was the case
    if (this->FoundPaths.size() == 0)
        return false;

    this->FoundPaths.resize(0);
    return true;
}

    //  Settings
    // --------------------

void SmartFileWildcard::SetWildcard(std::string wildcard)
{
    this->Wildcard    = wildcard;
}

void SmartFileWildcard::SetDelimiters(std::string open, std::string close)
{
    this->DelimOpen   = open;
    this->DelimClose  = close;
}

void SmartFileWildcard::SetCheckPath(FilePath path)
{
    this->CheckPath   = fs::canonical(path);
}

    //  Control
    // --------------------

void SmartFileWildcard::RemoveFound()
{
    this->FoundPaths.resize(0);
}

    //  Util
    // --------------------

bool SmartFileWildcard::Path::operator== (const Path &rh) const
{
    if (this->FoundPath != rh.FoundPath)
        return false;
    if (this->Replacements != rh.Replacements)
        return false;
    return true;
}