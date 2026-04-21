// -------------------------------------------------------------
//
//!\file polyconf.hpp
//!\brief Public API for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_HPP
#define POLYCONF_HPP

// inbuilt
#include <string>

// internal
#include "types.hpp"
#include "config.hpp"
#include "writer.hpp"

namespace POLYCONF
{
    //!\brief Load configuration from string
    CONFIG load_string(const std::string& input, FORMAT format);

    //!\brief Load configuration from file
    CONFIG load_file(const std::string& file, FORMAT format);

    //!\brief Save configuration to string
    std::string save_string(const CONFIG& config, FORMAT format);

    //!\brief Save configuration to file
    bool save_file(const CONFIG& config, const std::string& file, FORMAT format);

    //!\brief Merge multiple configuration strings into one
    CONFIG merge_string(const std::string& input, FORMAT format);

    //!\brief Merge multiple configuration files into one
    CONFIG merge_file(const std::string& file, FORMAT format);
}

#endif // POLYCONF_HPP
