#include "ui/BeatmapPathsPanel.hpp"

BeatmapPathsPanel::BeatmapPathsPanel()
    : Gtk::Frame("Beatmap Paths"),
      input_file_label("Input beatmap file"),
      input_file_picker("Input beatmap file"),
      output_folder_label("Output folder"),
      output_folder_picker("Output folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER),
      sep1(Gtk::ORIENTATION_VERTICAL), sep2(Gtk::ORIENTATION_VERTICAL), sep3(Gtk::ORIENTATION_VERTICAL)
{
    set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
    set_name("main_frame");
    get_label_widget()->set_margin_start(10);
    get_label_widget()->set_margin_end(10);

    auto osu_file_filter = Gtk::FileFilter::create();
    osu_file_filter->set_name("osu! Beatmap Files (*.osu)");
    osu_file_filter->add_pattern("*.osu");
    input_file_picker.add_filter(osu_file_filter);

    grid.set_border_width(10);
    grid.set_row_spacing(10);
    grid.set_column_spacing(10);

    sep1.set_vexpand(true);
    sep2.set_vexpand(true);
    sep3.set_vexpand(true);

    grid.attach(sep1, 0, 0);
    grid.attach(input_file_label, 0, 1);
    grid.attach(input_file_picker, 1, 1);
    grid.attach(sep2, 0, 2);
    grid.attach(output_folder_label, 0, 3);
    grid.attach(output_folder_picker, 1, 3);
    grid.attach(sep3, 0, 4);

    input_file_picker.set_hexpand(true);
    output_folder_picker.set_hexpand(true);

    // --- SIGNALS ---

    input_file_picker.signal_file_set().connect([&](){ 
        std::filesystem::path beatmap_path = input_file_picker.get_filename();
        output_folder_picker.select_filename(beatmap_path.parent_path().string());

        signal_file_picked.emit();
    });

    add(grid);
    show_all_children();
}

void BeatmapPathsPanel::unselect_paths()
{
    input_file_picker.unselect_all();
    output_folder_picker.unselect_all();
}
