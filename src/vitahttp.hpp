#pragma once

#include "http.hpp"
#include "pkgi.hpp"

struct pkgi_http;

class VitaHttp : public Http
{
public:
    ~VitaHttp();

    void start(const std::string& url, uint64_t offset, bool head = false) override;
    int64_t read(uint8_t* buffer, uint64_t size) override;
    void abort() override;
    void close() override;

    int get_status() override;
    int64_t get_length() override;

    explicit operator bool() const override;

private:
    pkgi_http* _http = nullptr;
    bool _status_checked = false;

    void check_status();
};
