// -------------------------------------------------------------
//
//!\file types.hpp
//!\brief Core shared types for PolyConf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_TYPES_HPP
#define POLYCONF_TYPES_HPP

#include <cstdint>

namespace POLYCONF
{
    // ---------------------------------------------------------
    //!\brief Supported configuration formats
    // ---------------------------------------------------------
    enum class FORMAT : uint8_t
    {
        AUTO = 0,
        ACF,
        //INI,
        //XML,
        //JSON,
        //YAML
    };
}

#endif // POLYCONF_TYPES_HPP