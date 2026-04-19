// -------------------------------------------------------------
//
//!\file path.hpp
//!\brief Path parsing and resolution for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_PATH_HPP
#define POLYCONF_PATH_HPP

// inbuilt
#include <cstddef>
#include <string>
#include <vector>

// internal
#include "node.hpp"

namespace POLYCONF
{
    struct PATH_SEGMENT
    {
        std::string name;
        std::size_t index = 0;
    };

    class PATH final
    {
    private:
        std::vector<PATH_SEGMENT> m_segments;

    public:
        //!\brief Default constructor
        PATH() = default;

        //!\brief Construct from parsed segments
        explicit PATH(const std::vector<PATH_SEGMENT>& segments);

        //!\brief Construct from parsed segments (move)
        explicit PATH(std::vector<PATH_SEGMENT>&& segments);

        //!\brief Get path segments
        const std::vector<PATH_SEGMENT>& segments() const;

        //!\brief Check whether path is empty
        bool empty() const;
    };

    //!\brief Parse a path string
    PATH parse_path(const std::string& path);

    //!\brief Resolve a parsed path against a root node
    const NODE* resolve_path(const NODE& root, const PATH& path);

    //!\brief Resolve a raw path string against a root node
    const NODE* resolve_path(const NODE& root, const std::string& path);
}

#endif // POLYCONF_PATH_HPP