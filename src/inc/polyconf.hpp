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

namespace POLYCONF
{
    //!\brief Load configuration from string
    CONFIG load_string(const std::string& input, FORMAT format);

    //!\brief Load configuration from file
    CONFIG load_file(const std::string& file, FORMAT format);
}

#endif // POLYCONF_HPP