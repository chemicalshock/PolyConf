// -------------------------------------------------------------
//
//!\file test_polyconf_mvp.cpp
//!\brief Basic smoke tests for Polyconf public API
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// shocktest
#include "shocktest/shocktest.hpp"

// SUT
#include "polyconf.hpp"

// -------------------------------------------------------------
// ACF smoke test
// -------------------------------------------------------------

SHOCKTEST_GOODWEATHER(mvp_acf_basic_smoke)
{
    const char* input =
        ".account\n"
        "name = colin\n"
        "age = 30\n"
        "\n"
        ".account\n"
        "name = orim\n"
        ".account.info\n"
        "is_agent = true\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.get_string("account.name", ""), "colin");
    EXPECT_EQ(config.get_int("account.age", 0), 30);
    EXPECT_EQ(config.get_string("account[1].name", ""), "orim");
    EXPECT_EQ(config.get_bool("account[1].info.is_agent", false), true);
}

// -------------------------------------------------------------
// Future XML smoke test placeholder
// -------------------------------------------------------------

SHOCKTEST_GOODWEATHER(mvp_xml_basic_smoke)
{
    const char* input =
        "<config>"
            "<value>"
                "<name>test</name>"
            "</value>"
        "</config>";
    
    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::XML);
    
    EXPECT_EQ(config.get_string("config.value.name", ""), "test");
}

SHOCKTEST_GOODWEATHER(mvp_merge_string_acf_overwrites_existing_and_keeps_new_nodes)
{
    const char* input =
        ".account\n"
        "name = colin\n"
        "age = 30\n"
        "\n"
        ".account\n"
        "name = orim\n"
        "active = true\n"
        ".account.info\n"
        "is_agent = true\n";

    POLYCONF::CONFIG config = POLYCONF::merge_string(input, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.count("account"), static_cast<std::size_t>(1));
    EXPECT_EQ(config.get_string("account.name", ""), "orim");
    EXPECT_EQ(config.get_int("account.age", 0), 30);
    EXPECT_EQ(config.get_bool("account.active", false), true);
    EXPECT_EQ(config.get_bool("account.info.is_agent", false), true);
}

SHOCKTEST_GOODWEATHER(mvp_merge_string_xml_overwrites_existing_and_keeps_new_nodes)
{
    const char* input =
        "<config>"
            "<server>"
                "<host>localhost</host>"
            "</server>"
            "<server>"
                "<host>prod</host>"
                "<port>8080</port>"
            "</server>"
        "</config>";

    POLYCONF::CONFIG config = POLYCONF::merge_string(input, POLYCONF::FORMAT::XML);

    EXPECT_EQ(config.count("config.server"), static_cast<std::size_t>(1));
    EXPECT_EQ(config.get_string("config.server.host", ""), "prod");
    EXPECT_EQ(config.get_int("config.server.port", 0), 8080);
}

SHOCKTEST_GOODWEATHER(mvp_merge_file_auto_detects_and_merges_acf)
{
    POLYCONF::CONFIG config = POLYCONF::merge_file("ref/acf/merge_basic.acf", POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.count("account"), static_cast<std::size_t>(1));
    EXPECT_EQ(config.get_string("account.name", ""), "beta");
    EXPECT_EQ(config.get_int("account.retry", 0), 2);
    EXPECT_EQ(config.get_bool("account.info.active", false), true);
}

// -------------------------------------------------------------
// Future INI smoke test placeholder
// -------------------------------------------------------------

SHOCKTEST_GOODWEATHER(mvp_ini_basic_smoke)
{
    // TODO:
    //
    // const char* input =
    //     "...";
    //
    // POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::INI);
    //
    // EXPECT_EQ(...);

    EXPECT_EQ(1, 1);
}

SHOCKTEST_GOODWEATHER(mvp_auto_detects_acf)
{
    const char* input =
        ".account\n"
        "name = colin\n";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.get_string("account.name", ""), "colin");
}

SHOCKTEST_GOODWEATHER(mvp_auto_detects_xml)
{
    const char* input =
        "<config>"
            "<value>123</value>"
        "</config>";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.get_int("config.value", 0), 123);
}

SHOCKTEST_GOODWEATHER(mvp_auto_detect_ignores_leading_whitespace)
{
    const char* input =
        "\n"
        "   \n"
        "\t<config><value>7</value></config>";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.get_int("config.value", 0), 7);
}

SHOCKTEST_GOODWEATHER(auto_detect_file_extension_xml)
{
    // simulate behaviour by calling detection directly via load_string
    // (full file test later if you add temp file helper)

    const char* input =
        "<config><value>5</value></config>";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::AUTO);

    EXPECT_EQ(config.get_int("config.value", 0), 5);
}
