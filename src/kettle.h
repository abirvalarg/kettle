#pragma once

typedef struct ktl_State ktl_State;

typedef enum ktl_Type
{
    KTL_NIL,
    KTL_BOOL,
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
