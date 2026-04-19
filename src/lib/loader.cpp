// -------------------------------------------------------------
//
//!\file loader.cpp
//!\brief Configuration loading implementation for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// own header
#include "loader.hpp"

// inbuilt
#include <fstream>
#include <sstream>
#include <utility>

// internal
#include "parser/acf_parser.hpp"

//
//!\brief Load configuration from string
//
POLYCONF::CONFIG POLYCONF::load_string(const std::string& input, POLYCONF::FORMAT format)
{
    switch (format)
    {
        case POLYCONF::FORMAT::ACF:
            return POLYCONF::parse_acf(input);

        default:
            return POLYCONF::CONFIG();
    }
}

//
//!\brief Load configuration from file
//
POLYCONF::CONFIG POLYCONF::load_file(const std::string& file, POLYCONF::FORMAT format)
{
    std::ifstream stream(file.c_str());
    std::stringstream buffer;

    if (!stream)
    {
        return POLYCONF::CONFIG();
    }

    buffer << stream.rdbuf();

    return POLYCONF::load_string(buffer.str(), format);
}