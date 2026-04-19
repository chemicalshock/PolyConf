// -------------------------------------------------------------
//
//!\file test_loader.cpp
//!\brief Tests for Polyconf loader behaviour
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// shocktest
#include "shocktest/shocktest.hpp"

// SUT
#include "polyconf.hpp"

SHOCKTEST_GOODWEATHER(load_string_auto_detects_acf_from_content)
{
    const char* input =
        ".account\n"
        "name = colin\n"
        "developer = true\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.get_string("account.name", ""), "colin");
    EXPECT_EQ(config.get_bool("account.developer", false), true);
}

SHOCKTEST_GOODWEATHER(load_string_auto_detects_xml_from_content)
{
    const char* input =
        "<account>"
            "<name>colin</name>"
            "<developer>true</developer>"
        "</account>";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.get_string("account.name", ""), "colin");
    EXPECT_EQ(config.get_bool("account.developer", false), true);
}

SHOCKTEST_GOODWEATHER(load_string_auto_detect_ignores_leading_whitespace)
{
    const char* input =
        "\n"
        "   \n"
        "\t<config><value>7</value></config>";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.get_int("config.value", 0), 7);
}

SHOCKTEST_GOODWEATHER(load_file_auto_detects_acf_from_extension)
{
    POLYCONF::CONFIG config = POLYCONF::load_file("ref/acf/auto_detect_basic.acf", POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.get_string("account.name", ""), "colin");
    EXPECT_EQ(config.get_bool("account.developer", false), true);
}

SHOCKTEST_GOODWEATHER(load_file_auto_detects_xml_from_extension)
{
    POLYCONF::CONFIG config = POLYCONF::load_file("ref/xml/auto_detect_basic.xml", POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.get_string("account.name", "colin"), "colin");
    EXPECT_EQ(config.get_bool("account.developer", false), true);
}

SHOCKTEST_GOODWEATHER(load_file_auto_detect_falls_back_when_no_extension)
{
    POLYCONF::CONFIG config = POLYCONF::load_file("ref/xml/auto_detect_no_ext", POLYCONF::FORMAT::AUTO);
    EXPECT_EQ(config.get_int("config.value", 0), 7);
}

SHOCKTEST_GOODWEATHER(load_file_auto_detect_falls_back_when_extension_unknown)
{
    POLYCONF::CONFIG config = POLYCONF::load_file("ref/acf/auto_detect_unknown.ext", POLYCONF::FORMAT::AUTO);
    EXPECT_EQ(config.get_string("server.addr", ""), "localhost");
}

SHOCKTEST_BADWEATHER(load_file_missing_file_returns_empty_config)
{
    POLYCONF::CONFIG config = POLYCONF::load_file("ref/acf/does_not_exist.acf", POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.has("anything"), false);
    EXPECT_EQ(config.count("anything"), static_cast<std::size_t>(0));
    EXPECT_EQ(config.get_string("anything", "fallback"), "fallback");
}