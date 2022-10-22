#pragma once

#include "kettle_cfg.h"
#include <stddef.h>

typedef struct ktl_State ktl_State;
typedef void (*ktl_CFunction)(ktl_State *ktl);

typedef enum ktl_Type
{
    KTL_NIL,
    KTL_BOOLEAN,
    KTL_INT,
    KTL_FLOAT,
    KTL_STRING,
    KTL_TABLE,
    KTL_OBJECT,
    KTL_FUNCTION,
    KTL_USERDATA
} ktl_Type;

typedef enum ktl_StdErr
{
    KTL_ERR_STACK,
    KTL_ERR_OVERFLOW
} ktl_StdErr;

extern const char *const KTL_ERR_CODES[2];
extern const char *const KTL_ERR_MSGS[2];

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

/// @brief Get type of value at given index
ktl_Type ktl_get_type(ktl_State *ktl, int idx);


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

/// @brief Create a new object with given capacity and push it on vstack
void ktl_create_object(ktl_State *ktl, size_t capacity);

/// @brief Create a new error object and push it on stack
void ktl_push_std_err(ktl_State *ktl, ktl_StdErr err);


/**
 @brief Set a new memory error handler
 @returns old error handler
 */
ktl_CFunction ktl_on_mem_err(ktl_State *ktl, ktl_CFunction handler);

/**
 @brief Set a new error handler
 @returns old error handler
 */
ktl_CFunction ktl_on_err(ktl_State *ktl, ktl_CFunction handler);


/// @brief Raise a memory error
__attribute__((noreturn))
void ktl_mem_err(ktl_State *ktl);

/// @brief Raise an error using the top value on vstack as error object
__attribute__((noreturn))
void ktl_err(ktl_State *ktl);
