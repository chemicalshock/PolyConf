// -------------------------------------------------------------
//
//!\file acf_parser.cpp
//!\brief ACF parser for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// own header
#include "acf_parser.hpp"

// inbuilt
#include <cctype>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// internal
#include "node.hpp"

//
//!\brief Trim whitespace from both ends
//
std::string POLYCONF_acf_trim(const std::string& text)
{
    std::size_t start = 0;
    std::size_t end = text.size();

    while (start < end && std::isspace(static_cast<unsigned char>(text[start])))
    {
        ++start;
    }

    while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1])))
    {
        --end;
    }

    return text.substr(start, end - start);
}

//
//!\brief Remove inline comment from a line
//
std::string POLYCONF_acf_strip_comment(const std::string& text)
{
    bool in_quotes = false;
    std::size_t i = 0;

    while (i < text.size())
    {
        if (text[i] == '"')
        {
            in_quotes = !in_quotes;
        }
        else if (text[i] == '#' && !in_quotes)
        {
            return text.substr(0, i);
        }

        ++i;
    }

    return text;
}

//
//!\brief Remove surrounding double quotes if present
//
std::string POLYCONF_acf_unquote(const std::string& text)
{
    if (text.size() >= 2 && text.front() == '"' && text.back() == '"')
    {
        return text.substr(1, text.size() - 2);
    }

    return text;
}

//
//!\brief Split dotted section path
//
std::vector<std::string> POLYCONF_acf_split_path(const std::string& path)
{
    std::vector<std::string> parts;
    std::size_t start = 0;
    std::size_t i = 0;

    while (i <= path.size())
    {
        if (i == path.size() || path[i] == '.')
        {
            if (i == start)
            {
                parts.clear();
                return parts;
            }

            parts.push_back(path.substr(start, i - start));
            start = i + 1;
        }

        ++i;
    }

    return parts;
}

//
//!\brief Resolve parent path using latest existing nodes
//
POLYCONF::NODE* POLYCONF_acf_resolve_parent_path(POLYCONF::NODE& root, const std::vector<std::string>& parts)
{
    POLYCONF::NODE* current = &root;
    std::size_t i = 0;

    while (i < parts.size())
    {
        current = &(current->get_or_create_last_child(parts[i]));
        ++i;
    }

    return current;
}

//
//!\brief Resolve section path where final segment appends a new node
//
POLYCONF::NODE* POLYCONF_acf_open_section(POLYCONF::NODE& root, const std::vector<std::string>& parts)
{
    POLYCONF::NODE* current = &root;
    std::size_t i = 0;

    if (parts.empty())
    {
        return current;
    }

    while ((i + 1) < parts.size())
    {
        current = &(current->get_or_create_last_child(parts[i]));
        ++i;
    }

    return &(current->append_child(parts.back()));
}

//
//!\brief Parse ACF input into a configuration object
//
POLYCONF::CONFIG POLYCONF::parse_acf(const std::string& input)
{
    POLYCONF::NODE root;
    POLYCONF::NODE* current = &root;
    std::istringstream stream(input);
    std::string line;

    while (std::getline(stream, line))
    {
        std::string stripped = POLYCONF_acf_strip_comment(line);
        std::string trimmed = POLYCONF_acf_trim(stripped);

        if (trimmed.empty())
        {
            continue;
        }

        if (trimmed[0] == '.')
        {
            std::string section_path = POLYCONF_acf_trim(trimmed.substr(1));
            std::vector<std::string> parts = POLYCONF_acf_split_path(section_path);

            if (parts.empty())
            {
                continue;
            }

            current = POLYCONF_acf_open_section(root, parts);
            continue;
        }

        {
            std::size_t eq = trimmed.find('=');

            if (eq == std::string::npos)
            {
                continue;
            }

            std::string key = POLYCONF_acf_trim(trimmed.substr(0, eq));
            std::string value = POLYCONF_acf_trim(trimmed.substr(eq + 1));
            POLYCONF::NODE child;

            if (key.empty())
            {
                continue;
            }

            child.set_value(POLYCONF_acf_unquote(value));
            current->add_child(key, std::move(child));
        }
    }

    return POLYCONF::CONFIG(std::move(root));
}