#pragma once

#include <cstdlib>

#include "util/ini.h"
#include "logic/Config.hpp"

class ConfigManager
{
private:
    Config config;

    std::filesystem::path get_default_paths();
public:
    ConfigManager();

    Config get_config() { return config; }

    void change_theme(const std::string &theme_name);
};