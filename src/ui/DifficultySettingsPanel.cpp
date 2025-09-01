#include "ui/DifficultySettingsPanel.hpp"

DifficultySettingsPanel::DifficultySettingsPanel()
    : Gtk::Frame("Difficulty settings"),
      difficulty_name_label("Difficulty\nname"),
      default_button("Default values"),
      hp_label("HP"),
      cs_label("CS"),
      od_label("OD"),
      ar_label("AR")
{
    set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);

    grid.set_border_width(10);
    grid.set_row_spacing(10);
    grid.set_column_spacing(10);

    difficulty_name_label.set_justify(Gtk::JUSTIFY_CENTER);

    hp_scale.set_range(0.0, 10.0);
    cs_scale.set_range(0.0, 10.0);
    od_scale.set_range(0.0, 10.0);
    ar_scale.set_range(0.0, 10.0);

    ar_scale.set_tooltip_text("Old maps have AR tied with OD");

    sep1.set_vexpand(true);
    sep2.set_vexpand(true);
    sep3.set_vexpand(true);
    sep4.set_vexpand(true);
    sep5.set_vexpand(true);
    sep6.set_vexpand(true);

    grid.attach(sep1, 0, 0);
    grid.attach(difficulty_name_label, 0, 1);
    grid.attach(difficulty_name_entry, 1, 1);
    grid.attach(default_button, 2, 1);
    grid.attach(sep2, 0, 2);
    grid.attach(hp_label, 0, 3);
    grid.attach(hp_scale, 1, 3, 2);
    grid.attach(sep3, 0, 4);
    grid.attach(cs_label, 0, 5);
    grid.attach(cs_scale, 1, 5, 2);
    grid.attach(sep4, 0, 6);
    grid.attach(od_label, 0, 7);
    grid.attach(od_scale, 1, 7, 2);
    grid.attach(sep5, 0, 8);
    grid.attach(ar_label, 0, 9);
    grid.attach(ar_scale, 1, 9, 2);
    grid.attach(sep6, 0, 10);

    difficulty_name_entry.set_hexpand(true);
    hp_scale.set_hexpand(true);
    cs_scale.set_hexpand(true);
    od_scale.set_hexpand(true);
    ar_scale.set_hexpand(true);

    // --- SIGNALS ---

    difficulty_name_entry.signal_changed().connect([&]() { 
        difficulty_name = difficulty_name_entry.get_text();
    });

    default_button.signal_pressed().connect([&](){ signal_default_button_pressed.emit(); });

    hp_scale.signal_value_changed().connect([&](){ difficulty_settings.hp = hp_scale.get_value(); });
    cs_scale.signal_value_changed().connect([&](){ difficulty_settings.cs = cs_scale.get_value(); });
    od_scale.signal_value_changed().connect([&](){ 
        difficulty_settings.od = od_scale.get_value();
        if (link_od_with_ar) ar_scale.set_value(od_scale.get_value());
    });
    ar_scale.signal_value_changed().connect([&](){ 
        difficulty_settings.ar = ar_scale.get_value();
    });

    add(grid);
    show_all_children();
}

void DifficultySettingsPanel::set_from_beatmap(const Beatmap &beatmap)
{
    hp_scale.set_value(static_cast<double>(beatmap.hp.value()));
    cs_scale.set_value(static_cast<double>(beatmap.cs.value()));
    od_scale.set_value(static_cast<double>(beatmap.od.value()));

    if (beatmap.ar.has_value()) {
        ar_scale.set_value(static_cast<double>(beatmap.ar.value()));
        link_od_with_ar = false;
        ar_scale.set_sensitive(true);
    } else {
        ar_scale.set_value(static_cast<double>(beatmap.od.value()));
        link_od_with_ar = true;
        ar_scale.set_sensitive(false);
    }

    difficulty_name_entry.set_text(beatmap.difficulty_name + " tag");
}
