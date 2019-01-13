#include "dialog.hpp"
#include "file.hpp"
#include "pkgi.hpp"
#include "vitahttp.hpp"

#include <boost/scope_exit.hpp>

#include <vector>

#define PKGJ_UPDATE_URL "http://47.100.37.250/pkgj"
#define PKGJ_UPDATE_URL_VERSION PKGJ_UPDATE_URL "/version"

namespace
{
std::string version;

void start_download()
{
    try
    {
        LOGF("starting downloading version {}", version);

        const auto filename = fmt::format(
                "{}/pkgj-v{}.vpk", pkgi_get_config_folder(), version);
        const auto url =
                fmt::format("{}/files/pkgj-v{}.vpk", PKGJ_UPDATE_URL, version);

        pkgi_dialog_message("正在下載新版本安裝文件...", 0);

        try
        {
            const auto file = pkgi_create(filename.c_str());
            BOOST_SCOPE_EXIT_ALL(&)
            {
                pkgi_close(file);
            };

            VitaHttp http;
            http.start(url, 0);
            std::vector<uint8_t> data(64 * 1024);
            while (true)
            {
                const auto read = http.read(data.data(), data.size());
                if (read == 0)
                    break;
                pkgi_write(file, data.data(), read);
            }

            LOGF("update download complete");
        }
        catch (...)
        {
            LOGF("download failed, removing partial file");
            pkgi_rm(filename.c_str());
            throw;
        }

        pkgi_dialog_message(
                fmt::format(
                        "新版本PKGj安裝文件已下載至{}, 請使"
                        "用VitaShell進行安裝",
                        filename)
                        .c_str());
    }
    catch (const std::exception& e)
    {
        pkgi_dialog_error(fmt::format("下載失敗: {}", e.what()).c_str());
    }
}

void update_thread()
{
    try
    {
        LOGF("checking latest pkgi version at {}", PKGJ_UPDATE_URL_VERSION);

        VitaHttp http;
        http.start(PKGJ_UPDATE_URL_VERSION, 0);
        std::vector<uint8_t> last_versionb(10);
        last_versionb.resize(
                http.read(last_versionb.data(), last_versionb.size()));
        std::string last_version(last_versionb.begin(), last_versionb.end());

        LOGF("last version is {}", last_version);

        if (last_version != PKGI_VERSION)
        {
            LOG("new version available");

            version = last_version;

            pkgi_dialog_question(
                    fmt::format(
                            "PKGj中文版現已更新至{}版本!\n是否立即"
                            "下載?",
                            last_version)
                            .c_str(),
                    {{"立即下載",
                      [] {
                          pkgi_start_thread(
                                  "pkgj_update_download", &start_download);
                      }},
                     {"以後再説", [] {}}});
        }
    }
    catch (const std::exception& e)
    {
        LOGF("error in update thread: {}", e.what());
    }
}
}

void start_update_thread()
{
    pkgi_start_thread("pkgj_update", &update_thread);
}
