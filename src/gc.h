#pragma once

#include "kettle.h"
#include <stddef.h>

typedef enum ktl_GCState
{
    KTL_GC_RUNNING,
    KTL_GC_PAUSE
} ktl_GCState;

typedef struct ktl_GCHeader
{
    struct ktl_GCHeader *next;
    ktl_Type type;
    char mark;
} ktl_GCHeader;

/// @brief Collect garbage if the threshold is reached
void ktl_gc_check(ktl_State *ktl);

/// @brief Collect garbage unconditionally
void ktl_gc(ktl_State *ktl);

/**
 @brief Prepare an object for garbage collection by calling the `__gc` method
 */
void ktl_Object_gc(ktl_State *ktl, ktl_GCHeader *_obj);

/// @brief Delete the object freeing all alocated memory
void ktl_Object_del(ktl_State *ktl, ktl_GCHeader *_obj);

/// @brief Delete a string object
void ktl_String_del(ktl_State *ktl, ktl_GCHeader *_obj);
