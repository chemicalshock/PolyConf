// -------------------------------------------------------------
//
//!\file xml_writer.hpp
//!\brief XML writer for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_XML_WRITER_HPP
#define POLYCONF_XML_WRITER_HPP

// inbuilt
#include <string>

// internal
#include "config.hpp"

namespace POLYCONF
{
    //!\brief Serialise configuration to XML string
    std::string write_xml(const CONFIG& config);
}

#endif // POLYCONF_XML_WRITER_HPP
