#include "filehttp.hpp"

#include "log.hpp"

FileHttp::FileHttp(const std::string& path) : override_path(path)
{
}

void FileHttp::start(const std::string& url, uint64_t offset, bool head)
{
    LOGF("Fake downloading {}", url);
    f.open(override_path.empty() ? url : override_path);
    f.seekg(offset, std::ios::beg);
}

int64_t FileHttp::read(uint8_t* buffer, uint64_t size)
{
    f.read(reinterpret_cast<char*>(buffer), size);
    return f.gcount();
}

void FileHttp::abort()
{
}

void FileHttp::close()
{
}

int FileHttp::get_status()
{
    return 200;
}

int64_t FileHttp::get_length()
{
    const uint64_t pos = f.tellg();
    f.seekg(0, std::ios::end);
    const uint64_t size = f.tellg();
    f.seekg(pos, std::ios::beg);
    return size;
}

FileHttp::operator bool() const
{
    return f.is_open();
}
