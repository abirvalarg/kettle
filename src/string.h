#pragma once

#include "kettle.h"
#include "gc.h"

typedef struct ktl_String
{
    ktl_GCHeader gc;
    char *content;
    size_t len;
} ktl_String;

/// @brief Create a new string object with given content
ktl_String *ktl_String_new(ktl_State *ktl, const char *const content, size_t len);
