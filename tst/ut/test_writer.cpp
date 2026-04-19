// -------------------------------------------------------------
//
//!\file test_writer.cpp
//!\brief Tests for Polyconf writer behaviour
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// shocktest
#include "shocktest/shocktest.hpp"

// inbuilt
#include <cstdlib>
#include <filesystem>
#include <string>

// internal
#include "utils.hpp"

// SUT
#include "polyconf.hpp"

SHOCKTEST_GOODWEATHER(save_string_writes_basic_acf)
{
    const char* input =
        ".account\n"
        "name = colin\n"
        "developer = true\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);
    std::string output = POLYCONF::save_string(config, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(string_matches_ref(output, "ref/acf/auto_detect_basic.acf"), true);
}

SHOCKTEST_GOODWEATHER(save_string_round_trips_acf)
{
    const char* input =
        ".account\n"
        "name = colin\n"
        "\n"
        ".account\n"
        "name = user\n"
        ".account.info\n"
        "is_active = true\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);
    std::string output = POLYCONF::save_string(config, POLYCONF::FORMAT::ACF);
    POLYCONF::CONFIG reloaded = POLYCONF::load_string(output, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(reloaded.get_string("account.name", ""), "colin");
    EXPECT_EQ(reloaded.get_string("account[1].name", ""), "user");
    EXPECT_EQ(reloaded.get_bool("account[1].info.is_active", false), true);
}

SHOCKTEST_GOODWEATHER(save_string_quotes_values_when_needed)
{
    const char* input =
        ".account\n"
        "name = \"colin stewart\"\n"
        "title = \"dev # one\"\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);
    std::string output = POLYCONF::save_string(config, POLYCONF::FORMAT::ACF);
    POLYCONF::CONFIG reloaded = POLYCONF::load_string(output, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(reloaded.get_string("account.name", ""), "colin stewart");
    EXPECT_EQ(reloaded.get_string("account.title", ""), "dev # one");
    EXPECT_EQ(string_matches_ref(output, "ref/acf/save_quotes_expected.acf"), true);
}

SHOCKTEST_GOODWEATHER(save_preserves_acf_comments_loaded_from_file)
{
    POLYCONF::CONFIG config = POLYCONF::load_file("ref/acf/save_comments_input.acf", POLYCONF::FORMAT::ACF);
    std::string output = POLYCONF::save_string(config, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.get_string("account.name", ""), "colin stewart");
    EXPECT_EQ(config.get_bool("account.developer", false), true);
    EXPECT_EQ(string_matches_ref(output, "ref/acf/save_comments_input.acf"), true);
}

SHOCKTEST_GOODWEATHER(save_file_writes_acf_file)
{
    const char* input =
        ".account\n"
        "name = colin\n"
        "developer = true\n";

    std::filesystem::path temp_file =
        std::filesystem::temp_directory_path() /
        ("polyconf_test_" + std::to_string(std::rand()) + ".acf");

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);
    bool ok = POLYCONF::save_file(config, temp_file.string(), POLYCONF::FORMAT::ACF);

    EXPECT_EQ(ok, true);
    EXPECT_EQ(file_matches_ref(temp_file.string(), "ref/acf/auto_detect_basic.acf"), true);

    std::filesystem::remove(temp_file);
}
