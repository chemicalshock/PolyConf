// -------------------------------------------------------------
//
//!\file acf_parser.hpp
//!\brief ACF parser for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_ACF_PARSER_HPP
#define POLYCONF_ACF_PARSER_HPP

// inbuilt
#include <string>

// internal
#include "config.hpp"

namespace POLYCONF
{
    //!\brief Parse ACF input into a configuration object
    CONFIG parse_acf(const std::string& input);
}

#endif // POLYCONF_ACF_PARSER_HPP