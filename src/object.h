#pragma once

#include "kettle.h"
#include "gc.h"
#include "strmap.h"

typedef struct ktl_Object
{
    ktl_GCHeader gc;
    ktl_StrMap map;
} ktl_Object;

/**
 @brief Create a new object

 Always returns a valid pointer. Raises memory error is not enough memory
 */
ktl_Object *ktl_Object_new(ktl_State *ktl);

/**
 @brief Create a new object with the given capacity

 @param capacity number of elements that will be inserted later

 Always returns a valid pointer. Raises memory error is not enough memory
 */
ktl_Object *ktl_Object_with_capacity(ktl_State *ktl, size_t capacity);
