#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/grid.h>
#include <gtkmm/messagedialog.h>
#include <glibmm/dispatcher.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>
#include <gtkmm/settings.h>
#include <iostream>
#include <thread>

#include "ui/BeatmapPathsPanel.hpp"
#include "ui/DifficultySettingsPanel.hpp"
#include "ui/TagDivisionSettingsPanel.hpp"

#include "logic/ConfigManager.hpp"
#include "logic/Beatmap.hpp"
#include "logic/BeatmapProcessor.hpp"

class MainWindow : public Gtk::ApplicationWindow
{
private:
    ConfigManager config_manager;
    Glib::RefPtr<Gtk::CssProvider> css_provider;

    Gtk::Box box;

    BeatmapPathsPanel beatmap_paths_panel;
    DifficultySettingsPanel difficulty_settings_panel;
    TagDivisionSettingsPanel tag_division_settings_panel;

    Gtk::Grid bottom_grid;
    Gtk::Button generate_button;
    Gtk::Label theme_label;
    Gtk::ComboBoxText theme_combo;

    std::optional<Beatmap> current_beatmap;

    Glib::Dispatcher dispatcher_saving;
    Glib::Dispatcher dispatcher_finish;
    std::string generate_exit_message;

    void try_generate_and_save_beatmaps();
    void try_load_beatmap();

    void load_config_theme();
public:
    MainWindow();
    ~MainWindow() = default;
};