// -------------------------------------------------------------
//
//!\file utils.hpp
//!\brief Test utilities for Polyconf unit tests
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_TEST_UTILS_HPP
#define POLYCONF_TEST_UTILS_HPP

// inbuilt
#include <fstream>
#include <sstream>
#include <string>

//!\brief Read full file contents into a string
inline std::string read_file_text(const std::string& file)
{
    std::ifstream stream(file.c_str());
    std::stringstream buffer;

    if (!stream)
    {
        return std::string();
    }

    buffer << stream.rdbuf();
    return buffer.str();
}

//!\brief Normalise line endings to '\n'
inline std::string normalise_text(const std::string& input)
{
    std::string output;
    std::size_t i = 0;

    output.reserve(input.size());

    while (i < input.size())
    {
        if (input[i] == '\r')
        {
            // skip CR, let LF handle newline
            ++i;
            continue;
        }

        output.push_back(input[i]);
        ++i;
    }

    return output;
}

//!\brief Compare two text blocks after normalisation
inline bool text_equal_normalised(const std::string& a, const std::string& b)
{
    return normalise_text(a) == normalise_text(b);
}

//!\brief Compare file contents to reference file
inline bool file_matches_ref(const std::string& file, const std::string& ref)
{
    std::string lhs = read_file_text(file);
    std::string rhs = read_file_text(ref);

    return text_equal_normalised(lhs, rhs);
}

//!\brief Compare string output to reference file
inline bool string_matches_ref(const std::string& text, const std::string& ref)
{
    std::string rhs = read_file_text(ref);

    return text_equal_normalised(text, rhs);
}

#endif // POLYCONF_TEST_UTILS_HPP