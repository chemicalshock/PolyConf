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

SHOCKTEST_GOODWEATHER(get_int_reads_valid_integer)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");
    POLYCONF::NODE value;

    value.set_value("123");
    section.add_child("count", value);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_int("section.count", 0), 123);
}

SHOCKTEST_GOODWEATHER(get_float_reads_valid_float)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");
    POLYCONF::NODE value;

    value.set_value("3.14");
    section.add_child("ratio", value);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_float("section.ratio", 0.0), 3.14);
}

SHOCKTEST_GOODWEATHER(get_float_returns_default_on_invalid_conversion)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");
    POLYCONF::NODE value;

    value.set_value("hello");
    section.add_child("ratio", value);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_float("section.ratio", 1.5), 1.5);
}

SHOCKTEST_GOODWEATHER(get_bool_reads_false_string)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");
    POLYCONF::NODE value;

    value.set_value("false");
    section.add_child("enabled", value);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_bool("section.enabled", true), false);
}

SHOCKTEST_GOODWEATHER(get_bool_returns_default_on_invalid_conversion)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");
    POLYCONF::NODE value;

    value.set_value("maybe");
    section.add_child("enabled", value);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_bool("section.enabled", false), false);
}

SHOCKTEST_GOODWEATHER(get_string_reads_valid_string)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");
    POLYCONF::NODE value;

    value.set_value("hello");
    section.add_child("name", value);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_string("section.name", ""), "hello");
}

SHOCKTEST_GOODWEATHER(get_string_returns_default_on_missing_path)
{
    POLYCONF::NODE root;
    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_string("missing.value", "fallback"), "fallback");
}

SHOCKTEST_GOODWEATHER(has_returns_true_for_existing_path)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");
    POLYCONF::NODE value;

    value.set_value("1");
    section.add_child("enabled", value);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.has("section.enabled"), true);
}

SHOCKTEST_GOODWEATHER(has_returns_false_for_missing_path)
{
    POLYCONF::NODE root;
    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.has("section.enabled"), false);
}

SHOCKTEST_GOODWEATHER(count_returns_number_of_matching_children)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");
    POLYCONF::NODE first;
    POLYCONF::NODE second;

    first.set_value("one");
    second.set_value("two");

    section.add_child("value", first);
    section.add_child("value", second);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.count("section.value"), static_cast<std::size_t>(2));
}

SHOCKTEST_GOODWEATHER(count_returns_zero_for_missing_path)
{
    POLYCONF::NODE root;
    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.count("section.value"), static_cast<std::size_t>(0));
}

SHOCKTEST_GOODWEATHER(count_returns_zero_for_invalid_path)
{
    POLYCONF::NODE root;
    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.count("section..value"), static_cast<std::size_t>(0));
}

SHOCKTEST_GOODWEATHER(get_bool_is_case_insensitive)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");
    POLYCONF::NODE value1;
    POLYCONF::NODE value2;

    value1.set_value("TRUE");
    value2.set_value("False");

    section.add_child("enabled", value1);
    section.add_child("disabled", value2);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_bool("section.enabled", false), true);
    EXPECT_EQ(config.get_bool("section.disabled", true), false);
}

SHOCKTEST_GOODWEATHER(get_bool_accepts_yes_no_on_off)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& section = root.append_child("section");

    POLYCONF::NODE v1; v1.set_value("yes");
    POLYCONF::NODE v2; v2.set_value("no");
    POLYCONF::NODE v3; v3.set_value("on");
    POLYCONF::NODE v4; v4.set_value("off");

    section.add_child("yes_val", v1);
    section.add_child("no_val", v2);
    section.add_child("on_val", v3);
    section.add_child("off_val", v4);

    POLYCONF::CONFIG config(root);

    EXPECT_EQ(config.get_bool("section.yes_val", false), true);
    EXPECT_EQ(config.get_bool("section.no_val", true), false);
    EXPECT_EQ(config.get_bool("section.on_val", false), true);
    EXPECT_EQ(config.get_bool("section.off_val", true), false);
}