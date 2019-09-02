#include "config.hpp"

#include <fmt/format.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "file.hpp"
#include "pkgi.hpp"

static const std::string PSV_GAMES_POSTFIX = "PSV_GAMES_SC.tsv";
static const std::string PSV_DLCS_POSTFIX = "PSV_DLCS.tsv";
static const std::string PSX_GAMES_POSTFIX = "PSX_GAMES.tsv";
static const std::string PSP_DLCS_POSTFIX = "PSP_DLCS.tsv";
static const std::string PSP_GAMES_POSTFIX = "PSP_GAMES.tsv";
static const std::string PSM_GAMES_POSTFIX = "PSM_GAMES.tsv";
static const std::string PSV_DEMOS_POSTFIX = "PSV_DEMOS.tsv";
static const std::string PSV_THEMES_POSTFIX = "PSV_THEMES.tsv";
static constexpr char default_comppack_url[] = {
        0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x70, 0x72, 0x6f, 0x78,
        0x79, 0x2e, 0x6e, 0x6f, 0x70, 0x61, 0x79, 0x73, 0x74, 0x61, 0x74,
        0x69, 0x6f, 0x6e, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x68, 0x74, 0x74,
        0x70, 0x73, 0x3a, 0x2f, 0x2f, 0x67, 0x69, 0x74, 0x6c, 0x61, 0x62,
        0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x6e, 0x6f, 0x70, 0x61, 0x79, 0x73,
        0x74, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x5f, 0x72, 0x65, 0x70, 0x6f,
        0x73, 0x2f, 0x6e, 0x70, 0x73, 0x5f, 0x63, 0x6f, 0x6d, 0x70, 0x61,
        0x74, 0x69, 0x5f, 0x70, 0x61, 0x63, 0x6b, 0x73, 0x2f, 0x72, 0x61,
        0x77, 0x2f, 0x6d, 0x61, 0x73, 0x74, 0x65, 0x72, 0x2f, 0x00};



static DbSort parse_sort(const char* value, DbSort sort)
{
    if (pkgi_stricmp(value, "title") == 0)
    {
        return SortByTitle;
    }
    else if (pkgi_stricmp(value, "region") == 0)
    {
        return SortByRegion;
    }
    else if (pkgi_stricmp(value, "name") == 0)
    {
        return SortByName;
    }
    else if (pkgi_stricmp(value, "size") == 0)
    {
        return SortBySize;
    }
    else if (pkgi_stricmp(value, "date") == 0)
    {
        return SortByDate;
    }
    else
    {
        return sort;
    }
}

static DbSortOrder parse_order(const char* value, DbSortOrder order)
{
    if (pkgi_stricmp(value, "asc") == 0)
    {
        return SortAscending;
    }
    else if (pkgi_stricmp(value, "desc") == 0)
    {
        return SortDescending;
    }
    else
    {
        return order;
    }
}

static DbFilter parse_filter(const char* cvalue, uint32_t filter)
{
    uint32_t result = 0;
    int len=0;
    for(;cvalue[len]!=0;len++);
    char * value = (char *)malloc(sizeof(char)*(len+1));
    char * pvalue = value;
    for(int i=0;i<=len;i++){
        value[i] = cvalue[i];
    }
    char* start = value;
    for (;;)
    {
        char ch = *value;
        if (ch == 0 || ch == ',')
        {
            *value = 0;
            if (pkgi_stricmp(start, "ASA") == 0)
            {
                result |= DbFilterRegionASA;
            }
            else if (pkgi_stricmp(start, "EUR") == 0)
            {
                result |= DbFilterRegionEUR;
            }
            else if (pkgi_stricmp(start, "JPN") == 0)
            {
                result |= DbFilterRegionJPN;
            }
            else if (pkgi_stricmp(start, "USA") == 0)
            {
                result |= DbFilterRegionUSA;
            }
            else
            {
                return static_cast<DbFilter>(filter);
            }
            if (ch == 0)
            {
                break;
            }
            value++;
            start = value;
        }
        else
        {
            value++;
        }
    }
    free(pvalue);
    return static_cast<DbFilter>(result);
}


void repo_to_address(Config &config, int isRefresh){
    if(isRefresh){
        config.repo_list = std::vector<std::string>();
        //GitHub 中文源
        config.repo_list.push_back("https://github.com/Anarch13/NPS_Chinese_Version/raw/master/");
        //NPS官方英文源
        config.repo_list.push_back("http://beta.nopaystation.com/tsv/");
        //即将废弃
        config.repo_list.push_back("http://47.100.37.250/tsvfiles/");
        config.repo = 0;
    }
    const std::string repo_address = config.repo_list[config.repo];
    config.games_url = repo_address + PSV_GAMES_POSTFIX;
    config.dlcs_url = repo_address + PSV_DLCS_POSTFIX;
    config.demos_url = repo_address + PSV_DEMOS_POSTFIX;
    config.themes_url = repo_address + PSV_THEMES_POSTFIX;
    config.psm_games_url = repo_address + PSM_GAMES_POSTFIX;
    config.psx_games_url = repo_address + PSX_GAMES_POSTFIX;
    config.psp_games_url = repo_address + PSP_GAMES_POSTFIX;
    config.psp_dlcs_url = repo_address + PSP_DLCS_POSTFIX;
}


