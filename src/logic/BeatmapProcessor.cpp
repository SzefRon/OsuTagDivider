#include "logic/BeatmapProcessor.hpp"

void copy_section(std::string &str, std::vector<std::string> &section)
{
    std::stringstream wss;
    auto size = section.size();
    for (int i = 0; i < size; i++) {
        wss << section[i];
        if (i != size - 1) wss << '\n';
    }
    str = wss.str();
}

std::string try_parse_field(std::string field_name, std::optional<float> &val, std::string line)
{
    if (line.starts_with(field_name)) {
        try {
            val = std::stof(line.substr(line.find(':') + 1));
        }
        catch (...) {
            return "Invalid difficulty value";
        }
        return "";
    }
    return "";
}

size_t find_nth(const std::string &str, const wchar_t &ch, int n)
{
    size_t pos = -1LL;
    for (int i = 0; i < n; i++) {
        pos = str.find(ch, pos + 1);
        if (pos == std::string::npos) return std::string::npos;
    }
    return pos;
}

std::string BeatmapProcessor::load_beatmap(Beatmap &beatmap, std::filesystem::path path)
{
    std::ifstream file(path);

    if (!file.is_open()) return "Can't open input file";

    // osu file format
    std::string line;
    bool valid = false;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line.starts_with("osu file format")) {
            beatmap.osu_file_format = line;
            valid = true;
            break;
        }
    }

    if (!valid) return "Can't find file osu format";

    std::map<std::string, std::vector<std::string>> section_data;

    // sections
    std::string current_section;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // get section name
        if (line.front() == '[') {
            line.erase(std::remove_if(line.begin(), line.end(), iswspace), line.end());
            current_section = line;
            continue;
        }

        // put data in section
        section_data[current_section].push_back(line);
    }

    file.close();

    // Parse Metadata

    if (section_data.contains("[Metadata]")) {
        std::stringstream wss;
        auto &section = section_data["[Metadata]"];
        auto size = section.size();
        for (int i = 0; i < size; i++) {
            auto &line = section[i];
            if (line.starts_with("Version:")) beatmap.difficulty_name = line.substr(line.find(':') + 1);
            else if (line.starts_with("Artist:")) beatmap.artist_name = line.substr(line.find(':') + 1);
            else if (line.starts_with("Title:")) beatmap.title_name = line.substr(line.find(':') + 1);
            else if (line.starts_with("Creator:")) beatmap.creator_name = line.substr(line.find(':') + 1);
            else {
                wss << line;
                if (i != size - 1) wss << '\n';
            }
        }
        beatmap.metadata_data = wss.str();
    } else return "There's no [Metadata] section in the file";

    if (beatmap.difficulty_name.empty()) return "The [Metadata] section doesn't have a Version field";
    if (beatmap.artist_name.empty()) return "The [Metadata] section doesn't have an Artist field";
    if (beatmap.title_name.empty()) return "The [Metadata] section doesn't have a Title field";
    if (beatmap.creator_name.empty()) return "The [Metadata] section doesn't have an Creator field";

    // Parse difficulty settings

    if (section_data.contains("[Difficulty]")) {
        auto &section = section_data["[Difficulty]"];
        for (auto &line : section) {
            if (auto ret = try_parse_field("HPDrainRate:", beatmap.hp, line); !ret.empty()) return ret;
            else if (auto ret = try_parse_field("CircleSize:", beatmap.cs, line); !ret.empty()) return ret;
            else if (auto ret = try_parse_field("OverallDifficulty:", beatmap.od, line); !ret.empty()) return ret;
            else if (auto ret = try_parse_field("ApproachRate:", beatmap.ar, line); !ret.empty()) return ret;
            else if (auto ret = try_parse_field("SliderMultiplier:", beatmap.slider_multiplier, line); !ret.empty()) return ret;
            else if (auto ret = try_parse_field("SliderTickRate:", beatmap.slider_tick_rate, line); !ret.empty()) return ret;
        }
    } else return "There's no [Difficulty] section in the file";

    if (!(beatmap.hp && beatmap.cs && beatmap.od && beatmap.slider_multiplier && beatmap.slider_tick_rate))
        return "Invalid difficulty settings";

    // Parse hitobjects

    HitObject ho;
    if (section_data.contains("[HitObjects]")) {
        auto &section = section_data["[HitObjects]"];
        for (auto &line : section) {
            size_t start = find_nth(line, ',', 3);
            if (start == std::string::npos) return "Invalid HitObject";
            start += 1;

            size_t length = find_nth(line, ',', 4);
            if (length == std::string::npos) return "Invalid HitObject";
            length -= start;

            std::string str_ho_type = line.substr(start, length);
            int ho_type;

            try {
                ho_type = std::stoi(str_ho_type);
            }
            catch (...) {
                return "Invalid HitObject";
            }

            ho.hit_object_data = line;
            ho.is_new_combo = ho_type & (1 << 2);

            beatmap.hit_objects.push_back(ho);
        }
    }

    // copy paste sections

    if (section_data.contains("[General]")) {
        copy_section(beatmap.general_data, section_data["[General]"]);
    } else return "There's no [General] section in the file";

    if (section_data.contains("[Editor]")) {
        copy_section(beatmap.editor_data, section_data["[Editor]"]);
    }

    if (section_data.contains("[Events]")) {
        copy_section(beatmap.events_data, section_data["[Events]"]);
    }

    if (section_data.contains("[TimingPoints]")) {
        copy_section(beatmap.timing_points_data, section_data["[TimingPoints]"]);
    } else return "There's no [TimingPoints] section in the file";

    if (section_data.contains("[Colours]")) {
        copy_section(beatmap.colors_data, section_data["[Colours]"]);
    }

    return "";
}

