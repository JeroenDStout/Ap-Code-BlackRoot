/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <exception>
#include <string>
#include <thread>

#include "BlackRoot/Pubc/Sys Path.h"
#include "BlackRoot/Pubc/Stringstream.h"

#define BR_GENERATE_DEBUG_INFO

#ifdef BR_GENERATE_DEBUG_INFO
    #ifdef _VISUAL_STUDIO
        #define BRGenDbgInfo     (BlackRoot::Debug::Info(__PROJECTSTR__, __FILE__, __LINE__))
        #define BRGenDbgInfoPtr  (new BlackRoot::Debug::Info(__PROJECTSTR__, __FILE__, __LINE__))
    #else
        error No code for build
    #endif
#else
    #define BRGenDbgInfo     (BlackRoot::Debug::Info())
    #define BRGenDbgInfoPtr  (nullptr)
#endif

namespace BlackRoot {
namespace Debug {

    struct Info {
        std::thread::id  Thread;
        std::string      Location;
        int              Line;

        Info();
        Info(const char * project, const char * file, int line);

        bool HasInfo() const { return Line != -1; }
    };

    class Exception : public std::exception {
    protected:
        Debug::Info         Info;
        std::string         Description;
        Exception           *InnerException;
        std::string         PrettyDescription;

    public:
        Exception(const Debug::Info);
        Exception(const std::string, const Debug::Info);
        Exception(Exception *, const Debug::Info);
        Exception(const std::string, Exception *, const Debug::Info);
        ~Exception() override;

        virtual void CreatePrettyDescription();

        const Exception * GetInnerException() const { return this->InnerException; }
        virtual const char * GetType() const { return "Exception"; }
        
        virtual const std::string GetPrettyDescription() const { return this->PrettyDescription; }
        const char * what() const override { return this->GetPrettyDescription().c_str(); }
    };

    inline Info::Info() : Thread(std::this_thread::get_id()), Line(-1) {
    }

    inline Info::Info(const char * project, const char * path, int line)
    : Thread(std::this_thread::get_id()), Line(line) {
        std::string file = path;
        auto pos = file.rfind(BlackRoot::System::DirSeperator);
        if (pos != std::string::npos) {
            file.erase(0, pos+1);
        }

        this->Location = project;
        this->Location += ", ";
        this->Location += file;
    }

    inline Exception::Exception(const Debug::Info info)
    : InnerException(nullptr), Info(info)
    {
        this->CreatePrettyDescription();
    }

    inline Exception::Exception(const std::string desc, const Debug::Info info)
    : Description(desc), InnerException(nullptr), Info(info)
    {
        this->CreatePrettyDescription();
    }

    inline Exception::Exception(Exception * inner, const Debug::Info info)
    : InnerException(inner), Info(info)
    {
        this->CreatePrettyDescription();
    }

    inline Exception::Exception(const std::string desc, Exception * inner, const Debug::Info info)
    : Description(desc), InnerException(inner), Info(info)
    {
        this->CreatePrettyDescription();
    }

    inline Exception::~Exception()
    {
        delete this->InnerException;
    }

    inline void Exception::CreatePrettyDescription()
    {
        std::stringstream ss;

        bool anyLine = false;

        ss << this->GetType() << ": ";

        if (this->Info.HasInfo()) {
            std::hash<std::thread::id> hasher;
            ss << this->Info.Location << " : l." << this->Info.Line << " : t." << std::hex << hasher(this->Info.Thread) << std::dec;
            anyLine = true;
        }

        if (this->Description.length() > 0) {
            if (anyLine) ss << std::endl;
            ss << " \"" << this->Description << "\"";
            anyLine = true;
        }

        if (nullptr != this->GetInnerException()) {
            ss << std::endl;
            ss << "\\ " << this->InnerException->GetPrettyDescription();
            anyLine = true;
        }

        this->PrettyDescription = ss.str();
    }
    
}
}