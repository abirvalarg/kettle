#pragma once

#include "kettle.h"
#include "gc.h"
#include <stddef.h>

/// @brief Represents a single Kettle value with type attached to it
typedef struct ktl_Value
{
    union {
        ktl_Int as_int;
        ktl_Float as_float;
        ktl_GCHeader *as_obj;
    } value;
    ktl_Type type;
} ktl_Value;

/// @brief Represents a variable with reference counter
typedef struct ktl_Var
{
    ktl_Value value;
    size_t rc;
} ktl_Var;

/// @brief Creates a new variable with rc=1
ktl_Var *ktl_Var_new(ktl_Value value);

/// @brief Decrements rc of a variable and deletes it if rc=0
void ktl_Var_del(ktl_Var *var);

/**
 @brief Mark a GC object

 Has no effect on non-GC values
 */
void ktl_Value_mark(ktl_Value value);