std::string BeatmapProcessor::save_beatmap(const Beatmap &beatmap, std::filesystem::path path)
{
    std::ofstream file(path);

    if (!file.is_open()) return "Can't create output file at: " + path.string();

    file << beatmap.osu_file_format << "\n\n"

         << "[General]\n" << beatmap.general_data << "\n\n";

    if (!beatmap.editor_data.empty())
        file << "[Editor]\n" << beatmap.editor_data << "\n\n";

    file << "[Metadata]\n" << beatmap.metadata_data << '\n'
         << "Title:" << beatmap.title_name << '\n'
         << "Artist:" << beatmap.artist_name << '\n'
         << "Creator:" << beatmap.creator_name << '\n'
         << "Version:" << beatmap.difficulty_name << "\n\n"

         << "[Difficulty]\n" << std::fixed << std::setprecision(1)
         << "HPDrainRate:" << beatmap.hp.value() << '\n'
         << "CircleSize:" << beatmap.cs.value() << '\n'
         << "OverallDifficulty:" << beatmap.od.value() << '\n';
        
    if (beatmap.ar.has_value())
        file << "ApproachRate:" << beatmap.ar.value() << '\n';

    file << "SliderMultiplier:" << beatmap.slider_multiplier.value() << '\n'
         << "SliderTickRate:" << beatmap.slider_tick_rate.value() << "\n\n"
    
         << "[Events]\n" << beatmap.events_data << "\n\n"

         << "[TimingPoints]\n" << beatmap.timing_points_data << "\n\n";

    if (!beatmap.colors_data.empty())
        file << "[Colours]\n" << beatmap.colors_data << "\n\n";

    file << "[HitObjects]\n";
    for (auto &tp : beatmap.hit_objects) {
        file << tp.hit_object_data << '\n';
    }

    file.close();

    return "";
}

std::string BeatmapProcessor::batch_save(const Beatmap &beatmap_source, const std::vector<std::shared_ptr<Beatmap>> beatmaps, std::filesystem::path folder_path)
{
    std::string filename_base = beatmap_source.artist_name + " - " + beatmap_source.title_name
                                + " (" + beatmap_source.creator_name + ") [";

    for (auto &beatmap : beatmaps) {
        std::string filename = filename_base + beatmap->difficulty_name + "].osu";
        std::filesystem::path path = folder_path / filename;
        auto ret = save_beatmap(*beatmap, path);
        if (!ret.empty()) return ret;
    }

    return "";
}

std::vector<std::shared_ptr<Beatmap>> BeatmapProcessor::divide_for_tag(const Beatmap &beatmap, const std::string &diff_name, const DifficultySettings &ds, const TagDivisionSettings &tds)
{
    std::vector<std::shared_ptr<Beatmap>> tag_beatmaps;
    tag_beatmaps.resize(tds.player_amount);

    // transfer data except hitobjects
    for (uint16_t i = 0; i < tds.player_amount; i++) {
        auto &tag_beatmap = tag_beatmaps.at(i);
        tag_beatmap = std::make_shared<Beatmap>();

        tag_beatmap->osu_file_format = beatmap.osu_file_format;
        tag_beatmap->general_data = beatmap.general_data;
        tag_beatmap->editor_data = beatmap.editor_data;

        tag_beatmap->metadata_data = beatmap.metadata_data;
        tag_beatmap->difficulty_name = diff_name + ' ' + std::to_string(i + 1);
        tag_beatmap->artist_name = beatmap.artist_name;
        tag_beatmap->title_name = beatmap.title_name;
        tag_beatmap->creator_name = beatmap.creator_name;
        
        tag_beatmap->hp = ds.hp;
        tag_beatmap->cs = ds.cs;
        tag_beatmap->od = ds.od;
        tag_beatmap->ar = ds.ar;

        tag_beatmap->slider_multiplier = beatmap.slider_multiplier;
        tag_beatmap->slider_tick_rate = beatmap.slider_tick_rate;

        tag_beatmap->events_data = beatmap.events_data;
        tag_beatmap->timing_points_data = beatmap.timing_points_data;
        tag_beatmap->colors_data = beatmap.colors_data;
    }

    switch (tds.tag_divide_mode)
    {
        case TagDivideMode::EVERY_NEW_COMBO: {
            uint16_t cur_player = 0;
            bool first = true;
            for (auto &hit_object : beatmap.hit_objects) {
                if (hit_object.is_new_combo && !first) cur_player = (cur_player + 1) % tds.player_amount;
                tag_beatmaps.at(cur_player)->hit_objects.push_back(hit_object);
                first = false;
            }
            break;
        }
        case TagDivideMode::EVERY_N_NOTES: {
            uint16_t cur_player = 0;
            uint16_t i = 0;
            for (auto &hit_object : beatmap.hit_objects) {
                tag_beatmaps.at(cur_player)->hit_objects.push_back(hit_object);
                i++;
                if (i == tds.N) {
                    cur_player = (cur_player + 1) % tds.player_amount;
                    i = 0;
                }
            }
            break;
        }
    }

    return tag_beatmaps;
}