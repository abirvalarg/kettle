#pragma once

#include "value.h"
#include <stddef.h>

typedef struct ktl_StrMapNode
{
    size_t hash;
    struct ktl_StrMapNode *next;
    char *key;
    ktl_Value value;
} ktl_StrMapNode;

/// @brief map with string keys
typedef struct ktl_StrMap
{
    ktl_StrMapNode **buckits;
    size_t num_buckits;
    size_t num_nodes;
} ktl_StrMap;

typedef struct ktl_StrMapIter
{
    ktl_StrMap *m;
    size_t buckit;
    ktl_StrMapNode *cur;
} ktl_StrMapIter;

/// @brief Initialize a map
void ktl_StrMap_init(ktl_StrMap *m, size_t capacity);

/// @brief Deinitialize a map freeing all alocated resources
void ktl_StrMap_deinit(ktl_StrMap *m);

/**
 @brief Set a value associated with the given key
 @returns the pointer to the value that belongs to the map
    or 0 if not enough memory
 */
ktl_Value *ktl_StrMap_set(ktl_StrMap *m, const char *const key, ktl_Value value);

/**
 @brief Get a value from the map
 @returns the value associated with the given key or `nil` if no value found
 */
ktl_Value ktl_StrMap_get(ktl_StrMap *m, const char *const key);

/**
 @brief Remove a pair from the map

 Has no effect if the key does not exist
 */
void ktl_StrMap_remove(ktl_StrMap *m, const char *const key);

/// @brief Creates a new iterator over a map
ktl_StrMapIter ktl_StrMap_iter(ktl_StrMap *m);

/**
 @brief Returns the next node that iterator is pointing to
 @returns 0 if no nodes left in the map
 */
ktl_StrMapNode *ktl_StrMap_next(ktl_StrMapIter *iter);
