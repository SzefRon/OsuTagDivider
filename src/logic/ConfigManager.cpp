#include "logic/ConfigManager.hpp"

std::filesystem::path ConfigManager::get_default_paths()
{
    std::filesystem::path default_folder;
    #ifdef _WIN32
        auto local_path = std::getenv("localappdata");
        if (local_path) {
            default_folder = local_path;
            default_folder /= "osu!";
            default_folder /= "Songs";
        }
    #endif
    #ifdef __linux__
        auto home_path = std::getenv("HOME");
        if (home_path) {
            default_folder = home_path;
            default_folder /= ".local";
            default_folder /= "share";
        }
    #endif
    return default_folder;
}

ConfigManager::ConfigManager()
{
    if (!std::filesystem::exists("data")) return;

    mINI::INIFile file("data/config.ini");
    mINI::INIStructure data;
    file.read(data);

    if (!data.has("config")) return;
    auto &config_sec = data["config"];

    if (config_sec.has("selected_theme")) {
        std::filesystem::path theme_path = "data";
        theme_path /= config_sec["selected_theme"] + ".css";

        if (std::filesystem::exists(theme_path))
            config.selected_theme = config_sec["selected_theme"];
    }

    if (config_sec.has("default_path")) {
        if (config_sec["default_path"] == "-") {
            config.default_path = get_default_paths();
            config_sec["default_path"] = config.default_path.string();
        }
        else config.default_path = config_sec["default_path"];
    }

    bool ret = file.generate(data);

    for (const auto& entry : std::filesystem::directory_iterator("data")) {
        if (entry.is_regular_file() && entry.path().extension() == ".css") {
            auto test = entry.path().filename().replace_extension("");
            available_themes.push_back(test.string());
        }
    }
}

void ConfigManager::change_theme(const std::string &theme_name)
{
    mINI::INIFile file("data/config.ini");
    mINI::INIStructure data;
    file.read(data);

    config.selected_theme = theme_name;
    data["config"]["selected_theme"] = theme_name;

    bool ret = file.generate(data);
}
