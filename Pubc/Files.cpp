#include <fstream>

#include "BlackRoot/Pubc/Stringstream.h"
#include "BlackRoot/Pubc/Number Types.h"
#include "BlackRoot/Pubc/Files.h"
#include "BlackRoot/Pubc/Exception Origin.h"
#include "BlackRoot/Pubc/Assert.h"

using namespace BlackRoot::IO;

// ------------------------------------------------------------------------------------------------------
//              Interface File Stream
// ------------------------------------------------------------------------------------------------------

IFileStream::IFileStream(const std::string originPath, FileMode::Access::Type access, const BlackRoot::Debug::Info dbInfo)
: OriginPath(originPath), AccessType(access), DbOriginInfo(dbInfo)
{
}

BlackRoot::Debug::Exception * IFileStream::CreateException(const std::string str)
{
    return new BlackRoot::Debug::OriginException(this->DbOriginInfo, (std::stringstream(str) << ", with path «" << this->OriginPath << "»").str(), {});
}

// ------------------------------------------------------------------------------------------------------
//              Base File Manager
// ------------------------------------------------------------------------------------------------------

bool BaseFileManager::ManagerIsReadOnly()
{
    return false;
}

void BaseFileManager::Copy(const FilePath from, const FilePath to)
{
    std::experimental::filesystem::copy(from, to);
}

void BaseFileManager::CopyFile(const FilePath from, const FilePath to)
{
    std::experimental::filesystem::copy_file(from, to);
}

bool BaseFileManager::Exists(const FilePath path)
{
    return std::experimental::filesystem::exists(path);
}

bool BaseFileManager::FileExists(const FilePath path)
{
    namespace fs = std::experimental::filesystem;

    fs::file_status s = fs::status(path);
    return fs::status_known(s) && s.type() == fs::file_type::regular;
}

bool BaseFileManager::DirectoryExists(const FilePath path)
{
    namespace fs = std::experimental::filesystem;

    fs::file_status s = fs::status(path);
    return fs::status_known(s) && s.type() == fs::file_type::directory;
}

BaseFileManager::FSize BaseFileManager::FileSize(const FilePath path)
{
    return std::experimental::filesystem::file_size(path);
}

BaseFileManager::FTime BaseFileManager::LastWriteTime(const FilePath path)
{
    return std::experimental::filesystem::last_write_time(path);
}

void BaseFileManager::Remove(const FilePath path)
{
    std::experimental::filesystem::remove(path);
}

void BaseFileManager::RemoveAll(const FilePath path)
{
    std::experimental::filesystem::remove_all(path);
}

void BaseFileManager::Rename(const FilePath from, const FilePath to)
{
    std::experimental::filesystem::rename(from, to);
}

void BaseFileManager::CreateDirectories(const FilePath path)
{
    std::experimental::filesystem::create_directories(path);
}

BaseFileManager::DirCon BaseFileManager::GetDirectoryContents(const FilePath path)
{
    DirCon dir;
    for(auto& p : std::experimental::filesystem::directory_iterator(path)) {
        dir.Paths.push_back(p.path());
    }
    return dir;
}

