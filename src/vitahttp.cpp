#include "vitahttp.hpp"

#include <psp2/io/fcntl.h>
#include <psp2/net/http.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>

#include <fmt/format.h>

#include <boost/scope_exit.hpp>

#define PKGI_USER_AGENT "libhttp/3.65 (PS Vita)"

struct pkgi_http
{
    int used;

    int tmpl;
    int conn;
    int req;
};

namespace
{
static pkgi_http g_http[4];
}

VitaHttp::~VitaHttp()
{
    close();
}

void VitaHttp::close()
{
    if (_http)
    {
        LOG("http close");
        sceHttpDeleteRequest(_http->req);
        sceHttpDeleteConnection(_http->conn);
        sceHttpDeleteTemplate(_http->tmpl);
        _http->used = 0;
        _http = nullptr;
    }
}

void VitaHttp::start(const std::string& url, uint64_t offset, bool head)
{
    if (_http)
        throw HttpError("HTTP连接已启动");

    LOG("http get");

    pkgi_http* http = NULL;
    for (size_t i = 0; i < 4; i++)
    {
        if (g_http[i].used == 0)
        {
            http = g_http + i;
            break;
        }
    }

    if (!http)
        throw HttpError("内部错误: 同时发起的连接过多");

    int tmpl = -1;
    int conn = -1;
    int req = -1;

    LOGF("starting http GET request for {}", url);

    if ((tmpl = sceHttpCreateTemplate(
                 PKGI_USER_AGENT, SCE_HTTP_VERSION_1_1, SCE_TRUE)) < 0)
        throw HttpError(fmt::format(
                "创建模板失败: {:#08x}",
                static_cast<uint32_t>(tmpl)));
    BOOST_SCOPE_EXIT_ALL(&)
    {
        if (tmpl > 0)
            sceHttpDeleteTemplate(tmpl);
    };
    // sceHttpSetRecvTimeOut(tmpl, 10 * 1000 * 1000);

    if ((conn = sceHttpCreateConnectionWithURL(tmpl, url.c_str(), SCE_FALSE)) <
        0)
        throw HttpError(fmt::format(
                "创建与链接的连接失败: {:#08x}",
                static_cast<uint32_t>(conn)));
    BOOST_SCOPE_EXIT_ALL(&)
    {
        if (conn > 0)
            sceHttpDeleteConnection(conn);
    };

    if ((req = sceHttpCreateRequestWithURL(
                 conn,
                 head ? SCE_HTTP_METHOD_HEAD : SCE_HTTP_METHOD_GET,
                 url.c_str(),
                 0)) < 0)
        throw HttpError(fmt::format(
                "创建链接的请求失败: {:#08x}",
                static_cast<uint32_t>(req)));
    BOOST_SCOPE_EXIT_ALL(&)
    {
        if (req > 0)
            sceHttpDeleteRequest(req);
    };

    int err;

    if (offset != 0 && !head)
    {
        char range[64];
        pkgi_snprintf(range, sizeof(range), "bytes=%llu-", offset);
        if ((err = sceHttpAddRequestHeader(
                     req, "Range", range, SCE_HTTP_HEADER_ADD)) < 0)
            throw HttpError(fmt::format(
                    "添加请求文件头失败: {:#08x}",
                    static_cast<uint32_t>(err)));
    }

    if ((err = sceHttpSendRequest(req, NULL, 0)) < 0)
    {
        std::string err_msg;
        switch (static_cast<uint32_t>(err))
        {
            case 0x80431063UL:
                err_msg = "网络错误";
                break;
            case 0x80431068UL:
                err_msg = "网络超时";
                break;
            case 0x80431082UL:
                err_msg = "请求被阻塞";
                break;
            case 0x80436007UL:
                err_msg = fmt::format("主机不存在, 建议删除 {} 后重试", pkgi_get_config_folder());
                break;
            case 0x80431084UL:
                err_msg = "代理错误";
                break;
            case 0x80431075UL:
                err_msg = "SSL错误";
                break;
            default:
                err_msg = "";
        }

        throw formatEx<HttpError>("发送请求失败: {:#08x}\n{}",
            static_cast<uint32_t>(err),
            err_msg);
    }

    http->used = 1;
    http->tmpl = tmpl;
    http->conn = conn;
    http->req = req;
    tmpl = conn = req = -1;

    _http = http;
}

int64_t VitaHttp::read(uint8_t* buffer, uint64_t size)
{
    check_status();

    int read = sceHttpReadData(_http->req, buffer, size);
    if (read < 0)
        throw HttpError(fmt::format(
                "下载错误 {:#08x}",
                static_cast<uint32_t>(static_cast<int32_t>(read))));
    return read;
}

void VitaHttp::abort()
{
    if (_http)
    {
        const auto err = sceHttpAbortRequest(_http->req);
        if (err)
            LOGF("abort() failed: {:#08x}", static_cast<uint32_t>(err));
    }
}

int64_t VitaHttp::get_length()
{
    check_status();

    int res;
    uint64_t content_length;
    res = sceHttpGetResponseContentLength(_http->req, &content_length);
    if (res < 0)
        throw HttpError(fmt::format(
                "获取响应内容长度失败: {:#08x}",
                static_cast<uint32_t>(res)));
    if (res == (int)SCE_HTTP_ERROR_NO_CONTENT_LENGTH ||
        res == (int)SCE_HTTP_ERROR_CHUNK_ENC)
    {
        LOG("http response has no content length (or chunked "
            "encoding)");
        return 0;
    }

    LOGF("http response length = {}", content_length);
    return content_length;
}

int VitaHttp::get_status()
{
    int res;
    int status;
    if ((res = sceHttpGetStatusCode(_http->req, &status)) < 0)
        throw HttpError(fmt::format(
                "获取状态代码失败: {:#08x}",
                static_cast<uint32_t>(res)));

    return status;
}

void VitaHttp::check_status()
{
    if (_status_checked)
        return;
    _status_checked = true;

    const auto status = get_status();

    LOGF("http status code = {}", status);

    if (status == 404)
        throw HttpError(fmt::format("未找到列表, 建议删除 {} 后重试", pkgi_get_config_folder()));
    if (status != 200 && status != 206)
        throw HttpError(fmt::format("HTTP状态异常: {}", status));
}

VitaHttp::operator bool() const
{
    return _http;
}
