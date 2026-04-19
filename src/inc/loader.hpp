// -------------------------------------------------------------
//
//!\file loader.hpp
//!\brief Internal loader interface for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_LOADER_HPP
#define POLYCONF_LOADER_HPP

// inbuilt
#include <string>

// internal
#include "config.hpp"
#include "types.hpp"

namespace POLYCONF
{
    //!\brief Load configuration from string
    CONFIG load_string(const std::string& input, FORMAT format);

    //!\brief Load configuration from file
    CONFIG load_file(const std::string& file, FORMAT format);
}

#endif // POLYCONF_LOADER_HPP