/*
 *
 *  © Stout Games 2019
 *
 */

#pragma once

#include "BlackRoot/Pubc/Exception.h"

namespace BlackRoot {
namespace Debug {

    class OriginException : public Exception {
    public:
        Debug::Info         OriginInfo;
        
        OriginException(const Debug::Info originInfo, const Debug::Info = BRGenDbgInfo);
        OriginException(const Debug::Info originInfo, const std::string, const Debug::Info = BRGenDbgInfo);
        OriginException(const Debug::Info originInfo, Exception *, const Debug::Info = BRGenDbgInfo);
        OriginException(const Debug::Info originInfo, const std::string, Exception *, const Debug::Info = BRGenDbgInfo);

        void CreatePrettyDescription() override;
    };

    inline OriginException::OriginException(const Debug::Info originInfo, const Debug::Info info)
    : Exception(info), OriginInfo(originInfo)
    {
        this->CreatePrettyDescription();
    }

    inline OriginException::OriginException(const Debug::Info originInfo, const std::string desc, const Debug::Info info)
    : Exception(desc, info), OriginInfo(originInfo)
    {
        this->CreatePrettyDescription();
    }

    inline OriginException::OriginException(const Debug::Info originInfo, Exception * inner, const Debug::Info info)
    : Exception(inner, info), OriginInfo(originInfo)
    {
        this->CreatePrettyDescription();
    }

    inline OriginException::OriginException(const Debug::Info originInfo, const std::string desc, Exception * inner, const Debug::Info info)
    : Exception(desc, inner, info), OriginInfo(originInfo)
    {
        this->CreatePrettyDescription();
    }

    inline void OriginException::CreatePrettyDescription()
    {
        std::stringstream ss;

        ss << "- <" << this->GetType() << ">:";
        if (this->Info.HasInfo()) {
            std::hash<std::thread::id> hasher;
            ss <<std::endl << "Thrown in " << this->Info.Location << ", l. " <<  std::dec << this->Info.Line << ", thread " << std::hex << hasher(this->Info.Thread);
        }
        if (this->OriginInfo.HasInfo()) {
            std::hash<std::thread::id> hasher;
            ss << std::endl << "Origin in " << this->Info.Location << ", l. " <<  std::dec << this->Info.Line << ", thread " << std::hex << hasher(this->Info.Thread);
        }
        if (this->Description.length() > 0) {
            ss << std::endl << "  " << this->Description;
        }
        if (nullptr != this->GetInnerException()) {
            ss << std::endl << std::endl;
            ss << this->InnerException->GetPrettyDescription();
        }

        this->Description = ss.str();
    }
}
}