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
    class FileSourcePrefix : public BaseClass {
    public:
            // Inherit all types from our baseclass, which we assume
            // is based on something like BlackRoot:IO::ISourceFile
        using FilePath  = typename BaseClass::FilePath;
        using FTime     = typename BaseClass::FTime;
        using Stream    = typename BaseClass::Stream;
        using FSize     = typename BaseClass::FSize;
        using FCont     = typename BaseClass::FCont;
        using FStr      = typename BaseClass::FStr;
        using OpenInstr = typename BaseClass::OpenInstr;
        using DirCon    = typename BaseClass::DirCon;

        FilePath    Prefix;

    public:
        void     Copy(const FilePath from, const FilePath to) override;
        void     CopyFile(const FilePath from, const FilePath to) override;

        bool     Exists(const FilePath) override;
        bool     FileExists(const FilePath) override;
        bool     DirectoryExists(const FilePath) override;
        FSize    FileSize(const FilePath) override;
        FTime    LastWriteTime(const FilePath) override;
        
        void     Remove(const FilePath) override;
        void     RemoveAll(const FilePath) override;

        void     Rename(const FilePath from, const FilePath to) override;
        
        void     CreateDirectories(const FilePath) override;

        DirCon   GetDirectoryContents(const FilePath) override;

        Stream * OpenFile(const FilePath, const BlackRoot::IO::FileMode::OpenInstr, BlackRoot::Debug::Info = BRGenDbgInfo) override;

        FSize    ReadFile(const FilePath, void **out, const BlackRoot::IO::FileMode::OpenInstr) override;
        FCont    ReadFile(const FilePath, const BlackRoot::IO::FileMode::OpenInstr) override;
        FStr     ReadFileAsString(const FilePath, const BlackRoot::IO::FileMode::OpenInstr) override;
    };
    
    template<typename BaseClass>
    void FileSourcePrefix<BaseClass>::Copy(const FilePath from, const FilePath to) {
        return this->BaseClass::Copy(this->Prefix / from, this->Prefix / to);
    }
    
    template<typename BaseClass>
    void FileSourcePrefix<BaseClass>::CopyFile(const FilePath from, const FilePath to) {
        return this->BaseClass::CopyFile(this->Prefix / from, this->Prefix / to);
    }
    
    template<typename BaseClass>
    bool FileSourcePrefix<BaseClass>::Exists(const FilePath path) {
        return this->BaseClass::Exists(this->Prefix / path);
    }
    
    template<typename BaseClass>
    bool FileSourcePrefix<BaseClass>::FileExists(const FilePath path) {
        return this->BaseClass::FileExists(this->Prefix / path);
    }
    
    template<typename BaseClass>
    bool FileSourcePrefix<BaseClass>::DirectoryExists(const FilePath path) {
        return this->BaseClass::DirectoryExists(this->Prefix / path);
    }
    
    template<typename BaseClass>
    auto FileSourcePrefix<BaseClass>::FileSize(const FilePath path) -> FSize {
        return this->BaseClass::FileSize(this->Prefix / path);
    }
    
    template<typename BaseClass>
    auto FileSourcePrefix<BaseClass>::LastWriteTime(const FilePath path) -> FTime {
        return this->BaseClass::LastWriteTime(this->Prefix / path);
    }
    
    template<typename BaseClass>
    void FileSourcePrefix<BaseClass>::Remove(const FilePath path) {
        return this->BaseClass::Remove(this->Prefix / path);
    }
    
    template<typename BaseClass>
    void FileSourcePrefix<BaseClass>::RemoveAll(const FilePath path) {
        return this->BaseClass::RemoveAll(this->Prefix / path);
    }
    
    template<typename BaseClass>
    void FileSourcePrefix<BaseClass>::Rename(const FilePath from, const FilePath to) {
        return this->BaseClass::Rename(this->Prefix / from, this->Prefix / to);
    }
    
    template<typename BaseClass>
    void FileSourcePrefix<BaseClass>::CreateDirectories(const FilePath path) {
        return this->BaseClass::CreateDirectories(this->Prefix / path);
    }
    
    template<typename BaseClass>
    auto FileSourcePrefix<BaseClass>::GetDirectoryContents(const FilePath path) -> DirCon {
        return this->BaseClass::GetDirectoryContents(this->Prefix / path);
    }
    
    template<typename BaseClass>
    auto FileSourcePrefix<BaseClass>::OpenFile(const FilePath path, const BlackRoot::IO::FileMode::OpenInstr instr, BlackRoot::Debug::Info info) -> Stream* {
        return this->BaseClass::OpenFile(this->Prefix / path, instr, info);
    }
    
    template<typename BaseClass>
    auto FileSourcePrefix<BaseClass>::ReadFile(const FilePath path, void **out, const BlackRoot::IO::FileMode::OpenInstr instr) -> FSize {
        return this->BaseClass::ReadFile(this->Prefix / path, out, instr);
    }
    
    template<typename BaseClass>
    auto FileSourcePrefix<BaseClass>::ReadFile(const FilePath path, const BlackRoot::IO::FileMode::OpenInstr instr) -> FCont {
        return this->BaseClass::ReadFile(this->Prefix / path, instr);
    }
    
    template<typename BaseClass>
    auto FileSourcePrefix<BaseClass>::ReadFileAsString(const FilePath path, const BlackRoot::IO::FileMode::OpenInstr instr) -> FStr {
        return this->BaseClass::ReadFileAsString(this->Prefix / path, instr);
    }

}
}