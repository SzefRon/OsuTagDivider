#pragma once

#include <optional>

#include "logic/Beatmap.hpp"

struct DifficultySettings
{
    std::optional<float> hp = 0.0f, cs = 0.0f, od = 0.0f, ar;

    DifficultySettings() = default;
    DifficultySettings(std::optional<float> hp, std::optional<float> cs,
                       std::optional<float> od, std::optional<float> ar)
        : hp(hp), cs(cs), od(od), ar(ar) {}

    inline static DifficultySettings from_beatmap(const Beatmap &bm)
    {
        return DifficultySettings(bm.hp, bm.cs, bm.od, bm.ar);
    }
};