Config pkgi_load_config(int isRefresh)
{
    try
    {
        Config config{};

        config.repo = 0;
        config.repo_list = std::vector<std::string>();
        config.sort = SortByName;
        config.order = SortAscending;
        config.filter = DbFilterAll;
        config.install_psp_psx_location = "ux0:";
        config.comppack_url = default_comppack_url;
        if(isRefresh){
            repo_to_address(config,1);
            pkgi_save_config(config);
            return config;
        }
        //IO:读取Config
        auto const path =
                fmt::format("{}/config_cn.json", pkgi_get_config_folder());
        LOGF("config location: {}", path);

        if (!pkgi_file_exists(path))
        {
            repo_to_address(config,1);
            pkgi_save_config(config);
            return config;
        }
        auto data = pkgi_load(path);
        data.push_back('\0');
        //JSON:解析数据
        std::string config_text(data.begin(), data.end());
        rapidjson::StringStream config_json(config_text.c_str());

        rapidjson::Document json_data;
        json_data.ParseStream(config_json);
        if(!json_data.IsObject()){
            throw formatEx<std::runtime_error>(
                "加载配置失败:\n{}", "解析失败");
        }
        if(json_data.HasMember("sort")&&json_data["sort"].IsString()){
            config.sort = parse_sort(json_data["sort"].GetString(), SortByName);
        }
        if(json_data.HasMember("order")&&json_data["order"].IsString()){
            config.order = parse_order(json_data["order"].GetString(), SortAscending);
        }
        if(json_data.HasMember("filter")&&json_data["filter"].IsString()){
            config.filter = parse_filter(json_data["filter"].GetString(), DbFilterAll);
        }
        if(json_data.HasMember("auto_update")&&json_data["auto_update"].IsBool()){
            config.no_version_check = !json_data["filter"].GetBool();
        }
        if(json_data.HasMember("install_psp_as_pbp")&&json_data["install_psp_as_pbp"].IsBool()){
            config.install_psp_as_pbp = json_data["install_psp_as_pbp"].GetBool();
        }
        if(json_data.HasMember("install_psp_psx_location")&&json_data["install_psp_psx_location"].IsString()){
            config.install_psp_psx_location = json_data["install_psp_psx_location"].GetString();
        }
        if(json_data.HasMember("enablePSM")&&json_data["enablePSM"].IsString()){
            config.psm_readme_disclaimer = json_data["enablePSM"].GetBool();
        }
        if(json_data.HasMember("repoID")&&json_data["repoID"].IsInt()){
            config.repo = json_data["repoID"].GetInt();
        }
        if(json_data.HasMember("repoList")&&json_data["repoList"].IsArray()){
            rapidjson::Value list = json_data["repoList"].GetArray();
            rapidjson::Value temp;
            for (rapidjson::SizeType i=0;i<list.Size();i++){
                temp = list[i];
                if(temp.IsString()){
                    config.repo_list.push_back(temp.GetString());
                }
            }
        }
        
        if(json_data.HasMember("url_comppack")&&json_data["url_comppack"].IsString()){
            config.comppack_url = json_data["url_comppack"].GetString();
        }
        //读取JSON结束
        if(config.repo_list.size() == 0){
            //内置地址
            config.repo_list.push_back("http://47.100.37.250");
            config.repo_list.push_back("https://github.com");
        }
        //若超出范围则直接置0
        if(config.repo>=config.repo_list.size()){
            config.repo = 0;
        }
        //repo转真实地址
        repo_to_address(config,0);
        return config;
    }
    catch (const std::exception& e)
    {
        throw formatEx<std::runtime_error>(
                "加载配置失败:\n{}", e.what());
    }
}

static const char* sort_str(DbSort sort)
{
    switch (sort)
    {
    case SortByTitle:
        return "title";
    case SortByRegion:
        return "region";
    case SortByName:
        return "name";
    case SortBySize:
        return "size";
    case SortByDate:
        return "date";
    }
    return "";
}

static const char* order_str(DbSortOrder order)
{
    switch (order)
    {
    case SortAscending:
        return "asc";
    case SortDescending:
        return "desc";
    }
    return "";
}

static const std::string filter_str(uint32_t filter){
    std::string tmp = "";
    bool isNew = true;
    if (filter & DbFilterRegionASA)
    {
        if(isNew){isNew=false;}
        else{tmp+=",";}
        tmp += "ASA";
    }
    if (filter & DbFilterRegionEUR)
    {
        if(isNew){isNew=false;}
        else{tmp+=",";}
        tmp += "EUR";
    }
    if (filter & DbFilterRegionJPN)
    {
        if(isNew){isNew=false;}
        else{tmp+=",";}
        tmp += "JPN";
    }
    if (filter & DbFilterRegionUSA)
    {
        if(isNew){isNew=false;}
        else{tmp+=",";}
        tmp += "USA";
    }
    return tmp;
}

int pkgi_strlen(const char * str){
    int len;
    for(len = 0;str[len]!=0;len++);
    return len;
}

void pkgi_save_config(const Config& config)
{
    
    
    rapidjson::StringBuffer buff;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buff);
    writer.StartObject();
    writer.Key("sort");
    writer.String(sort_str(config.sort));
    writer.Key("order");
    writer.String(order_str(config.order));
    writer.Key("filter");
    writer.String(filter_str(config.filter).c_str());
    writer.Key("auto_update");
    writer.Bool(!config.no_version_check);
    writer.Key("install_psp_as_pbp");
    writer.Bool(config.install_psp_as_pbp);
    writer.Key("install_psp_psx_location");
    writer.String(config.install_psp_psx_location.c_str());
    writer.Key("enablePSM");
    writer.Bool(config.psm_readme_disclaimer);
    writer.Key("repoID");
    writer.Int(config.repo);
    writer.Key("url_comppack");
    writer.String(config.comppack_url.c_str());
    writer.Key("repoList");
    writer.StartArray();
    for(int i=0;i<config.repo_list.size();i++){
        writer.String(config.repo_list[i].c_str());
    }
    writer.EndArray();
    writer.EndObject();
    pkgi_save(
            fmt::format("{}/config_cn.json", pkgi_get_config_folder()), buff.GetString(), pkgi_strlen(buff.GetString()));
    
}