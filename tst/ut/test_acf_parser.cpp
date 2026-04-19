// -------------------------------------------------------------
//
//!\file test_acf_parser.cpp
//!\brief Tests for the Polyconf ACF parser
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// shocktest
#include "shocktest/shocktest.hpp"

// SUT
#include "polyconf.hpp"

SHOCKTEST_GOODWEATHER(acf_single_value_lookup)
{
    const char* input =
        ".myconfig\n"
        "value = 1\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.get_int("myconfig.value", 0), 1);
    EXPECT_EQ(config.get_int("myconfig.value[0]", 0), 1);
    EXPECT_EQ(config.get_int("myconfig.value[1]", 0), 0);
}

SHOCKTEST_GOODWEATHER(acf_repeated_keys_become_array)
{
    const char* input =
        ".config\n"
        "value = 1\n"
        "value = 2\n"
        "value = 3\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.get_int("config.value", 0), 1);
    EXPECT_EQ(config.get_int("config.value[0]", 0), 1);
    EXPECT_EQ(config.get_int("config.value[1]", 0), 2);
    EXPECT_EQ(config.get_int("config.value[2]", 0), 3);
}

SHOCKTEST_GOODWEATHER(acf_repeated_sections_create_multiple_nodes)
{
    const char* input =
        ".account\n"
        "name = username\n"
        "age = 30\n"
        "\n"
        ".account\n"
        "name = seconduser\n"
        "age = 42\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.get_string("account.name", ""), "username");
    EXPECT_EQ(config.get_int("account.age", 0), 30);
    EXPECT_EQ(config.get_string("account[1].name", ""), "seconduser");
    EXPECT_EQ(config.get_int("account[1].age", 0), 42);
}

SHOCKTEST_GOODWEATHER(acf_subsection_attaches_to_latest_parent)
{
    const char* input =
        ".account\n"
        "name = colin\n"
        "\n"
        ".account\n"
        "name = orim\n"
        ".account.info\n"
        "is_agent = true\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.get_string("account.name", ""), "colin");
    EXPECT_EQ(config.get_string("account[1].name", ""), "orim");
    EXPECT_EQ(config.get_bool("account[1].info.is_agent", false), true);
    EXPECT_EQ(config.count("account"), 2);
    EXPECT_EQ(config.count("account.info"), 0);
    EXPECT_EQ(config.count("account[1].info"), 1);
    EXPECT_EQ(config.has("account.info.is_agent"), false);
}

SHOCKTEST_GOODWEATHER(acf_quoted_values_are_unquoted)
{
    const char* input =
        ".another\n"
        "def = \"another test\"\n"
        "def = \"def is an array\"\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.get_string("another.def", ""), "another test");
    EXPECT_EQ(config.get_string("another.def[1]", ""), "def is an array");
}

SHOCKTEST_GOODWEATHER(acf_inline_comments_are_ignored)
{
    const char* input =
        ".server\n"
        "addr = localhost # comment\n"
        "port = 1234\n"
        "name = \"web # not comment\"\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.get_string("server.addr", ""), "localhost");
    EXPECT_EQ(config.get_int("server.port", 0), 1234);
    EXPECT_EQ(config.get_string("server.name", ""), "web # not comment");
}

SHOCKTEST_GOODWEATHER(acf_missing_path_returns_default)
{
    const char* input =
        ".server\n"
        "addr = localhost\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.get_string("server.missing", "fallback"), "fallback");
    EXPECT_EQ(config.get_int("server.port", 8080), 8080);
    EXPECT_EQ(config.get_bool("server.enabled", true), true);
    EXPECT_EQ(config.count("server.missing"), 0);
}

SHOCKTEST_GOODWEATHER(acf_invalid_conversion_returns_default)
{
    const char* input =
        ".server\n"
        "addr = localhost\n"
        "enabled = maybe\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.get_int("server.addr", 7), 7);
    EXPECT_EQ(config.get_bool("server.enabled", false), false);
}

SHOCKTEST_GOODWEATHER(acf_auto_detect_defaults_to_acf)
{
    const char* input =
        ".server\n"
        "addr = localhost\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.get_string("server.addr", ""), "localhost");
}