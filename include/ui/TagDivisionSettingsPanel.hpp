#pragma once

#include <gtkmm/frame.h>
#include <gtkmm/radiobuttongroup.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/box.h>

#include "logic/TagDivisionSettings.hpp"

class TagDivisionSettingsPanel : public Gtk::Frame
{
private:
    Gtk::Grid grid;

    Gtk::Box sep1, sep2, sep3, sep4, sep5, sep6;

    Gtk::Label player_amount_label;
    Gtk::SpinButton player_amount_spinner;

    Gtk::RadioButtonGroup tag_mode_radio_group;
    Gtk::Frame tag_mode_frame;
    Gtk::Box tag_mode_box;
    Gtk::RadioButton every_new_combo_radio_button;
    Gtk::RadioButton every_n_notes_radio_button;

    Gtk::Label n_label;
    Gtk::SpinButton n_spinner;

    TagDivisionSettings tds;
public:
    TagDivisionSettingsPanel();

    TagDivisionSettings get_settings() { return tds; }
};