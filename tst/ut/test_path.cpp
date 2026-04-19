// -------------------------------------------------------------
//
//!\file test_path.cpp
//!\brief Tests for Polyconf path parsing and resolution
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// shocktest
#include "shocktest/shocktest.hpp"

// SUT
#include "node.hpp"
#include "path.hpp"

SHOCKTEST_GOODWEATHER(parse_simple_path)
{
    POLYCONF::PATH path = POLYCONF::parse_path("config.value.name");
    const std::vector<POLYCONF::PATH_SEGMENT>& segments = path.segments();

    EXPECT_EQ(path.empty(), false);
    EXPECT_EQ(segments.size(), static_cast<std::size_t>(3));

    EXPECT_EQ(segments[0].name, "config");
    EXPECT_EQ(segments[0].index, static_cast<std::size_t>(0));

    EXPECT_EQ(segments[1].name, "value");
    EXPECT_EQ(segments[1].index, static_cast<std::size_t>(0));

    EXPECT_EQ(segments[2].name, "name");
    EXPECT_EQ(segments[2].index, static_cast<std::size_t>(0));
}

SHOCKTEST_GOODWEATHER(parse_indexed_path)
{
    POLYCONF::PATH path = POLYCONF::parse_path("config.value[1].name[2]");
    const std::vector<POLYCONF::PATH_SEGMENT>& segments = path.segments();

    EXPECT_EQ(path.empty(), false);
    EXPECT_EQ(segments.size(), static_cast<std::size_t>(3));

    EXPECT_EQ(segments[0].name, "config");
    EXPECT_EQ(segments[0].index, static_cast<std::size_t>(0));

    EXPECT_EQ(segments[1].name, "value");
    EXPECT_EQ(segments[1].index, static_cast<std::size_t>(1));

    EXPECT_EQ(segments[2].name, "name");
    EXPECT_EQ(segments[2].index, static_cast<std::size_t>(2));
}

SHOCKTEST_GOODWEATHER(resolve_simple_path)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& config = root.append_child("config");
    POLYCONF::NODE value;

    value.set_value("123");
    config.add_child("value", value);

    {
        const POLYCONF::NODE* result = POLYCONF::resolve_path(root, "config.value");

        EXPECT_TRUE(result != nullptr);
        EXPECT_EQ(result->has_value(), true);
        EXPECT_EQ(result->value(), "123");
    }
}

SHOCKTEST_GOODWEATHER(resolve_indexed_path)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& config = root.append_child("config");
    POLYCONF::NODE first;
    POLYCONF::NODE second;

    first.set_value("one");
    second.set_value("two");

    config.add_child("value", first);
    config.add_child("value", second);

    {
        const POLYCONF::NODE* result = POLYCONF::resolve_path(root, "config.value[1]");

        EXPECT_TRUE(result != nullptr);
        EXPECT_EQ(result->has_value(), true);
        EXPECT_EQ(result->value(), "two");
    }
}

SHOCKTEST_GOODWEATHER(resolve_deep_path)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& config = root.append_child("config");
    POLYCONF::NODE& value = config.append_child("value");
    POLYCONF::NODE name;

    name.set_value("test");
    value.add_child("name", name);

    {
        const POLYCONF::NODE* result = POLYCONF::resolve_path(root, "config.value.name");

        EXPECT_TRUE(result != nullptr);
        EXPECT_EQ(result->has_value(), true);
        EXPECT_EQ(result->value(), "test");
    }
}

SHOCKTEST_GOODWEATHER(resolve_deep_indexed_path)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& config = root.append_child("config");
    POLYCONF::NODE first_name;
    POLYCONF::NODE second_name;

    first_name.set_value("one");
    second_name.set_value("two");

    config.append_child("value");
    config.append_child("value");

    {
        POLYCONF::NODE* first_value = config.get_child("value", 0);
        POLYCONF::NODE* second_value = config.get_child("value", 1);

        EXPECT_TRUE(first_value != nullptr);
        EXPECT_TRUE(second_value != nullptr);

        first_value->add_child("name", first_name);
        second_value->add_child("name", second_name);
    }

    {
        const POLYCONF::NODE* result = POLYCONF::resolve_path(root, "config.value[1].name");

        EXPECT_TRUE(result != nullptr);
        EXPECT_EQ(result->has_value(), true);
        EXPECT_EQ(result->value(), "two");
    }
}

SHOCKTEST_GOODWEATHER(resolve_missing_path_returns_null)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& config = root.append_child("config");
    POLYCONF::NODE value;

    value.set_value("123");
    config.add_child("value", value);

    const POLYCONF::NODE* result = POLYCONF::resolve_path(root, "config.missing");

    EXPECT_TRUE(result == nullptr);
}

SHOCKTEST_GOODWEATHER(resolve_out_of_range_index_returns_null)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& config = root.append_child("config");
    POLYCONF::NODE value;

    value.set_value("123");
    config.add_child("value", value);

    const POLYCONF::NODE* result = POLYCONF::resolve_path(root, "config.value[1]");

    EXPECT_TRUE(result == nullptr);
}

SHOCKTEST_BADWEATHER(parse_invalid_empty_path_throws)
{
    EXPECT_THROW((void)POLYCONF::parse_path(""));
}

SHOCKTEST_BADWEATHER(parse_invalid_trailing_dot_throws)
{
    EXPECT_THROW((void)POLYCONF::parse_path("config."));
}

SHOCKTEST_BADWEATHER(parse_invalid_empty_index_throws)
{

    EXPECT_THROW((void)POLYCONF::parse_path("config.value[]"));
}

SHOCKTEST_BADWEATHER(parse_invalid_missing_bracket_throws)
{
    EXPECT_THROW((void)POLYCONF::parse_path("config.value[1"));
}

SHOCKTEST_BADWEATHER(parse_invalid_non_numeric_index_throws)
{
    EXPECT_THROW((void)POLYCONF::parse_path("config.value[abc]"));
}

SHOCKTEST_BADWEATHER(parse_invalid_double_dot_throws)
{
    EXPECT_THROW((void)POLYCONF::parse_path("config..value"));
}