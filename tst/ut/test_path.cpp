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

SHOCKTEST_GOODWEATHER(resolve_simple_path)
{
    POLYCONF::NODE root;
    POLYCONF::NODE& config = root.append_child("config");
    POLYCONF::NODE child;

    child.set_value("123");
    config.add_child("value", child);

    const POLYCONF::NODE* result = POLYCONF::resolve_path(root, "config.value");

    EXPECT_TRUE(result != nullptr);
    EXPECT_EQ(result->value(), "123");
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

    const POLYCONF::NODE* result = POLYCONF::resolve_path(root, "config.value[1]");

    EXPECT_TRUE(result != nullptr);
    EXPECT_EQ(result->value(), "two");
}