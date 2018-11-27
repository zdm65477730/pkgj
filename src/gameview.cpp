#include "gameview.hpp"

#include <fmt/format.h>

#include "dialog.hpp"
#include "imgui.hpp"
extern "C"
{
#include "style.h"
}

namespace
{
constexpr unsigned GameViewWidth = VITA_WIDTH * 0.8;
constexpr unsigned GameViewHeight = VITA_HEIGHT * 0.8;
}

void GameView::render()
{
    ImGui::SetNextWindowPos(
            ImVec2((VITA_WIDTH - GameViewWidth) / 2,
                   (VITA_HEIGHT - GameViewHeight) / 2));
    ImGui::SetNextWindowSize(ImVec2(GameViewWidth, GameViewHeight), 0);

    ImGui::Begin(
            fmt::format("{}###gameview", _item->titleid)
                    .c_str(),
            nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoScrollbar |
                    ImGuiWindowFlags_NoScrollWithMouse |
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoInputs);

    ImGui::PushTextWrapPos(0.f);
    ImGui::Text(fmt::format("當前系統固件版本: {}", pkgi_get_system_version())
                        .c_str());
    ImGui::Text(fmt::format("運行所需固件版本: {}", _item->fw_version)
                        .c_str());

    ImGui::Text(" ");

    ImGui::Text(fmt::format(
                        "游戲安裝及版本更新情況: {}",
                        _game_version.empty() ? "未安裝" : _game_version)
                        .c_str());
    if (_comppack_versions.present && _comppack_versions.base.empty() &&
        _comppack_versions.patch.empty())
    {
        ImGui::Text("已安裝的游戲兼容包: 未知版本");
    }
    else
    {
        ImGui::Text(fmt::format(
                            "游戲本體兼容包安裝情況: {}",
                            _comppack_versions.base.empty() ? "未安裝" : "已安裝")
                            .c_str());
        ImGui::Text(fmt::format(
                            "游戲更新兼容包及版本情況: {}",
                            _comppack_versions.patch.empty()
                                    ? "未安裝"
                                    : _comppack_versions.patch)
                            .c_str());
    }

    ImGui::Text(" ");

    printDiagnostic();

    ImGui::Text(" ");

    ImGui::PopTextWrapPos();

    if (!_downloader->is_in_queue(_item->content))
    {
        if (ImGui::Button("安裝游戲本體###installgame"))
            start_download_package();
    }
    else
    {
        if (ImGui::Button("取消安裝游戲本體###installgame"))
            cancel_download_package();
    }

    if (_base_comppack)
        if (ImGui::Button("安裝游戲本體兼容包"))
            start_download_comppack(false);
    if (_patch_comppack)
        if (ImGui::Button(fmt::format(
                                  "安裝游戲更新兼容包 {}",
                                  _patch_comppack->app_version)
                                  .c_str()))
            start_download_comppack(true);

    // HACK: comppack are identified by their titleid instead of content id
    if (_downloader->is_in_queue(_item->titleid))
        if (ImGui::Button("取消安裝游戲兼容包"))
            cancel_download_comppacks();

    if (ImGui::Button("關閉"))
        _closed = true;

    ImGui::End();
}

static const auto Red = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
static const auto Yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
static const auto Green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

void GameView::printDiagnostic()
{
    bool ok = true;
    auto const printError = [&](auto const& str) {
        ok = false;
        ImGui::TextColored(Red, str);
    };

    auto const systemVersion = pkgi_get_system_version();
    auto const& minSystemVersion = _item->fw_version;

    ImGui::Text("運行診斷:");

    if (systemVersion < minSystemVersion)
    {
        if (!_comppack_versions.present)
            printError(
                    "- 當前固件版本低於游戲運行所需固件版本, 必須"
                    "安裝兼容包");
    }
    else
    {
        ImGui::Text(
                "- 當前固件版本已高於游戲運行所需固件版本, 無需安裝兼容"
                "包");
    }

    if (_comppack_versions.present && _comppack_versions.base.empty() &&
        _comppack_versions.patch.empty())
    {
        ImGui::TextColored(
                Yellow,
                "- 游戲兼容包已安裝,但并非通過PKGj進行安裝, 請"
                "確保該兼容包與游戲版本相匹配, 如出現運行異常, 請通過PKGj"
                "重新安裝");
        ok = false;
    }

    if (_comppack_versions.base.empty() && !_comppack_versions.patch.empty())
        printError(
                "- 已安裝游戲更新兼容包, 但未安裝"
                "游戲本體兼容包, 請先安裝游戲本體兼容包, "
                "再安裝游戲更新兼容包");

    std::string comppack_version;
    if (!_comppack_versions.patch.empty())
        comppack_version = _comppack_versions.patch;
    else if (!_comppack_versions.base.empty())
        comppack_version = _comppack_versions.base;

    if (_item->presence == PresenceInstalled && !comppack_version.empty() &&
        comppack_version < _game_version)
        printError(
                "- 游戲版本與已安裝的兼容包版本"
                "不匹配. 如游戲版本已更新, 請"
                "安裝游戲更新兼容包");

    if (_item->presence == PresenceInstalled &&
        comppack_version > _game_version)
        printError(
                "- 游戲版本與已安裝的兼容包版本"
                "不匹配. 請刪除游戲更新文件恢復至初始版本,"
                "或通過桌面氣泡更新游戲版本");

    if (_item->presence != PresenceInstalled)
    {
        ImGui::Text("- 未安裝游戲");
        ok = false;
    }

    if (ok)
        ImGui::TextColored(Green, "已滿足運行條件");
}

void GameView::refresh()
{
    LOGF("refreshing gameview");
    _game_version = pkgi_get_game_version(_item->titleid);
    _comppack_versions = pkgi_get_comppack_versions(_item->titleid);
}

void GameView::start_download_package()
{
    if (_item->presence == PresenceInstalled)
    {
        LOGF("[{}] {} - already installed", _item->titleid, _item->name);
        pkgi_dialog_error("游戲已安裝");
        return;
    }

    pkgi_start_download(*_downloader, *_item);

    _item->presence = PresenceUnknown;
}

void GameView::cancel_download_package()
{
    _downloader->remove_from_queue(_item->content);
    _item->presence = PresenceUnknown;
}

void GameView::start_download_comppack(bool patch)
{
    const auto& entry = patch ? _patch_comppack : _base_comppack;

    _downloader->add(DownloadItem{CompPack,
                                  _item->name,
                                  _item->titleid,
                                  _config->comppack_url + entry->path,
                                  std::vector<uint8_t>{},
                                  std::vector<uint8_t>{},
                                  false,
                                  "ux0:",
                                  patch,
                                  entry->app_version});
}

void GameView::cancel_download_comppacks()
{
    // HACK: comppack are identified by their titleid instead of content id
    _downloader->remove_from_queue(_item->titleid);
}
