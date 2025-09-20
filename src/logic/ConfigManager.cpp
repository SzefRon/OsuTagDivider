#include "logic/ConfigManager.hpp"

std::filesystem::path ConfigManager::get_default_paths()
{
    std::filesystem::path default_folder;
    #ifdef _WIN32
        auto local_path = std::getenv("localappdata");
        if (local_path) {
            default_folder = local_path;
            default_folder = default_folder / "osu!" / "Songs";
        }
        else return ".";
    #endif
    #ifdef __linux__
        auto home_path = std::getenv("HOME");
        if (home_path) {
            default_folder = home_path;
            default_folder = default_folder / ".local" / "share";
        }
        else return ".";
    #endif
    return default_folder;
}

ConfigManager::ConfigManager()
{
    #ifdef _WIN32
        data_path = "data";
    #endif
    #ifdef __linux__
        auto home = std::getenv("HOME");
        if (home) {
            data_path = home;
            data_path = data_path / ".local" / "share" / "OsuTagDivider" / "data";
            if (!std::filesystem::exists(data_path)) {
                std::filesystem::create_directories(data_path);
                std::filesystem::path copy_data_path;
                auto appdir = std::getenv("APPDIR");
                if (appdir) {
                    copy_data_path = appdir;
                    copy_data_path = copy_data_path / "usr" / "share" / "OsuTagDivider" / "data";
                }
                else {
                    copy_data_path = "./data";
                }
                std::filesystem::copy(copy_data_path, data_path, std::filesystem::copy_options::recursive);
            }
        }
    #endif

    if (!std::filesystem::exists(data_path)) return;
    themes_path = data_path / "themes" / "color";

    mINI::INIFile file((data_path / "config.ini").string());
    mINI::INIStructure data;
    file.read(data);

    if (!data.has("config")) return;
    auto &config_sec = data["config"];

    if (config_sec.has("selected_theme")) {
        std::filesystem::path theme_path = themes_path;
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

    for (const auto& entry : std::filesystem::directory_iterator(themes_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".css") {
            auto test = entry.path().filename().replace_extension("");
            available_themes.push_back(test.string());
        }
    }
}

void ConfigManager::change_theme(const std::string &theme_name)
{
    mINI::INIFile file((data_path / "config.ini").string());
    mINI::INIStructure data;
    file.read(data);

    config.selected_theme = theme_name;
    data["config"]["selected_theme"] = theme_name;

    bool ret = file.generate(data);
}
