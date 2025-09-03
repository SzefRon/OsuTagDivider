#pragma once

#include <string>
#include <filesystem>

struct Config
{
    std::string selected_theme = "default";
    std::filesystem::path default_path = "-";
};