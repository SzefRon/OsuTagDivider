#include <gtest/gtest.h>
#include "logic/BeatmapProcessor.hpp"

TEST(BeatmapFileManagerTest, LoadBeatmapTest)
{
    Beatmap bm;
    std::filesystem::path path = "test_data/oldmap.osu";
    auto result = BeatmapProcessor::load_beatmap(bm, path);
    EXPECT_EQ(result, "");
}

TEST(BeatmapFileManagerTest, LoadBrokenBeatmapTest)
{
    Beatmap bm;
    std::filesystem::path path = "test_data/broken_oldmap.osu";
    auto result = BeatmapProcessor::load_beatmap(bm, path);
    EXPECT_NE(result, "");
}

TEST(BeatmapFileManagerTest, SaveBeatmapTest)
{
    Beatmap bm;
    std::filesystem::path input_path = "test_data/oldmap.osu";
    auto result = BeatmapProcessor::load_beatmap(bm, input_path);
    EXPECT_EQ(result, "");

    std::filesystem::path output_path = "test_data/oldmapRESAVE.osu";
    result = BeatmapProcessor::save_beatmap(bm, output_path);
    EXPECT_EQ(result, "");
}

TEST(BeatmapFileManagerTest, DivideAndSaveBeatmapTest)
{
    Beatmap bm;
    std::filesystem::path input_path = "test_data/oldmap.osu";
    std::filesystem::path output_folder = "test_data";
    auto result = BeatmapProcessor::load_beatmap(bm, input_path);
    EXPECT_EQ(result, "");

    auto tag_beatmaps = BeatmapProcessor::divide_for_tag(
        bm, "test", DifficultySettings::from_beatmap(bm),
        {2, TagDivideMode::EVERY_NEW_COMBO}
    );

    EXPECT_FALSE(tag_beatmaps.empty());

    BeatmapProcessor::batch_save(bm, tag_beatmaps, output_folder);
}