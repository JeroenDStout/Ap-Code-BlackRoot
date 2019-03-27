#pragma once

#include "BlackRoot/Pubc/JSON.h"
#include "BlackRoot/Pubc/Files.h"

namespace BlackRoot {
namespace Util {

    struct JSONMerge {
        BlackRoot::IO::IFileManager *FileManager;

        BlackRoot::Format::JSON     JSON;
        
        JSONMerge(BlackRoot::IO::IFileManager *, BlackRoot::Format::JSON &&);
        JSONMerge(BlackRoot::IO::IFileManager *, const BlackRoot::Format::JSON &);

        void MergeRecursively();
    };

}
}