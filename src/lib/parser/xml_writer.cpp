// -------------------------------------------------------------
//
//!\file xml_writer.cpp
//!\brief XML writer for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// own header
#include "xml_writer.hpp"

// inbuilt
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// internal
#include "node.hpp"

//
//!\brief Write indentation for one XML depth
//
void POLYCONF_xml_write_indent(std::ostringstream& stream, std::size_t depth)
{
    std::size_t i = 0;

    while (i < depth)
    {
        stream << "    ";
        ++i;
    }
}

//
//!\brief Escape XML text content
//
std::string POLYCONF_xml_escape_text(const std::string& value)
{
    std::string output;
    std::size_t i = 0;

    output.reserve(value.size());

    while (i < value.size())
    {
        if (value[i] == '&')
        {
            output += "&amp;";
        }
        else if (value[i] == '<')
        {
            output += "&lt;";
        }
        else if (value[i] == '>')
        {
            output += "&gt;";
        }
        else
        {
            output.push_back(value[i]);
        }

        ++i;
    }

    return output;
}

//
//!\brief Escape XML comment content enough to keep output valid
//
std::string POLYCONF_xml_escape_comment(const std::string& comment)
{
    std::string output;
    std::size_t i = 0;

    output.reserve(comment.size());

    while (i < comment.size())
    {
        if (comment[i] == '-' && (i + 1) < comment.size() && comment[i + 1] == '-')
        {
            output += "- -";
            i += 2;
            continue;
        }

        output.push_back(comment[i]);
        ++i;
    }

    if (!output.empty() && output.back() == '-')
    {
        output.push_back(' ');
    }

    return output;
}

//
//!\brief Write one XML comment line
//
void POLYCONF_xml_write_comment_line(
    std::ostringstream& stream,
    const std::string& comment,
    std::size_t depth)
{
    std::string escaped = POLYCONF_xml_escape_comment(comment);

    POLYCONF_xml_write_indent(stream, depth);
    stream << "<!--";

    if (!escaped.empty())
    {
        stream << " " << escaped << " ";
    }

    stream << "-->\n";
}

//
//!\brief Write XML comments at a depth
//
void POLYCONF_xml_write_comments(
    std::ostringstream& stream,
    const std::vector<std::string>& comments,
    std::size_t depth)
{
    std::size_t i = 0;

    while (i < comments.size())
    {
        POLYCONF_xml_write_comment_line(stream, comments[i], depth);
        ++i;
    }
}

//
//!\brief Check whether a node has comments that must live inside an element
//
bool POLYCONF_xml_has_inner_comments(const POLYCONF::NODE& node)
{
    return node.has_inline_comment() || !node.trailing_comments().empty();
}

//
//!\brief Write one XML element recursively
//
void POLYCONF_xml_write_node(
    std::ostringstream& stream,
    const std::string& name,
    const POLYCONF::NODE& node,
    std::size_t depth)
{
    const std::vector<std::pair<std::string, std::size_t>>& child_order = node.child_order();
    bool has_children = !child_order.empty();
    bool has_inner_comments = POLYCONF_xml_has_inner_comments(node);
    std::size_t i = 0;

    POLYCONF_xml_write_comments(stream, node.comments_before(), depth);
    POLYCONF_xml_write_indent(stream, depth);

    if (!node.has_value() && !has_children && !has_inner_comments)
    {
        stream << "<" << name << "/>\n";
        return;
    }

    if (node.has_value() && !has_children && !has_inner_comments)
    {
        stream << "<" << name << ">" << POLYCONF_xml_escape_text(node.value()) << "</" << name << ">\n";
        return;
    }

    stream << "<" << name << ">\n";

    if (node.has_value())
    {
        POLYCONF_xml_write_indent(stream, depth + 1);
        stream << POLYCONF_xml_escape_text(node.value()) << "\n";
    }

    while (i < child_order.size())
    {
        const std::string& key = child_order[i].first;
        std::size_t index = child_order[i].second;
        const std::vector<POLYCONF::NODE>* nodes = node.get_children(key);

        if (nodes != nullptr && index < nodes->size())
        {
            POLYCONF_xml_write_node(stream, key, (*nodes)[index], depth + 1);
        }

        ++i;
    }

    if (node.has_inline_comment())
    {
        POLYCONF_xml_write_comment_line(stream, node.inline_comment(), depth + 1);
    }

    POLYCONF_xml_write_comments(stream, node.trailing_comments(), depth + 1);

    POLYCONF_xml_write_indent(stream, depth);
    stream << "</" << name << ">\n";
}

//
//!\brief Serialise configuration to XML string
//
std::string POLYCONF::write_xml(const POLYCONF::CONFIG& config)
{
    std::ostringstream stream;
    const std::vector<std::pair<std::string, std::size_t>>& child_order = config.root().child_order();
    std::size_t i = 0;

    while (i < child_order.size())
    {
        const std::string& key = child_order[i].first;
        std::size_t index = child_order[i].second;
        const std::vector<POLYCONF::NODE>* nodes = config.root().get_children(key);

        if (nodes != nullptr && index < nodes->size())
        {
            POLYCONF_xml_write_node(stream, key, (*nodes)[index], 0);
        }

        ++i;
    }

    POLYCONF_xml_write_comments(stream, config.root().trailing_comments(), 0);

    return stream.str();
}
