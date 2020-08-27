#pragma once

#include <fmt/format.h>

#include <stdexcept>

// To use printf, and output to serial port, please:
// 1. Install PSM Usb Serial Drivers.
// 2. Add PSMLogUSB.skprx to your config.txt and reboot.
// 3. Open your favorite serial monitoring program and set the correct COM port. Set the baudrate to 57600.
// 4. In the application you wish to log use: 
//    ksceDebugPrintf, printf (when SceLibc is included such as in games), or sceClibPrintf
// Refer to https://github.com/CelesteBlue-dev/PSVita-RE-tools

#ifdef PKGI_ENABLE_SERIAL_LOGGING
#include <time.h>
#include <sys/time.h>
#define PRELOG(s) \
    char s[256]; memset(s, 0, sizeof(s)); \
    struct timeval tv; struct timezone tz; struct tm *p; gettimeofday(&tv, &tz); p = localtime(&tv.tv_sec); \
    sprintf(s,"%04d-%02d-%02d %02d:%02d:%02d.%03d %s:%d ", static_cast<short>(p->tm_year + 1900), static_cast<char>(p->tm_mon + 1), static_cast<char>(p->tm_mday), static_cast<char>(p->tm_hour), static_cast<char>(p->tm_min), static_cast<char>(p->tm_sec), static_cast<short>(tv.tv_usec/1000), __BASE_FILE__, __LINE__);
#endif

#ifdef PKGI_ENABLE_LOGGING
#define LOG(msg, ...)                 \
    do                                \
    {                                 \
        pkgi_log(msg, ##__VA_ARGS__); \
    } while (0)
#define LOGF(msg, ...)                                     \
    do                                                     \
    {                                                      \
        pkgi_log(fmt::format(msg, ##__VA_ARGS__).c_str()); \
    } while (0)
#elif defined PKGI_ENABLE_SERIAL_LOGGING
#define LOG(msg, ...)                 \
    do                                \
    {                                 \
        PRELOG(szDate)                  \
        printf("%s ", szDate); printf(msg"\n\r", ##__VA_ARGS__); \
    } while (0)
#define LOGF(msg, ...)                                     \
    do                                                     \
    {                                                      \
        PRELOG(szDate)                                       \
        printf("%s %s\n\r", szDate, fmt::format(msg, ##__VA_ARGS__).c_str()); \
    } while (0)
#else
#define LOG(...) \
    do           \
    {            \
    } while (0)
#define LOGF(...) \
    do            \
    {             \
    } while (0)
#endif

template <typename E = std::runtime_error, typename... Args>
[[nodiscard]] E formatEx(Args&&... args) {
    return E(fmt::format(std::forward<Args>(args)...));
}

void pkgi_log(const char* msg, ...);
