#include "ui/MainWindow.hpp"

void MainWindow::try_generate_and_save_beatmaps()
{
    this->set_sensitive(false);
    generate_button.set_label("Generating beatmaps...");
    std::thread thr([&](){
        auto tag_beatmaps = BeatmapProcessor::divide_for_tag(
            current_beatmap.value(), difficulty_settings_panel.get_difficulty_name(),
            difficulty_settings_panel.get_settings(), tag_division_settings_panel.get_settings()
        );
        dispatcher_saving.emit();
        auto ret = BeatmapProcessor::batch_save(
            current_beatmap.value(), tag_beatmaps, beatmap_paths_panel.get_output_folder()
        );
        dispatcher_finish.emit();
    });

    thr.detach();
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
    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_path("data/style.css");

    Gtk::StyleContext::add_provider_for_screen(
        Gdk::Screen::get_default(),
        css_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    generate_button.set_name("generate_button");

    set_default_size(600, 600);
    // set_resizable(false);
    set_border_width(10);
    set_title("Osu Tag Divider");

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

    generate_button.signal_clicked().connect([&](){
        if (current_beatmap.has_value()) {
            auto player_amount = tag_division_settings_panel.get_settings().player_amount;
            if (player_amount >= 15) {
                Gtk::MessageDialog dialog(*this, "For each player the tool will create a separate file.\nAre you sure you want to generate " + std::to_string(player_amount) + " beatmaps?", 
                    false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
                auto response = dialog.run();
                if (response == Gtk::RESPONSE_OK) try_generate_and_save_beatmaps();
            }
            else
                try_generate_and_save_beatmaps();
        }
    });

    dispatcher_saving.connect([&](){
        generate_button.set_label("Saving beatmaps...");
    });
    dispatcher_finish.connect([&](){
        this->set_sensitive(true);
        generate_button.set_label("The OwO button that generates the maps");
        if (generate_exit_message.empty()) {
            Gtk::MessageDialog dialog(*this, "Success!", false, Gtk::MESSAGE_INFO);
            dialog.run();
        }
        else {
            Gtk::MessageDialog dialog(*this, generate_exit_message, false, Gtk::MESSAGE_ERROR);
            dialog.run();
        }
    });

    add(box);
    show_all_children();
}
