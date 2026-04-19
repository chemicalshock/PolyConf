// -------------------------------------------------------------
//
//!\file writer.cpp
//!\brief Configuration writing implementation for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// own header
#include "writer.hpp"

// inbuilt
#include <fstream>

// internal
#include "parser/acf_writer.hpp"

//
//!\brief Save configuration to string
//
std::string POLYCONF::save_string(const POLYCONF::CONFIG& config, POLYCONF::FORMAT format)
{
    switch (format)
    {
        case POLYCONF::FORMAT::ACF:
            return POLYCONF::write_acf(config);

        default:
            return std::string();
    }
}

//
//!\brief Save configuration to file
//
bool POLYCONF::save_file(const POLYCONF::CONFIG& config, const std::string& file, POLYCONF::FORMAT format)
{
    std::ofstream stream(file.c_str());
    std::string output;

    if (!stream)
    {
        return false;
    }

    output = POLYCONF::save_string(config, format);
    stream << output;

    return stream.good();
}