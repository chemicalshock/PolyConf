// -------------------------------------------------------------
//
//!\file test_config.cpp
//!\brief Tests for Polyconf CONFIG accessors
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// shocktest
#include "shocktest/shocktest.hpp"

// SUT
#include "config.hpp"
#include "node.hpp"

SHOCKTEST_GOODWEATHER(get_int_returns_default_on_missing_path)
{
    POLYCONF::NODE root;
    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_int("missing.value", 42), 42);
}

SHOCKTEST_GOODWEATHER(get_bool_reads_true_string)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");
    POLYCONF::NODE value;

    value.set_value("true");
    section.add_child("enabled", value);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_bool("section.enabled", false), true);
}

SHOCKTEST_GOODWEATHER(get_int_returns_default_on_invalid_conversion)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");
    POLYCONF::NODE value;

    value.set_value("hello");
    section.add_child("count", value);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_int("section.count", 7), 7);
}