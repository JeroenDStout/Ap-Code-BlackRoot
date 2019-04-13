/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <string>
#include <vector>

#include "BlackRoot/Pubc/Exception.h"
#include "BlackRoot/Pubc/Files.h"

namespace BlackRoot {
namespace Util {

    template<typename BaseClass>
    class FileSourceSnooper : public BaseClass {
    public:
            // Inherit all types from our baseclass, which we assume
            // is based on something like BlackRoot:IO::ISourceFile
        using Time = std::chrono::system_clock::time_point;
        using FilePath  = typename BaseClass::FilePath;
        using FileTime  = typename BaseClass::FTime;
        using Stream    = typename BaseClass::Stream;
        using FSize     = typename BaseClass::FSize;
        using FCont     = typename BaseClass::FCont;
        using FStr      = typename BaseClass::FStr;
        using OpenInstr = typename BaseClass::OpenInstr;

            // A snooped item records path, write time, open time, access
        struct SnoopedItem {
            FilePath    Path;
            FileTime    PreviousLastWriteTime;
            Time        OpenTime;
            bool        WriteAccess;
        };
        using SnoopList = std::vector<SnoopedItem>;

    protected:
        SnoopList     SnoopedList;

    public:
            // Overloaded base class functions for file use
        void     Copy(const FilePath from, const FilePath to) override;
        void     CopyFile(const FilePath from, const FilePath to) override;

        Stream * OpenFile(const FilePath, const OpenInstr, BlackRoot::Debug::Info = BRGenDbgInfo) override;

        FSize    ReadFile(const FilePath, void **out, const OpenInstr) override;
        FCont    ReadFile(const FilePath, const OpenInstr) override;
        FStr     ReadFileAsString(const FilePath, const OpenInstr) override;
        
        virtual void AddSnoopedItem(FilePath, bool writeAccess);
        virtual void ClearList();
        virtual const SnoopList & GetList();
    };
    
    template<typename BaseClass>
    void FileSourceSnooper<BaseClass>::Copy(const FilePath from, const FilePath to)
    {
        this->AddSnoopedItem(from, false);
        this->AddSnoopedItem(to, true);
        this->BaseClass::Copy(from, to);
    }
    
    template<typename BaseClass>
    void FileSourceSnooper<BaseClass>::CopyFile(const FilePath from, const FilePath to)
    {
        this->AddSnoopedItem(from, false);
        this->AddSnoopedItem(to, true);
        this->BaseClass::CopyFile(from, to);
    }
    
    template<typename BaseClass>
    typename FileSourceSnooper<BaseClass>::Stream * FileSourceSnooper<BaseClass>::OpenFile(const FilePath path, const OpenInstr instr, BlackRoot::Debug::Info info)
    {
        this->AddSnoopedItem(path, 0 != (instr.MAccess & BlackRoot::IO::FileMode::Access::Write));
        return this->BaseClass::OpenFile(path, instr, info);
    }
    
    template<typename BaseClass>
    typename FileSourceSnooper<BaseClass>::FSize FileSourceSnooper<BaseClass>::ReadFile(const FilePath path, void **out, const OpenInstr instr)
    {
        this->AddSnoopedItem(path, false);
        return this->BaseClass::ReadFile(path, out, instr);
    }
    
    template<typename BaseClass>
    typename FileSourceSnooper<BaseClass>::FCont FileSourceSnooper<BaseClass>::ReadFile(const FilePath path, const OpenInstr instr)
    {
        this->AddSnoopedItem(path, false);
        return this->BaseClass::ReadFile(path, instr);
    }
    
    template<typename BaseClass>
    typename FileSourceSnooper<BaseClass>::FStr FileSourceSnooper<BaseClass>::ReadFileAsString(const FilePath path, const OpenInstr instr)
    {
        this->AddSnoopedItem(path, false);
        return this->BaseClass::ReadFileAsString(path, instr);
    }
    
    template<typename BaseClass>
    void FileSourceSnooper<BaseClass>::AddSnoopedItem(FilePath path, bool writeAccess)
    {
        SnoopedItem item;
        item.Path = path;
        if (this->Exists(path)) {
            item.PreviousLastWriteTime = this->LastWriteTime(path);
        }
        else {
            item.PreviousLastWriteTime = std::chrono::system_clock::from_time_t({});
        }
        item.OpenTime = std::chrono::system_clock::now();
        item.WriteAccess = writeAccess;

        this->SnoopedList.push_back(item);
    }
    
    template<typename BaseClass>
    void FileSourceSnooper<BaseClass>::ClearList()
    {
        this->SnoopedList.resize(0);
    }
    
    template<typename BaseClass>
    typename const FileSourceSnooper<BaseClass>::SnoopList & FileSourceSnooper<BaseClass>::GetList()
    {
        return this->SnoopedList;
    }

}
}