#pragma once

#include "kettle.h"
#include "config.h"

/// @brief Represents a single Kettle value with type attached to it
typedef struct ktl_Value
{
    union {
        ktl_Int as_int;
        ktl_Float as_float;
    } value;
    ktl_Type type;
} ktl_Value;
