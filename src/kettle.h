#pragma once

#include "kettle_cfg.h"

typedef struct ktl_State ktl_State;

typedef enum ktl_Type
{
    KTL_NIL,
    KTL_BOOLEAN,
    KTL_INT,
    KTL_FLOAT,
    KTL_STRING,
    KTL_ARRAY,
    KTL_MAP,
    KTL_OBJECT,
    KTL_FUNCTION,
    KTL_USERDATA
} ktl_Type;

/**
 @brief Creates a new state object

 Returns 0 if not enough memory
 */
ktl_State *ktl_State_new();

/**
 @brief Destroys the state object and all associated values
 */
void ktl_State_del(ktl_State *state);

/**
 @brief Get the size of the vstack
 @returns the number of elements on vstack
 */
unsigned ktl_top(ktl_State *ktl);

/**
 @brief Remove top element from vstack
 
 Has no effect if vstack is empty
 */
void ktl_pop(ktl_State *ktl);

/// @brief Remove all values from vstack
void ktl_clear_vstack(ktl_State *ktl);

/// @brief Push `nil` to vstack
void ktl_push_nil(ktl_State *ktl);

/// @brief Push a boolean value to vstack
void ktl_push_boolean(ktl_State *ktl, char value);
