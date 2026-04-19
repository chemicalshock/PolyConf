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
    // TODO:
    //
    // const char* input =
    //     "<config>"
    //         "<value>"
    //             "<name>test</name>"
    //         "</value>"
    //     "</config>";
    //
    // POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::XML);
    //
    // EXPECT_EQ(config.get_string("config.value.name", ""), "test");

    EXPECT_EQ(1, 1);
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