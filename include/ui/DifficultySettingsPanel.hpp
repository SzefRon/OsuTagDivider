#pragma once

#include <gtkmm/frame.h>
#include <gtkmm/grid.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/scale.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <iostream>

#include "logic/DifficultySettings.hpp"

class DifficultySettingsPanel : public Gtk::Frame
{
private:
    Gtk::Grid grid;

    Gtk::Box sep1, sep2, sep3, sep4, sep5, sep6;

    Gtk::Label difficulty_name_label;
    Gtk::Entry difficulty_name_entry;

    Gtk::Button default_button;

    Gtk::Label hp_label;
    Gtk::Scale hp_scale;

    Gtk::Label cs_label;
    Gtk::Scale cs_scale;

    Gtk::Label od_label;
    Gtk::Scale od_scale;

    Gtk::Label ar_label;
    Gtk::Scale ar_scale;

    std::string difficulty_name;
    DifficultySettings difficulty_settings;

    bool link_od_with_ar = false;
public:
    DifficultySettingsPanel();

    void set_from_beatmap(const Beatmap &beatmap);
    std::string get_difficulty_name() { return difficulty_name; }
    DifficultySettings get_settings() { return difficulty_settings; }

    sigc::signal<void> signal_default_button_pressed;
};