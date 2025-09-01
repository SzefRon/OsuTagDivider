#pragma once

#include <stdint.h>

#include "logic/TagDivideMode.hpp"

struct TagDivisionSettings
{
    uint16_t player_amount = 2;
    TagDivideMode tag_divide_mode = TagDivideMode::EVERY_NEW_COMBO;
    uint16_t N = 1;

    TagDivisionSettings() = default;
    TagDivisionSettings(uint16_t player_amount, TagDivideMode tag_divide_mode,
                        uint16_t N = 0)
        : player_amount(player_amount), tag_divide_mode(tag_divide_mode), N(N) {}
};