BaseFileManager::Stream * BaseFileManager::OpenFile(const FilePath fPath, const FileMode::OpenInstr instr, BlackRoot::Debug::Info dbInfo)
{    
#if _WIN32
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    std::wstring path = L"\?";
    path += (this->BasePath / fPath);

    DWORD access = 0x0;
    access |= (instr.MAccess & FileMode::Access::Read)?   GENERIC_READ  : 0;
    access |= (instr.MAccess & FileMode::Access::Write)?  GENERIC_WRITE : 0;

    DWORD shareMode = 0x0;
    shareMode |= (instr.MShare    & FileMode::Share::Delete)?  FILE_SHARE_DELETE : 0;
    shareMode |= (instr.MShare    & FileMode::Share::Read)?    FILE_SHARE_READ   : 0;
    shareMode |= (instr.MShare    & FileMode::Share::Write)?   FILE_SHARE_WRITE  : 0;

    LPSECURITY_ATTRIBUTES securityAttr = nullptr;

    DWORD creation = 0x0;
    switch (instr.MCreation) {
    default:
        throw new BlackRoot::Debug::Exception((std::stringstream("Invalid creation parameters opening file: «") << converter.to_bytes(path).c_str() << "»").str());
    case FileMode::Creation::CreateAlways:
        creation = CREATE_ALWAYS; break;
    case FileMode::Creation::CreateNew:
        creation = CREATE_NEW; break;
    case FileMode::Creation::OpenAlways:
        creation = OPEN_ALWAYS; break;
    case FileMode::Creation::OpenExisting:
        creation = OPEN_EXISTING; break;
    case FileMode::Creation::TruncateExisting:
        creation = TRUNCATE_EXISTING; break;
    }
    
    DWORD attribute = 0x0;
    attribute |= (instr.MAttributes & FileMode::Attributes::Hidden)?  FILE_ATTRIBUTE_HIDDEN     : 0;

    HANDLE handle = ::CreateFileW(path.c_str(), access, shareMode, securityAttr, creation, attribute, NULL);
    if (handle == INVALID_HANDLE_VALUE) {
        throw new BlackRoot::Debug::Exception((std::stringstream("{WIN} Cannot create file: «") << converter.to_bytes(path).c_str() << "»").str());
    }
#endif

    BaseFileStream * stream = new BaseFileStream(fPath.string(), instr.MAccess, dbInfo);
    
#if _WIN32
    stream->Win_File = handle;
#endif

    return stream;
}

BaseFileManager::FSize BaseFileManager::ReadFile(const FilePath fPath, void ** out, const FileMode::OpenInstr instr)
{
    FilePath path = this->BasePath / fPath;

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw new BlackRoot::Debug::Exception((std::stringstream("Cannot read file: «") << path << "»").str());
    }

    file.seekg(0, std::ios::end);
    uint32 length = (uint32)(file.tellg());

    file.seekg(0);

    *out = new char[length];

    file.read((char*)(*out), length);

    return length;
}

BaseFileManager::FCont BaseFileManager::ReadFile(const FilePath fPath, const FileMode::OpenInstr instr)
{
    FilePath path = this->BasePath / fPath;

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw new BlackRoot::Debug::Exception((std::stringstream("Cannot read file: «") << path << "»").str());
    }

    file.seekg(0, std::ios::end);
    uint32 length = (uint32)file.tellg();

    file.seekg(0);

    FCont contents;
    contents.resize(length);

    file.read(&contents[0], length);

    return contents;
}

BaseFileManager::FStr BaseFileManager::ReadFileAsString(const FilePath fPath, const FileMode::OpenInstr instr)
{
    FilePath path = this->BasePath / fPath;

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw new BlackRoot::Debug::Exception((std::stringstream("Cannot read file: «") << path << "»").str(), BRGenDbgInfo);
    }

    file.seekg(0, std::ios::end);
    uint32 length = (uint32)file.tellg();

    file.seekg(0);

    FStr contents;
    contents.resize(length);

    file.read(&contents[0], length);

    return contents;
}

// ------------------------------------------------------------------------------------------------------
//              Base File Stream
// ------------------------------------------------------------------------------------------------------

BaseFileStream::BaseFileStream(const std::string originPath, FileMode::Access::Type access, const BlackRoot::Debug::Info dbInfo)
: IFileStream(originPath, access, dbInfo)
{
}

BaseFileStream::~BaseFileStream()
{
}

void BaseFileStream::CloseAndRelease()
{
#if _WIN32
    if (::CloseHandle(this->Win_File)) {
        return;
    }
    
    throw this->CreateException("Cannot close file handle on file.");
#endif

    delete this;
}

BaseFileStream::FSize BaseFileStream::Seek(FSize seek)
{
#if _WIN32
    LARGE_INTEGER s; s.QuadPart = seek;
    LARGE_INTEGER p;
    if (!SetFilePointerEx(this->Win_File, s, &p, FILE_BEGIN)) {
        throw this->CreateException((std::stringstream("Cannot seek to ") << seek).str());
    }

    return (FSize)p.QuadPart;
#endif
}

