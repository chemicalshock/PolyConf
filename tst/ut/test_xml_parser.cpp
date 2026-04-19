// -------------------------------------------------------------
//
//!\file test_xml_parser.cpp
//!\brief Tests for the Polyconf XML parser
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// shocktest
#include "shocktest/shocktest.hpp"

// SUT
#include "polyconf.hpp"

SHOCKTEST_GOODWEATHER(xml_deep_tree_lookup)
{
    const char* input =
        "<config>"
            "<value>"
                "<name>test</name>"
            "</value>"
        "</config>";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::XML);

    EXPECT_EQ(config.get_string("config.value.name", ""), "test");
    EXPECT_EQ(config.get_string("config.value[0].name[0]", ""), "test");
}

SHOCKTEST_GOODWEATHER(xml_repeated_nodes_behave_like_arrays)
{
    const char* input =
        "<config>"
            "<value><name>one</name></value>"
            "<value><name>two</name></value>"
        "</config>";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::XML);

    EXPECT_EQ(config.get_string("config.value.name", ""), "one");
    EXPECT_EQ(config.get_string("config.value[1].name", ""), "two");
    EXPECT_EQ(config.count("config.value"), 2);
}

SHOCKTEST_GOODWEATHER(xml_leaf_values_are_readable)
{
    const char* input =
        "<server>"
            "<addr>localhost</addr>"
            "<port>1234</port>"
            "<enabled>true</enabled>"
        "</server>";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::XML);

    EXPECT_EQ(config.get_string("server.addr", ""), "localhost");
    EXPECT_EQ(config.get_int("server.port", 0), 1234);
    EXPECT_EQ(config.get_bool("server.enabled", false), true);
}

SHOCKTEST_GOODWEATHER(xml_self_closing_tags_are_supported)
{
    const char* input =
        "<root>"
            "<empty/>"
            "<value>test</value>"
        "</root>";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::XML);

    EXPECT_EQ(config.has("root.empty"), true);
    EXPECT_EQ(config.get_string("root.empty", "fallback"), "fallback");
    EXPECT_EQ(config.get_string("root.value", ""), "test");
}

SHOCKTEST_GOODWEATHER(xml_declaration_and_comments_are_ignored)
{
    const char* input =
        "<?xml version=\"1.0\"?>"
        "<!-- comment -->"
        "<config>"
            "<value>123</value>"
        "</config>";

    POLYCONF::CONFIG config = POLYCONF::load_string(input, POLYCONF::FORMAT::XML);

    EXPECT_EQ(config.get_int("config.value", 0), 123);
}