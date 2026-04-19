// -------------------------------------------------------------
//
// !\file config.hpp
// !\brief Main configuration object for Polyconf
// !\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_CONFIG_HPP
#define POLYCONF_CONFIG_HPP

// inbuilt
#include <string>

// internal
#include "types.hpp"
#include "node.hpp"

namespace POLYCONF
{
    class CONFIG final
    {
    private:
        NODE m_root;

    public:
        // !\brief Default constructor
        CONFIG();

        // !\brief Construct from root node
        explicit CONFIG(const NODE& root);

        // !\brief Construct from root node (move)
        explicit CONFIG(NODE&& root);

        // !\brief Get integer value by path
        int get_int(const std::string& path, int default_value) const;

        // !\brief Get floating-point value by path
        double get_float(const std::string& path, double default_value) const;

        // !\brief Get boolean value by path
        bool get_bool(const std::string& path, bool default_value) const;

        // !\brief Get string value by path
        std::string get_string(const std::string& path, const std::string& default_value) const;

        // !\brief Check if a path exists
        bool has(const std::string& path) const;

        // !\brief Access root node
        const NODE& root() const;
    };
}

#endif // POLYCONF_CONFIG_HPP