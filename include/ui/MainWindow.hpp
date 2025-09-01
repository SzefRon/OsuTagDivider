#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/messagedialog.h>
#include <glibmm/dispatcher.h>
#include <iostream>
#include <thread>

#include "ui/BeatmapPathsPanel.hpp"
#include "ui/DifficultySettingsPanel.hpp"
#include "ui/TagDivisionSettingsPanel.hpp"

#include "logic/Beatmap.hpp"
#include "logic/BeatmapProcessor.hpp"

class MainWindow : public Gtk::ApplicationWindow
{
private:
    Gtk::Box box;

    BeatmapPathsPanel beatmap_paths_panel;
    DifficultySettingsPanel difficulty_settings_panel;
    TagDivisionSettingsPanel tag_division_settings_panel;

    Gtk::Button generate_button;

    std::optional<Beatmap> current_beatmap;

    Glib::Dispatcher dispatcher_saving;
    Glib::Dispatcher dispatcher_finish;
    std::string generate_exit_message;

    void try_generate_and_save_beatmaps();
    void try_load_beatmap();
public:
    MainWindow();
    ~MainWindow() = default;
};