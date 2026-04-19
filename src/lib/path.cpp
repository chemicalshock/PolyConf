// -------------------------------------------------------------
//
//!\file path.cpp
//!\brief Path parsing and resolution for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// own header
#include "path.hpp"

// inbuilt
#include <cctype>
#include <stdexcept>
#include <utility>

//
//!\brief Construct from parsed segments
//
POLYCONF::PATH::PATH(const std::vector<POLYCONF::PATH_SEGMENT>& segments)
    : m_segments(segments)
{
}

//
//!\brief Construct from parsed segments (move)
//
POLYCONF::PATH::PATH(std::vector<POLYCONF::PATH_SEGMENT>&& segments)
    : m_segments(std::move(segments))
{
}

//
//!\brief Get path segments
//
const std::vector<POLYCONF::PATH_SEGMENT>& POLYCONF::PATH::segments() const
{
    return m_segments;
}

//
//!\brief Check whether path is empty
//
bool POLYCONF::PATH::empty() const
{
    return m_segments.empty();
}

//
//!\brief Parse a path string
//
POLYCONF::PATH POLYCONF::parse_path(const std::string& path)
{
    std::vector<POLYCONF::PATH_SEGMENT> segments;
    std::size_t i = 0;

    if (path.empty())
    {
        throw std::runtime_error("Invalid path: empty path");
    }

    while (i < path.size())
    {
        POLYCONF::PATH_SEGMENT segment;
        std::size_t name_start = i;

        while (i < path.size() && path[i] != '.' && path[i] != '[')
        {
            ++i;
        }

        if (i == name_start)
        {
            throw std::runtime_error("Invalid path: empty segment");
        }

        segment.name = path.substr(name_start, i - name_start);
        segment.index = 0;

        if (i < path.size() && path[i] == '[')
        {
            std::size_t index_value = 0;

            ++i;

            if (i >= path.size() || !std::isdigit(static_cast<unsigned char>(path[i])))
            {
                throw std::runtime_error("Invalid path: expected array index");
            }

            while (i < path.size() && std::isdigit(static_cast<unsigned char>(path[i])))
            {
                index_value = (index_value * 10) + static_cast<std::size_t>(path[i] - '0');
                ++i;
            }

            if (i >= path.size() || path[i] != ']')
            {
                throw std::runtime_error("Invalid path: missing closing bracket");
            }

            segment.index = index_value;
            ++i;
        }

        segments.push_back(std::move(segment));

        if (i < path.size())
        {
            if (path[i] != '.')
            {
                throw std::runtime_error("Invalid path: expected dot separator");
            }

            ++i;

            if (i >= path.size())
            {
                throw std::runtime_error("Invalid path: trailing dot");
            }
        }
    }

    return POLYCONF::PATH(std::move(segments));
}

//
//!\brief Resolve a parsed path against a root node
//
const POLYCONF::NODE* POLYCONF::resolve_path(const POLYCONF::NODE& root, const POLYCONF::PATH& path)
{
    const POLYCONF::NODE* current = &root;
    const std::vector<POLYCONF::PATH_SEGMENT>& segments = path.segments();
    std::size_t i;

    for (i = 0; i < segments.size(); ++i)
    {
        current = current->get_child(segments[i].name, segments[i].index);

        if (current == nullptr)
        {
            return nullptr;
        }
    }

    return current;
}

//
//!\brief Resolve a raw path string against a root node
//
const POLYCONF::NODE* POLYCONF::resolve_path(const POLYCONF::NODE& root, const std::string& path)
{
    return POLYCONF::resolve_path(root, POLYCONF::parse_path(path));
}