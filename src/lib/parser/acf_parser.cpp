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
//!\brief Find inline comment marker outside quoted strings
//
std::size_t POLYCONF_acf_find_comment(const std::string& text)
{
    bool escaped = false;
    bool in_quotes = false;
    std::size_t i = 0;

    while (i < text.size())
    {
        if (escaped)
        {
            escaped = false;
        }
        else if (text[i] == '\\' && in_quotes)
        {
            escaped = true;
        }
        else if (text[i] == '"')
        {
            in_quotes = !in_quotes;
        }
        else if (text[i] == '#' && !in_quotes)
        {
            return i;
        }

        ++i;
    }

    return std::string::npos;
}

//
//!\brief Split ACF line into content and optional comment text
//
void POLYCONF_acf_split_comment(
    const std::string& line,
    std::string& content,
    std::string& comment,
    bool& has_comment)
{
    std::size_t pos = POLYCONF_acf_find_comment(line);

    if (pos == std::string::npos)
    {
        content = line;
        comment.clear();
        has_comment = false;
        return;
    }

    content = line.substr(0, pos);
    comment = POLYCONF_acf_trim(line.substr(pos + 1));
    has_comment = true;
}

//
//!\brief Attach pending comments to a parsed node
//
void POLYCONF_acf_apply_pending_comments(POLYCONF::NODE& node, std::vector<std::string>& pending_comments)
{
    std::size_t i = 0;

    while (i < pending_comments.size())
    {
        node.add_comment_before(std::move(pending_comments[i]));
        ++i;
    }

    pending_comments.clear();
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
    std::vector<std::string> pending_comments;
    std::istringstream stream(input);
    std::string line;

    while (std::getline(stream, line))
    {
        std::string content;
        std::string comment;
        bool has_comment = false;
        std::string trimmed;

        POLYCONF_acf_split_comment(line, content, comment, has_comment);
        trimmed = POLYCONF_acf_trim(content);

        if (trimmed.empty())
        {
            if (has_comment)
            {
                pending_comments.push_back(std::move(comment));
            }

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
            POLYCONF_acf_apply_pending_comments(*current, pending_comments);

            if (has_comment)
            {
                current->set_inline_comment(std::move(comment));
            }

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
            POLYCONF_acf_apply_pending_comments(child, pending_comments);

            if (has_comment)
            {
                child.set_inline_comment(std::move(comment));
            }

            current->add_child(key, std::move(child));
        }
    }

    {
        std::size_t i = 0;

        while (i < pending_comments.size())
        {
            root.add_trailing_comment(std::move(pending_comments[i]));
            ++i;
        }
    }

    return POLYCONF::CONFIG(std::move(root));
}
