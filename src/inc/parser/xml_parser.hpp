// -------------------------------------------------------------
//
//!\file xml_parser.hpp
//!\brief XML parser for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_XML_PARSER_HPP
#define POLYCONF_XML_PARSER_HPP

// inbuilt
#include <string>

// internal
#include "config.hpp"

namespace POLYCONF
{
    //!\brief Parse XML input into a configuration object
    CONFIG parse_xml(const std::string& input);
}

#endif // POLYCONF_XML_PARSER_HPP