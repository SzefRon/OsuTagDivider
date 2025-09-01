#pragma once

#include <filesystem>
#include <cstdlib>

#include <gtkmm/frame.h>
#include <gtkmm/Grid.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <gtkmm/filechooserbutton.h>

class BeatmapPathsPanel : public Gtk::Frame
{
private:
    Gtk::Grid grid;

    Gtk::Box sep1, sep2, sep3;

    Gtk::Label input_file_label;
    Gtk::FileChooserButton input_file_picker;

    Gtk::Label output_folder_label;
    Gtk::FileChooserButton output_folder_picker;

    void set_input_default_folder();
public:
    BeatmapPathsPanel();

    sigc::signal<void> signal_file_picked;

    std::string get_current_input_path() { return input_file_picker.get_filename(); }
    std::string get_output_folder() { return output_folder_picker.get_filename(); }
    void unselect_paths();
};