BaseFileStream::FSize BaseFileStream::SeekStart()
{
    return this->Seek(0);
}

BaseFileStream::FSize BaseFileStream::SeekEnd()
{
#if _WIN32
    LARGE_INTEGER s; s.QuadPart = 0x0;
    LARGE_INTEGER p;
    if (!SetFilePointerEx(this->Win_File, s, &p, FILE_END)) {
        throw this->CreateException("Cannot seek to end.");
    }

    return (FSize)p.QuadPart;
#endif
}

BaseFileStream::FSize BaseFileStream::SeekRelative(FSize seek)
{
#if _WIN32
    LARGE_INTEGER s; s.QuadPart = seek;
    LARGE_INTEGER p;
    if (!SetFilePointerEx(this->Win_File, s, &p, FILE_CURRENT)) {
        throw this->CreateException((std::stringstream("Cannot seek relative by ") << seek).str());
    }

    return (FSize)p.QuadPart;
#endif
}

BaseFileStream::FSize BaseFileStream::GetRWPoint()
{
    return this->SeekRelative(0x0);
}

FileManipLength BaseFileStream::Read(void *buffer, FileManipLength byteCount)
{
#ifdef BR_FILES_PARANOIA
    if (0 == (this->AccessType & FileMode::Access::Read)) {
        throw this->CreateException("File does not have reading rights.");
    }
#endif
    
    FSize bytesRead    = 0;

#if _WIN32
    using limit = std::numeric_limits<DWORD>;

    void            *bufferParce = buffer;
    FileManipLength bytesToRead  = byteCount;

    DWORD rd;
    while (bytesToRead > limit::max()) {
        ::ReadFile(this->Win_File, buffer, limit::max(), &rd, nullptr);
        bytesRead += (FSize)(rd);
        bytesToRead -= limit::max();
    }
    if (bytesToRead > 0) {
        ::ReadFile(this->Win_File, buffer, (DWORD)(bytesToRead), &rd, nullptr);
        bytesRead += (FSize)(rd);
    }
#endif

    return bytesRead;
}

FileManipLength BaseFileStream::Write(void *buffer, FileManipLength byteCount)
{
#ifdef BR_FILES_PARANOIA
    if (0 == (this->AccessType & FileMode::Access::Write)) {
        throw this->CreateException("File does not have writing rights.");
    }
#endif
    
    FSize bytesWritten    = 0;

#if _WIN32
    using limit = std::numeric_limits<DWORD>;

    void            *bufferParce = buffer;
    FileManipLength bytesToWrite = byteCount;

    DWORD rd;
    while (bytesToWrite > limit::max()) {
        ::WriteFile(this->Win_File, buffer, limit::max(), &rd, nullptr);
        bytesWritten += (FSize)(rd);
        bytesToWrite -= limit::max();
    }
    if (bytesToWrite > 0) {
        ::ReadFile(this->Win_File, buffer, (DWORD)(bytesToWrite), &rd, nullptr);
        bytesWritten += (FSize)(rd);
    }
#endif

    return (uint32)bytesWritten;
}

BaseFileStream::FCont BaseFileStream::ReadUntilEnd()
{
    FCont out;

    FSize curRW = this->GetRWPoint();
    FSize fileSize = this->SeekEnd();

    FSize rCount = fileSize - curRW;

    if (rCount == 0)
        return out;

    this->Seek(curRW);

    DbAssert(rCount < std::numeric_limits<size_t>::max());

    out.resize((size_t)rCount);
    
    this->Read(out.begin()._Ptr, (size_t)rCount);

    return out;
}

BaseFileStream::FSize BaseFileStream::ReadUntilEnd(void ** out)
{
    FSize curRW = this->GetRWPoint();
    FSize fileSize = this->SeekEnd();

    FSize rCount = fileSize - curRW;

    if (rCount == 0) {
        *out = nullptr;
        return 0x0;
    }
    
    DbAssert(rCount < std::numeric_limits<size_t>::max());

    this->Seek(curRW);

    *out = new char[(size_t)rCount];

    return this->Read(*out, (size_t)rCount);
}