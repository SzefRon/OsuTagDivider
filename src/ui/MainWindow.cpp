#include "ui/MainWindow.hpp"

void MainWindow::try_generate_and_save_beatmaps()
{
    if (current_beatmap.has_value()) {
        auto tag_beatmaps = BeatmapProcessor::divide_for_tag(
            current_beatmap.value(), difficulty_settings_panel.get_difficulty_name(),
            difficulty_settings_panel.get_settings(), tag_division_settings_panel.get_settings()
        );

        auto ret = BeatmapProcessor::batch_save(
            current_beatmap.value(), tag_beatmaps, beatmap_paths_panel.get_output_folder()
        );

        if (ret.empty()) {
            Gtk::MessageDialog dialog(*this, "Success!", false, Gtk::MESSAGE_INFO);
            dialog.run();
        }
        else {
            Gtk::MessageDialog dialog(*this, ret, false, Gtk::MESSAGE_ERROR);
            dialog.run();
        }
    }
    else {
        Gtk::MessageDialog dialog(*this, "No beatmap loaded!", false, Gtk::MESSAGE_ERROR);
        dialog.run();
    }
}

void MainWindow::try_load_beatmap()
{
    current_beatmap = Beatmap();
    auto err = BeatmapProcessor::load_beatmap(current_beatmap.value(),
        beatmap_paths_panel.get_current_input_path());

    if (err.empty()) {
        difficulty_settings_panel.set_from_beatmap(current_beatmap.value());
    }
    else {
        beatmap_paths_panel.unselect_paths();
        current_beatmap.reset();
        Gtk::MessageDialog dialog(*this, err, false, Gtk::MESSAGE_ERROR);
        dialog.run();
    }
}

MainWindow::MainWindow()
    : box(Gtk::ORIENTATION_VERTICAL, 10),
      generate_button("The OwO button that generates the maps")
{
    set_default_size(600, 600);
    // set_resizable(false);
    set_border_width(10);

    box.set_homogeneous(false);
    
    box.pack_start(beatmap_paths_panel);
    box.pack_start(difficulty_settings_panel);
    box.pack_start(tag_division_settings_panel);
    
    generate_button.set_size_request(-1, 70);
    box.pack_start(generate_button, Gtk::PACK_SHRINK);

    // --- SIGNALS ---

    beatmap_paths_panel.signal_file_picked.connect([&](){ try_load_beatmap(); });

    difficulty_settings_panel.signal_default_button_pressed.connect([&](){
        if (current_beatmap.has_value())
            difficulty_settings_panel.set_from_beatmap(current_beatmap.value());
    });

    generate_button.signal_clicked().connect([&](){ try_generate_and_save_beatmaps(); });

    add(box);
    show_all_children();
}
