// -------------------------------------------------------------
//
//!\file writer.hpp
//!\brief Configuration writing interface for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_WRITER_HPP
#define POLYCONF_WRITER_HPP

// inbuilt
#include <string>

// internal
#include "config.hpp"
#include "types.hpp"

namespace POLYCONF
{
    //!\brief Save configuration to string
    std::string save_string(const CONFIG& config, FORMAT format);

    //!\brief Save configuration to file
    bool save_file(const CONFIG& config, const std::string& file, FORMAT format);
}

#endif // POLYCONF_WRITER_HPP