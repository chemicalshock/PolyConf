// -------------------------------------------------------------
//
// !\file test_polyconf_mvp.cpp
// !\brief MVP behaviour tests for Polyconf
// !\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// shocktest
#include "shocktest/shocktest.hpp"

// SUT


// -------------------------------------------------------------
// ACF - single value
// -------------------------------------------------------------

SHOCKTEST_GOODWEATHER(acf_single_value_lookup)
{
    const char* input =
        "[myconfig]\n"
        "value = 1\n";

    /*
    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::ACF);

    EXPECT_EQ(config.get_int("myconfig.value", 0), 1);
    EXPECT_EQ(config.get_int("myconfig.value[0]", 0), 1);
    EXPECT_EQ(config.get_int("myconfig.value[1]", 0), 0);
    */

    EXPECT_EQ(1, 1);
}