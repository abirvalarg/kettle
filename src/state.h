#pragma once

#include "kettle.h"
#include "gc.h"
#include "value.h"
#include "strmap.h"

typedef enum ktl_VarKind
{
    KTL_GLOBAL,
    KTL_LOCAL,
    KTL_ANON
} ktl_VarKind;

typedef struct ktl_VStackNode
{
    ktl_Value value;
    char *name;
    ktl_VarKind kind;
} ktl_VStackNode;

typedef struct ktl_Context
{
    struct ktl_Context *next;
    ktl_VStackNode *next_vstack;
    char **local_names;
    ktl_Var **locals;
    unsigned next_vstack_size;
    unsigned short num_locals;
    unsigned short locals_capacity;
} ktl_Context;

struct ktl_State
{
    ktl_GCHeader *gc_head;
    size_t mem_usage;
    size_t gc_threshold;
    ktl_VStackNode *vstack;
    ktl_Context *context;
    ktl_CFunction on_mem_err;
    ktl_CFunction on_err;
    ktl_StrMap global;
    unsigned vstack_size;
    ktl_GCState gc_state;
};

/// @brief Free current context setting next one as current
void ktl_pop_context(ktl_State *ktl);

/// @brief Push a value on vstack
void ktl_push_anon_value(ktl_State *ktl, ktl_Value value);

/// @brief Get a value from vstack
ktl_Value ktl_get_value(ktl_State *ktl, int idx);
