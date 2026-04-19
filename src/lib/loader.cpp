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
#include <cctype>
#include <fstream>
#include <sstream>

// internal
#include "parser/acf_parser.hpp"
#include "parser/xml_parser.hpp"


namespace
{
    //
    //!\brief Detect configuration format from file extension
    //
    POLYCONF::FORMAT detect_format_from_extension(const std::string& file)
    {
        std::size_t pos = file.find_last_of('.');

        if (pos == std::string::npos)
        {
            return POLYCONF::FORMAT::AUTO;
        }

        std::string ext = file.substr(pos + 1);
        std::string lower;
        std::size_t i;

        lower.reserve(ext.size());

        for (i = 0; i < ext.size(); ++i)
        {
            lower.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ext[i]))));
        }

        if (lower == "xml")
        {
            return POLYCONF::FORMAT::XML;
        }

        if (lower == "acf" || lower == "conf" || lower == "cfg")
        {
            return POLYCONF::FORMAT::ACF;
        }

        return POLYCONF::FORMAT::AUTO;
    }

    //
    //!\brief Detect configuration format from input
    //
    POLYCONF::FORMAT detect_format(const std::string& input)
    {
        std::size_t i = 0;

        while (i < input.size() && std::isspace(static_cast<unsigned char>(input[i])))
        {
            ++i;
        }

        if (i >= input.size())
        {
            return POLYCONF::FORMAT::ACF;
        }

        if (input[i] == '<')
        {
            return POLYCONF::FORMAT::XML;
        }

        if (input[i] == '.')
        {
            return POLYCONF::FORMAT::ACF;
        }

        return POLYCONF::FORMAT::ACF;
    }
}

//
//!\brief Load configuration from string
//
POLYCONF::CONFIG POLYCONF::load_string(const std::string& input, POLYCONF::FORMAT format)
{
    POLYCONF::FORMAT detected_format = format;

    if (detected_format == POLYCONF::FORMAT::AUTO)
    {
        detected_format = ::detect_format(input);
    }

    switch (detected_format)
    {
        case POLYCONF::FORMAT::ACF:
            return POLYCONF::parse_acf(input);

        case POLYCONF::FORMAT::XML:
            return POLYCONF::parse_xml(input);

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
    POLYCONF::FORMAT detected_format = format;

    if (!stream)
    {
        return POLYCONF::CONFIG();
    }

    if (detected_format == POLYCONF::FORMAT::AUTO)
    {
        detected_format = ::detect_format_from_extension(file);
    }

    buffer << stream.rdbuf();

    if (detected_format == POLYCONF::FORMAT::AUTO)
    {
        return POLYCONF::load_string(buffer.str(), POLYCONF::FORMAT::AUTO);
    }

    return POLYCONF::load_string(buffer.str(), detected_format);
}