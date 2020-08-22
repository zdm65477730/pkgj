#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

class HttpError : public std::exception
{
public:
    HttpError(std::string msg) : _msg("HttpError: " + std::move(msg))
    {
    }

    virtual const char* what() const noexcept override
    {
        return _msg.c_str();
    }

private:
    std::string _msg;
};

class Http
{
public:
    virtual ~Http()
    {
    }

    virtual void start(const std::string& url, uint64_t offset, bool head = false) = 0;
    virtual int64_t read(uint8_t* buffer, uint64_t size) = 0;
    virtual void abort() = 0;
    virtual void close() = 0;

    virtual int get_status() = 0;
    virtual int64_t get_length() = 0;

    virtual explicit operator bool() const = 0;
};
