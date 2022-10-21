#pragma once

#include "kettle.h"
#include "gc.h"
#include "value.h"

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

struct ktl_State
{
    ktl_GCHeader *gc_head;
    size_t mem_usage;
    size_t gc_threshold;
    ktl_GCState gc_state;
    ktl_VStackNode *vstack;
    unsigned vstack_size;
};

void ktl_push_anon_value(ktl_State *ktl, ktl_Value value);
