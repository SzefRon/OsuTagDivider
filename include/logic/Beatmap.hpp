#pragma once

#include <string>
#include <optional>
#include <vector>
#include <map>

#include "logic/HitObject.hpp"

struct Beatmap
{
    std::string osu_file_format;
    std::string general_data;
    std::string editor_data;

    // Metadata
    std::string metadata_data;
    std::string artist_name;
    std::string title_name;
    std::string creator_name;
    std::string difficulty_name;

    // Difficulty
    std::optional<float> hp, cs, od, ar, slider_multiplier, slider_tick_rate;

    std::string events_data;
    std::string timing_points_data;
    std::string colors_data;

    std::vector<HitObject> hit_objects;
};