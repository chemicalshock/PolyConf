// -------------------------------------------------------------
//
//!\file acf_writer.hpp
//!\brief ACF writer for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_ACF_WRITER_HPP
#define POLYCONF_ACF_WRITER_HPP

// inbuilt
#include <string>

// internal
#include "config.hpp"

namespace POLYCONF
{
    //!\brief Serialise configuration to ACF string
    std::string write_acf(const CONFIG& config);
}

#endif // POLYCONF_ACF_WRITER_HPP