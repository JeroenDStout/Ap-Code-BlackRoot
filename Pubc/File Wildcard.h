/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

/* {quality} This is very much a sketch
 */

#pragma once

#include <map>

#include "BlackRoot/Pubc/Files.h"

namespace BlackRoot {
namespace Util {

    struct SmartFileWildcard {
    public:
        using FilePath = BlackRoot::IO::FilePath;
        using FileSrc  = BlackRoot::IO::IFileSource;
        using KeyValue = std::map<std::string, std::string>;

        struct Path {
            FilePath  FoundPath;
            KeyValue  Replacements;

            Path() { ; }
            bool operator== (const Path&) const;
        };

        using FoundList    = std::vector<Path>;
        
    protected:
        std::string         Wildcard;
        std::string         DelimOpen, DelimClose;
        FilePath            CheckPath;
        std::vector<Path>   FoundPaths;

    public:
        SmartFileWildcard() { ; }
        
        void SetWildcard(std::string);
        void SetDelimiters(std::string, std::string);

        void SetCheckPath(FilePath);

        bool Check(FileSrc *);

        void RemoveFound();

        const FoundList & GetFound()        { return this->FoundPaths; }
        const FilePath &  GetCheckPath()    { return this->CheckPath; }
    };

}
}