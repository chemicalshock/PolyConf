// -------------------------------------------------------------
//
//!\file acf_writer.cpp
//!\brief ACF writer for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// own header
#include "acf_writer.hpp"

// inbuilt
#include <utility>
#include <sstream>
#include <string>
#include <vector>

// internal
#include "node.hpp"

//
//!\brief Join path parts into ACF section path
//
std::string POLYCONF_acf_join_path(const std::vector<std::string>& parts)
{
    std::ostringstream stream;
    std::size_t i = 0;

    while (i < parts.size())
    {
        if (i != 0)
        {
            stream << ".";
        }

        stream << parts[i];
        ++i;
    }

    return stream.str();
}

//
//!\brief Check whether value needs quotes
//
bool POLYCONF_acf_needs_quotes(const std::string& value)
{
    std::size_t i = 0;

    if (value.empty())
    {
        return true;
    }

    while (i < value.size())
    {
        if (value[i] == ' ' || value[i] == '\t' || value[i] == '#' || value[i] == '"')
        {
            return true;
        }

        ++i;
    }

    return false;
}

//
//!\brief Escape ACF value for output
//
std::string POLYCONF_acf_escape_value(const std::string& value)
{
    std::string output;
    std::size_t i = 0;

    output.reserve(value.size());

    while (i < value.size())
    {
        if (value[i] == '"')
        {
            output.push_back('\\');
        }

        output.push_back(value[i]);
        ++i;
    }

    return output;
}

//
//!\brief Write one scalar entry
//
void POLYCONF_acf_write_comment_line(std::ostringstream& stream, const std::string& comment)
{
    stream << "#";

    if (!comment.empty())
    {
        stream << " " << comment;
    }

    stream << "\n";
}

//
//!\brief Write leading comment lines for a node
//
void POLYCONF_acf_write_comments(std::ostringstream& stream, const std::vector<std::string>& comments)
{
    std::size_t i = 0;

    while (i < comments.size())
    {
        POLYCONF_acf_write_comment_line(stream, comments[i]);
        ++i;
    }
}

//
//!\brief Write an inline comment for a node when present
//
void POLYCONF_acf_write_inline_comment(std::ostringstream& stream, const POLYCONF::NODE& node)
{
    if (!node.has_inline_comment())
    {
        return;
    }

    stream << " #";

    if (!node.inline_comment().empty())
    {
        stream << " " << node.inline_comment();
    }
}

//
//!\brief Write one scalar entry
//
void POLYCONF_acf_write_scalar(std::ostringstream& stream, const std::string& key, const POLYCONF::NODE& node)
{
    std::string value = node.value();

    POLYCONF_acf_write_comments(stream, node.comments_before());

    stream << key << " = ";

    if (POLYCONF_acf_needs_quotes(value))
    {
        stream << "\"" << POLYCONF_acf_escape_value(value) << "\"";
    }
    else
    {
        stream << value;
    }

    POLYCONF_acf_write_inline_comment(stream, node);
    stream << "\n";
}

//
//!\brief Write one section node recursively
//
bool POLYCONF_acf_has_section_children(const POLYCONF::NODE& node)
{
    const std::vector<std::pair<std::string, std::size_t>>& child_order = node.child_order();
    std::size_t i = 0;

    while (i < child_order.size())
    {
        const std::string& key = child_order[i].first;
        std::size_t index = child_order[i].second;
        const std::vector<POLYCONF::NODE>* nodes = node.get_children(key);

        if (nodes != nullptr && index < nodes->size() && !((*nodes)[index].children().empty()))
        {
            return true;
        }

        ++i;
    }

    return false;
}

void POLYCONF_acf_write_section_node(
    std::ostringstream& stream,
    const POLYCONF::NODE& node,
    std::vector<std::string>& path_parts)
{
    const std::vector<std::pair<std::string, std::size_t>>& child_order = node.child_order();
    bool has_section_children = POLYCONF_acf_has_section_children(node);
    std::size_t i = 0;

    if (!path_parts.empty())
    {
        POLYCONF_acf_write_comments(stream, node.comments_before());
        stream << "." << POLYCONF_acf_join_path(path_parts);
        POLYCONF_acf_write_inline_comment(stream, node);
        stream << "\n";
    }

    while (i < child_order.size())
    {
        const std::string& key = child_order[i].first;
        std::size_t index = child_order[i].second;
        const std::vector<POLYCONF::NODE>* nodes = node.get_children(key);

        if (nodes != nullptr && index < nodes->size() && (*nodes)[index].has_value())
        {
            POLYCONF_acf_write_scalar(stream, key, (*nodes)[index]);
        }

        ++i;
    }

    if (has_section_children && !path_parts.empty())
    {
        stream << "\n";
    }

    i = 0;

    while (i < child_order.size())
    {
        const std::string& key = child_order[i].first;
        std::size_t index = child_order[i].second;
        const std::vector<POLYCONF::NODE>* nodes = node.get_children(key);

        if (nodes != nullptr && index < nodes->size() && !((*nodes)[index].children().empty()))
        {
            path_parts.push_back(key);
            POLYCONF_acf_write_section_node(stream, (*nodes)[index], path_parts);
            path_parts.pop_back();
        }

        ++i;
    }

    POLYCONF_acf_write_comments(stream, node.trailing_comments());
}

//
//!\brief Serialise configuration to ACF string
//
std::string POLYCONF::write_acf(const POLYCONF::CONFIG& config)
{
    std::ostringstream stream;
    std::vector<std::string> path_parts;
    const std::vector<std::pair<std::string, std::size_t>>& child_order = config.root().child_order();
    std::size_t i = 0;

    while (i < child_order.size())
    {
        const std::string& key = child_order[i].first;
        std::size_t index = child_order[i].second;
        const std::vector<POLYCONF::NODE>* nodes = config.root().get_children(key);

        if (nodes != nullptr && index < nodes->size())
        {
            path_parts.push_back(key);
            POLYCONF_acf_write_section_node(stream, (*nodes)[index], path_parts);
            path_parts.pop_back();
        }

        ++i;
    }

    POLYCONF_acf_write_comments(stream, config.root().trailing_comments());

    return stream.str();
}
