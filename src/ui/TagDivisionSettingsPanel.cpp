#include "ui/TagDivisionSettingsPanel.hpp"

TagDivisionSettingsPanel::TagDivisionSettingsPanel()
    : Gtk::Frame("Tag division settings"),
      player_amount_label("Number of players"),
      every_new_combo_radio_button(tag_mode_radio_group, "Every new combo"),
      every_n_notes_radio_button(tag_mode_radio_group, "Every N notes"),
      tag_mode_frame("Division mode"),
      tag_mode_box(Gtk::ORIENTATION_VERTICAL),
      n_label("N amount")
{
    set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
    set_name("main_frame");
    get_label_widget()->set_margin_start(10);
    get_label_widget()->set_margin_end(10);

    grid.set_border_width(10);
    grid.set_row_spacing(10);
    grid.set_column_spacing(10);

    player_amount_spinner.set_range(2.0, (1 << 16) - 1);
    player_amount_spinner.set_digits(0);
    player_amount_spinner.set_increments(1.0, 1.0);

    tag_mode_box.pack_start(every_new_combo_radio_button, Gtk::PACK_EXPAND_PADDING);
    tag_mode_box.pack_end(every_n_notes_radio_button, Gtk::PACK_EXPAND_PADDING);
    tag_mode_box.set_border_width(10);
    tag_mode_frame.add(tag_mode_box);
    tag_mode_frame.set_label_align(Gtk::ALIGN_CENTER);
    tag_mode_frame.get_label_widget()->set_margin_start(5);
    tag_mode_frame.get_label_widget()->set_margin_end(5);

    n_spinner.set_range(1.0, (1 << 16) - 1);
    n_spinner.set_digits(0);
    n_spinner.set_increments(1.0, 1.0);
    n_spinner.set_sensitive(false);

    sep1.set_vexpand(true);
    sep2.set_vexpand(true);
    sep3.set_hexpand(true);
    sep4.set_hexpand(true);
    sep5.set_hexpand(true);
    sep6.set_hexpand(true);

    grid.attach(sep1, 1, 0);
    grid.attach(sep2, 1, 3);
    grid.attach(sep3, 0, 1);
    grid.attach(player_amount_label, 1, 1);
    grid.attach(player_amount_spinner, 1, 2);
    grid.attach(sep4, 2, 1);
    grid.attach(tag_mode_frame, 3, 1, 1, 2);
    grid.attach(sep5, 4, 1);
    grid.attach(n_label, 5, 1);
    grid.attach(n_spinner, 5, 2);
    grid.attach(sep6, 6, 1);

    player_amount_spinner.set_hexpand(true);
    n_spinner.set_hexpand(true);

    // --- SIGNALS ---

    player_amount_spinner.signal_value_changed().connect([&](){
        tds.player_amount = player_amount_spinner.get_value_as_int();
    });

    every_new_combo_radio_button.signal_pressed().connect([&](){
        tds.tag_divide_mode = TagDivideMode::EVERY_NEW_COMBO;
    });

    every_n_notes_radio_button.signal_pressed().connect([&](){
        tds.tag_divide_mode = TagDivideMode::EVERY_N_NOTES;
    });

    every_n_notes_radio_button.signal_toggled().connect([&](){
        bool state = every_n_notes_radio_button.get_active();
        n_spinner.set_sensitive(state);
        if (state) n_label.set_name("label_active");
        else n_label.set_name("label_inactive");
    });

    n_spinner.signal_value_changed().connect([&](){ 
        tds.N = n_spinner.get_value_as_int();
    });

    player_amount_spinner.set_value(2.0);
    every_new_combo_radio_button.set_active(true);
    n_label.set_name("label_inactive");
    n_spinner.set_value(1.0);

    add(grid);
    show_all_children();
}