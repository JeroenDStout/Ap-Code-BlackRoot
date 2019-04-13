/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <string>
#include <vector>

#if _WIN32
#undef  NOMINMAX
#define NOMINMAX
#include <windows.h>
#undef CopyFile
#else
#error No code for build
#endif

#ifdef _DEBUG
#define BR_FILES_PARANOIA
#endif

#include "BlackRoot/Pubc/Exception.h"
#include "BlackRoot/Pubc/Files Types.h"

namespace BlackRoot {
namespace IO {

    class IFileSource;
    class IFileStream;

    class BaseFileSource;
    class BaseFile;

    namespace FileMode {
        struct Access {
            using Type = uint8_t;
            enum {
                Read    = 0x1,
                Write   = 0x2
            };
        };

        struct Share {
            using Type = uint8_t;
            enum {
                None    = 0x0,
                Delete  = 0x1,
                Read    = 0x2,
                Write   = 0x4
            };
        };

        struct Creation {
            using Type = uint8_t;
            enum {
                CreateAlways,
                CreateNew,
                OpenAlways,
                OpenExisting,
                TruncateExisting
            };
        };

        struct Attributes {
            using Type = uint8_t;
            enum {
                None    = 0x0,
                Hidden  = 0x1
            };
        };

        struct OpenInstr {
            Access::Type      MAccess;
            Share::Type       MShare;
            Creation::Type    MCreation;
            Attributes::Type  MAttributes;

            OpenInstr() {}

            OpenInstr & Default() {
                this->Access(Access::Read | Access::Write)
                    .Share(Share::Read | Share::Write)
                    .Creation(Creation::OpenAlways)
                    .Attributes(Attributes::None);
                return *this;
            }
            OpenInstr & Access(Access::Type t) {
                this->MAccess = t;
                return *this;
            }
            OpenInstr & Share(Share::Type t) {
                this->MShare = t;
                return *this;
            }
            OpenInstr & Creation(Creation::Type t) {
                this->MCreation = t;
                return *this;
            }
            OpenInstr & Attributes(Attributes::Type t) {
                this->MAttributes = t;
                return *this;
            }
        };
    }

    struct DirectoryContents {
        std::vector<FilePath> Paths;

        std::vector<FilePath>::iterator begin() { return Paths.begin(); }
        std::vector<FilePath>::iterator end()   { return Paths.end(); }
    };

    class IFileSource {
    public:
        using FilePath  = FilePath;
        using FSize     = FileSize;
        using FTime     = FileTime;
        using OpenInstr = FileMode::OpenInstr;

        using Stream    = IFileStream;
        using DirCon    = DirectoryContents;
        using FCont     = std::vector<char>;
        using FStr      = std::string;

        virtual ~IFileSource() = 0 {};

        virtual bool     ManagerIsReadOnly() = 0;

        virtual void     Copy(const FilePath from, const FilePath to) = 0;
        virtual void     CopyFile(const FilePath from, const FilePath to) = 0;

        virtual bool     Exists(const FilePath) = 0;
        virtual bool     FileExists(const FilePath) = 0;
        virtual bool     DirectoryExists(const FilePath) = 0;
        virtual FSize    FileSize(const FilePath) = 0;
        virtual FTime    LastWriteTime(const FilePath) = 0;
        
        virtual void     Remove(const FilePath) = 0;
        virtual void     RemoveAll(const FilePath) = 0;

        virtual void     Rename(const FilePath from, const FilePath to) = 0;

        virtual void     CreateDirectories(const FilePath) = 0;

        virtual DirCon   GetDirectoryContents(const FilePath) = 0;

        virtual Stream * OpenFile(const FilePath, const OpenInstr, BlackRoot::Debug::Info = BRGenDbgInfo) = 0;

        virtual FSize    ReadFile(const FilePath, void **out, const OpenInstr) = 0;
        virtual FCont    ReadFile(const FilePath, const OpenInstr) = 0;
        virtual FStr     ReadFileAsString(const FilePath, const OpenInstr) = 0;
    };

    class IFileStream {
    protected:
        BlackRoot::Debug::Info      DbOriginInfo;

        std::string                 OriginPath;
        FileMode::Access::Type      AccessType;
        
        virtual ~IFileStream() = 0;

    public:
        IFileStream(const std::string originPath, FileMode::Access::Type, const BlackRoot::Debug::Info);

        virtual void CloseAndRelease() = 0;

        using FSize   = IFileSource::FSize;
        using FCont   = IFileSource::FCont;

        virtual bool  CanRead()  const { return 0 != (AccessType & FileMode::Access::Read); }
        virtual bool  CanWrite() const { return 0 != (AccessType & FileMode::Access::Write); }
        virtual bool  CanSeek() const = 0;

        
        virtual FSize Seek(FSize) = 0;
        virtual FSize SeekStart() = 0;
        virtual FSize SeekEnd() = 0;
        virtual FSize SeekRelative(FSize) = 0;

        virtual FSize GetRWPoint() = 0;

        virtual FileManipLength Read(void *buffer, FileManipLength byteCount) = 0;
        virtual FileManipLength Write(void *buffer, FileManipLength byteCount) = 0;

        virtual FCont ReadUntilEnd() = 0;
        virtual FSize ReadUntilEnd(void ** out) = 0;

        virtual BlackRoot::Debug::Exception * CreateException(const std::string);
    };

    class BaseFileSource : public IFileSource {
    public:
        FilePath    BasePath;
        
        BaseFileSource() { ; }
        BaseFileSource(FilePath base) : BasePath(base) { ; }
        ~BaseFileSource() override { ; }

        bool     ManagerIsReadOnly() override;

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

        Stream * OpenFile(const FilePath, const FileMode::OpenInstr, BlackRoot::Debug::Info = BRGenDbgInfo) override;

        FSize    ReadFile(const FilePath, void **out, const FileMode::OpenInstr) override;
        FCont    ReadFile(const FilePath, const FileMode::OpenInstr) override;
        FStr     ReadFileAsString(const FilePath, const FileMode::OpenInstr) override;
    };

    class BaseFileStream : public IFileStream {
    protected:
        ~BaseFileStream() override;

    public:
#if _WIN32
        HANDLE   Win_File;
#endif
        
        BaseFileStream(const std::string originPath, FileMode::Access::Type, const BlackRoot::Debug::Info);

        void CloseAndRelease() override;
        
        bool  CanSeek() const { return true; }
        
        FSize Seek(FSize) override;
        FSize SeekStart() override;
        FSize SeekEnd()  override;
        FSize SeekRelative(FSize) override;

        FSize GetRWPoint() override;

        FileManipLength Read(void *buffer, FileManipLength byteCount) override;
        FileManipLength Write(void *buffer, FileManipLength byteCount) override;

        FCont ReadUntilEnd() override;
        FSize ReadUntilEnd(void ** out) override;
    };

}
}