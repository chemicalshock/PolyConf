// -------------------------------------------------------------
//
//!\file config.cpp
//!\brief Main configuration object implementation for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

// own header
#include "config.hpp"

// inbuilt
#include <cstdlib>
#include <utility>

// internal
#include "path.hpp"

//
//!\brief Default constructor
//
POLYCONF::CONFIG::CONFIG()
{
}

//
//!\brief Construct from root node
//
POLYCONF::CONFIG::CONFIG(const POLYCONF::NODE& root)
    : m_root(root)
{
}

//
//!\brief Construct from root node (move)
//
POLYCONF::CONFIG::CONFIG(POLYCONF::NODE&& root)
    : m_root(std::move(root))
{
}

//
//!\brief Get integer value by path
//
int POLYCONF::CONFIG::get_int(const std::string& path, int default_value) const
{
    const POLYCONF::NODE* node;
    const std::string* value;
    char* end = nullptr;
    long result;

    try
    {
        node = POLYCONF::resolve_path(m_root, path);
    }
    catch (...)
    {
        return default_value;
    }

    if (node == nullptr || !node->has_value())
    {
        return default_value;
    }

    value = &(node->value());

    if (value->empty())
    {
        return default_value;
    }

    result = std::strtol(value->c_str(), &end, 10);

    if (end == value->c_str() || *end != '\0')
    {
        return default_value;
    }

    return static_cast<int>(result);
}

//
//!\brief Get floating-point value by path
//
double POLYCONF::CONFIG::get_float(const std::string& path, double default_value) const
{
    const POLYCONF::NODE* node;
    const std::string* value;
    char* end = nullptr;
    double result;

    try
    {
        node = POLYCONF::resolve_path(m_root, path);
    }
    catch (...)
    {
        return default_value;
    }

    if (node == nullptr || !node->has_value())
    {
        return default_value;
    }

    value = &(node->value());

    if (value->empty())
    {
        return default_value;
    }

    result = std::strtod(value->c_str(), &end);

    if (end == value->c_str() || *end != '\0')
    {
        return default_value;
    }

    return result;
}

//
//!\brief Get boolean value by path
//
bool POLYCONF::CONFIG::get_bool(const std::string& path, bool default_value) const
{
    const POLYCONF::NODE* node;
    const std::string* value;

    try
    {
        node = POLYCONF::resolve_path(m_root, path);
    }
    catch (...)
    {
        return default_value;
    }

    if (node == nullptr || !node->has_value())
    {
        return default_value;
    }

    value = &(node->value());

    if (*value == "true" || *value == "1" || *value == "yes" || *value == "on")
    {
        return true;
    }

    if (*value == "false" || *value == "0" || *value == "no" || *value == "off")
    {
        return false;
    }

    return default_value;
}

//
//!\brief Get string value by path
//
std::string POLYCONF::CONFIG::get_string(const std::string& path, const std::string& default_value) const
{
    const POLYCONF::NODE* node;

    try
    {
        node = POLYCONF::resolve_path(m_root, path);
    }
    catch (...)
    {
        return default_value;
    }

    if (node == nullptr)
    {
        return default_value;
    }

    if (!node->has_value())
    {
        return default_value;
    }

    return node->value();
}

//
//!\brief Check whether path exists
//
bool POLYCONF::CONFIG::has(const std::string& path) const
{
    const POLYCONF::NODE* node;

    try
    {
        node = POLYCONF::resolve_path(m_root, path);
    }
    catch (...)
    {
        return false;
    }

    return (node != nullptr);
}

//
//!\brief Get root node
//
const POLYCONF::NODE& POLYCONF::CONFIG::root() const
{
    return m_root;
}