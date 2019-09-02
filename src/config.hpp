#pragma once

#include "db.hpp"

#include <string>
#include <vector>

typedef struct Config
{
    DbSort sort;
    DbSortOrder order;
    uint32_t filter;
    int no_version_check;
    int install_psp_as_pbp;
    int repo;
    std::string install_psp_psx_location;
    bool psm_readme_disclaimer;

    std::vector<std::string> repo_list;

    std::string games_url;
    std::string dlcs_url;
    std::string demos_url;
    std::string themes_url;
    std::string psm_games_url;
    std::string psx_games_url;
    std::string psp_games_url;
    std::string psp_dlcs_url;

    std::string comppack_url;
} Config;

Config pkgi_load_config(int isRefresh);
void pkgi_save_config(const Config& config);
