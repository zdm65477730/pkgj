#pragma once

#include <fmt/format.h>

#include <stdexcept>

// printf to serial port, please:
// 1. Install PSM Usb Serial Drivers.
// 2. Add PSMLogUSB.skprx to your config.txt and reboot.
// 3. Open your favorite serial monitoring program and set the correct COM port. Set the baudrate to 57600.
// 4. In the application you wish to log use: 
//    ksceDebugPrintf, printf (when SceLibc is included such as in games), or sceClibPrintf

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
        printf(msg, ##__VA_ARGS__); \
    } while (0)
#define LOGF(msg, ...)                                     \
    do                                                     \
    {                                                      \
        printf(fmt::format(msg, ##__VA_ARGS__).c_str()); \
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
