#include "pkgi.hpp"
extern "C"
{
#include "style.h"
}

#include <fmt/format.h>

#include <boost/scope_exit.hpp>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PKGI_FOLDER "pkgi"
#define PKGI_APP_FOLDER "app"

void pkgi_log(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    printf("\n");
    va_end(args);
}

int pkgi_snprintf(char* buffer, uint32_t size, const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    int len = vsnprintf(buffer, size - 1, msg, args);
    va_end(args);
    buffer[len] = 0;
    return len;
}

void pkgi_vsnprintf(char* buffer, uint32_t size, const char* msg, va_list args)
{
    int len = vsnprintf(buffer, size - 1, msg, args);
    buffer[len] = 0;
}

char* pkgi_strstr(const char* str, const char* sub)
{
    return strstr((char*)str, sub);
}

int pkgi_stricontains(const char* str, const char* sub)
{
    return strcasestr(str, sub) != NULL;
}

int pkgi_stricmp(const char* a, const char* b)
{
    return strcasecmp(a, b);
}

void pkgi_strncpy(char* dst, uint32_t size, const char* src)
{
    strncpy(dst, src, size);
}

char* pkgi_strrchr(const char* str, char ch)
{
    return strrchr((char*)str, ch);
}

void pkgi_memcpy(void* dst, const void* src, uint32_t size)
{
    memcpy(dst, src, size);
}

void pkgi_memmove(void* dst, const void* src, uint32_t size)
{
    memmove(dst, src, size);
}

int pkgi_memequ(const void* a, const void* b, uint32_t size)
{
    return memcmp(a, b, size) == 0;
}

int pkgi_is_unsafe_mode(void)
{
    return 1;
}

bool pkgi_file_exists(const std::string& path)
{
    struct stat s;
    return stat(path.c_str(), &s) == 0;
}

int64_t pkgi_get_size(const char*)
{
    return 0;
}

void pkgi_rename(const std::string& from, const std::string& to)
{
    int res = rename(from.c_str(), to.c_str());
    if (res < 0)
        throw std::runtime_error(fmt::format(
                "无法将 {} 重命名为 {}:\n{:#08x}",
                from,
                to,
                strerror(errno)));
}

void pkgi_mkdirs(const char* ppath)
{
    std::string path = ppath;
    path.push_back('/');
    auto ptr = path.begin();
    while (true)
    {
        ptr = std::find(ptr, path.end(), '/');
        if (ptr == path.end())
            break;

        char last = *ptr;
        *ptr = 0;
        LOG("mkdir %s", path.c_str());
        int err = mkdir(path.c_str(), 0777);
        if (err < 0 && errno != EEXIST)
            throw std::runtime_error(fmt::format(
                    "新建文件夹 ({}) 失败: {:#08x}",
                    path.c_str(),
                    static_cast<uint32_t>(err)));
        *ptr = last;
        ++ptr;
    }
}

void pkgi_rm(const char* file)
{
    unlink(file);
}

void pkgi_delete_dir(const std::string& path)
{
    DIR* dfd = opendir(path.c_str());
    if (!dfd && errno == ENOENT)
        return;

    if (!dfd)
        throw formatEx<std::runtime_error>(
                "无法打开 ({}): {}", path, strerror(errno));

    BOOST_SCOPE_EXIT_ALL(&)
    {
        if (dfd)
            closedir(dfd);
    };

    int res = 0;
    errno = 0;
    struct dirent* dir;
    while ((dir = readdir(dfd)))
    {
        std::string d_name = dir->d_name;

        if (d_name == "." || d_name == "..")
            continue;

        std::string new_path =
                path + (path[path.size() - 1] == '/' ? "" : "/") + d_name;

        if (dir->d_type == DT_DIR)
            pkgi_delete_dir(new_path);
        else
        {
            const auto ret = unlink(new_path.c_str());
            if (ret < 0)
                throw formatEx<std::runtime_error>(
                        "取消链接失败 ({}): {}", new_path, strerror(errno));
        }
    }

    closedir(dfd);
    dfd = nullptr;

    res = rmdir(path.c_str());
    if (res < 0)
        throw formatEx<std::runtime_error>(
                "无法移动文件夹 ({}): {}", path, strerror(errno));
}

std::vector<uint8_t> pkgi_load(const std::string& path)
{
    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0)
        throw std::runtime_error(fmt::format(
                "打开 ({}) 失败: {:#08x}", path, static_cast<uint32_t>(fd)));

    BOOST_SCOPE_EXIT_ALL(&)
    {
        close(fd);
    };

    const auto size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    std::vector<uint8_t> data(size);

    const auto readsize = read(fd, data.data(), data.size());
    if (readsize < 0)
        throw std::runtime_error(fmt::format(
                "读取 ({}) 失败: {:#08x}",
                path.c_str(),
                static_cast<uint32_t>(readsize)));

    data.resize(readsize);

    return data;
}

int pkgi_load(const char* name, void* data, uint32_t max)
{
    int fd = open(name, O_RDONLY, 0777);
    if (fd < 0)
        return -1;

    char* data8 = static_cast<char*>(data);

    int total = 0;
    while (max != 0)
    {
        int readd = read(fd, data8 + total, max);
        if (readd < 0)
        {
            total = -1;
            break;
        }
        else if (readd == 0)
        {
            break;
        }
        total += readd;
        max -= readd;
    }

    close(fd);
    return total;
}

int pkgi_save(const char* name, const void* data, uint32_t size)
{
    int fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
        return 0;

    int ret = 1;
    const char* data8 = static_cast<const char*>(data);
    while (size != 0)
    {
        int written = write(fd, data8, size);
        if (written <= 0)
        {
            ret = 0;
            break;
        }
        data8 += written;
        size -= written;
    }

    close(fd);
    return ret;
}

void pkgi_save(const std::string& path, const void* data, uint32_t size)
{
    int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
        throw std::runtime_error(fmt::format(
                "打开 ({}) 失败:\n{:#08x}", path, static_cast<uint32_t>(fd)));

    BOOST_SCOPE_EXIT_ALL(&)
    {
        close(fd);
    };

    const char* data8 = static_cast<const char*>(data);
    while (size != 0)
    {
        int written = write(fd, data8, size);
        if (written <= 0)
            throw std::runtime_error(fmt::format(
                    "写入 ({}) 失败:\n{:#08x}",
                    path,
                    static_cast<uint32_t>(written)));
        data8 += written;
        size -= written;
    }
}

void* pkgi_create(const std::string& path)
{
    LOGF("pkgi_create {}", path);
    int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
        throw std::runtime_error("PKGi创建失败");

    return (void*)(intptr_t)fd;
}

void* pkgi_openrw(const char* path)
{
    int fd = open(path, O_RDWR, 0777);
    if (fd < 0)
        return NULL;

    return (void*)(intptr_t)fd;
}

int64_t pkgi_seek(void* f, uint64_t offset)
{
    return lseek((intptr_t)f, offset, SEEK_SET);
}

int pkgi_read(void* f, void* buffer, uint32_t size)
{
    return read((intptr_t)f, buffer, size);
}

int pkgi_write(void* f, const void* buffer, uint32_t size)
{
    const auto wrote = write((intptr_t)f, buffer, size);
    if (wrote < 0)
        throw formatEx<std::runtime_error>(
                "无法写入文件:\n{}", strerror(errno));
    return wrote;
}

void pkgi_close(void* f)
{
    close((intptr_t)f);
}

uint32_t pkgi_time_msec()
{
    return time(NULL) * 1000;
}
