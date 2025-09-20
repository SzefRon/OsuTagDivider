#pragma once

#include <cstdlib>

#include "util/ini.h"
#include "logic/Config.hpp"

class ConfigManager
{
private:
    Config config;
    std::vector<std::string> available_themes;

    std::filesystem::path data_path = ".";
    std::filesystem::path themes_path = ".";

    std::filesystem::path get_default_paths();
public:
    ConfigManager();

    Config get_config() { return config; }
    const std::vector<std::string> &get_themes() { return available_themes; }
    std::filesystem::path get_data_path() { return data_path; }
    std::filesystem::path get_themes_path() { return themes_path; }

    void change_theme(const std::string &theme_name);
};