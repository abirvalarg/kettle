#pragma once

typedef struct ktl_State ktl_State;

/**
 @brief Creates a new state object

 Returns 0 if not enough memory
 */
ktl_State *ktl_State_new();

/**
 @brief Destroys the state object and all associated values
 */
void ktl_State_del(ktl_State *state);
