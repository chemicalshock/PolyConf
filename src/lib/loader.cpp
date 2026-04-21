// -------------------------------------------------------------
//
//!\file loader.cpp
//!\brief Configuration loading implementation for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// own header
#include "loader.hpp"

// inbuilt
#include <fstream>
#include <sstream>
#include <utility>
#include <cctype>
#include <fstream>
#include <sstream>

// internal
#include "parser/acf_parser.hpp"
#include "parser/xml_parser.hpp"


namespace
{
    //
    //!\brief Merge one node into another
    //
    void merge_node_into(POLYCONF::NODE& target, const POLYCONF::NODE& source);

    //
    //!\brief Create a merged copy of a node
    //
    POLYCONF::NODE merge_node_copy(const POLYCONF::NODE& source)
    {
        POLYCONF::NODE merged;

        merge_node_into(merged, source);

        return merged;
    }

    //
    //!\brief Merge one node into another
    //
    void merge_node_into(POLYCONF::NODE& target, const POLYCONF::NODE& source)
    {
        const std::vector<std::string>& comments_before = source.comments_before();
        const std::vector<std::string>& trailing_comments = source.trailing_comments();
        const std::vector<std::pair<std::string, std::size_t>>& child_order = source.child_order();
        std::size_t i = 0;

        if (source.has_value())
        {
            target.set_value(source.value());
        }

        while (i < comments_before.size())
        {
            target.add_comment_before(comments_before[i]);
            ++i;
        }

        if (source.has_inline_comment())
        {
            target.set_inline_comment(source.inline_comment());
        }

        i = 0;

        while (i < child_order.size())
        {
            const std::string& name = child_order[i].first;
            const POLYCONF::NODE& child = source.children().at(name)[child_order[i].second];
            POLYCONF::NODE* existing = target.get_last_child(name);

            if (existing == nullptr)
            {
                target.add_child(name, merge_node_copy(child));
            }
            else
            {
                merge_node_into(*existing, child);
            }

            ++i;
        }

        i = 0;

        while (i < trailing_comments.size())
        {
            target.add_trailing_comment(trailing_comments[i]);
            ++i;
        }
    }

    //
    //!\brief Detect configuration format from file extension
    //
    POLYCONF::FORMAT detect_format_from_extension(const std::string& file)
    {
        std::size_t pos = file.find_last_of('.');

        if (pos == std::string::npos)
        {
            return POLYCONF::FORMAT::AUTO;
        }

        std::string ext = file.substr(pos + 1);
        std::string lower;
        std::size_t i;

        lower.reserve(ext.size());

        for (i = 0; i < ext.size(); ++i)
        {
            lower.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ext[i]))));
        }

        if (lower == "xml")
        {
            return POLYCONF::FORMAT::XML;
        }

        if (lower == "acf" || lower == "conf" || lower == "cfg")
        {
            return POLYCONF::FORMAT::ACF;
        }

        return POLYCONF::FORMAT::AUTO;
    }

    //
    //!\brief Detect configuration format from input
    //
    POLYCONF::FORMAT detect_format(const std::string& input)
    {
        std::size_t i = 0;

        while (i < input.size() && std::isspace(static_cast<unsigned char>(input[i])))
        {
            ++i;
        }

        if (i >= input.size())
        {
            return POLYCONF::FORMAT::ACF;
        }

        if (input[i] == '<')
        {
            return POLYCONF::FORMAT::XML;
        }

        if (input[i] == '.')
        {
            return POLYCONF::FORMAT::ACF;
        }

        return POLYCONF::FORMAT::ACF;
    }
}

//
//!\brief Load configuration from string
//
POLYCONF::CONFIG POLYCONF::load_string(const std::string& input, POLYCONF::FORMAT format)
{
    POLYCONF::FORMAT detected_format = format;

    if (detected_format == POLYCONF::FORMAT::AUTO)
    {
        detected_format = ::detect_format(input);
    }

    switch (detected_format)
    {
        case POLYCONF::FORMAT::ACF:
            return POLYCONF::parse_acf(input);

        case POLYCONF::FORMAT::XML:
            return POLYCONF::parse_xml(input);

        default:
            return POLYCONF::CONFIG();
    }
}

//
//!\brief Load configuration from file
//
POLYCONF::CONFIG POLYCONF::load_file(const std::string& file, POLYCONF::FORMAT format)
{
    std::ifstream stream(file.c_str());
    std::stringstream buffer;
    POLYCONF::FORMAT detected_format = format;

    if (!stream)
    {
        return POLYCONF::CONFIG();
    }

    if (detected_format == POLYCONF::FORMAT::AUTO)
    {
        detected_format = ::detect_format_from_extension(file);
    }

    buffer << stream.rdbuf();

    if (detected_format == POLYCONF::FORMAT::AUTO)
    {
        return POLYCONF::load_string(buffer.str(), POLYCONF::FORMAT::AUTO);
    }

    return POLYCONF::load_string(buffer.str(), detected_format);
}

//
//!\brief Merge a configuration string into an existing configuration
//
POLYCONF::CONFIG POLYCONF::merge_string(
    const POLYCONF::CONFIG& config,
    const std::string& input,
    POLYCONF::FORMAT format)
{
    POLYCONF::CONFIG incoming = POLYCONF::load_string(input, format);
    POLYCONF::NODE merged_root(config.root());

    merge_node_into(merged_root, incoming.root());

    return POLYCONF::CONFIG(std::move(merged_root));
}

//
//!\brief Merge a configuration file into an existing configuration
//
POLYCONF::CONFIG POLYCONF::merge_file(
    const POLYCONF::CONFIG& config,
    const std::string& file,
    POLYCONF::FORMAT format)
{
    POLYCONF::CONFIG incoming = POLYCONF::load_file(file, format);
    POLYCONF::NODE merged_root(config.root());

    merge_node_into(merged_root, incoming.root());

    return POLYCONF::CONFIG(std::move(merged_root));
}
