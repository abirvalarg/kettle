#include "state.h"
#include "kettle.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

ktl_State *ktl_State_new()
{
    ktl_State *ktl = malloc(sizeof(ktl_State));
    if(ktl)
    {
        ktl->gc_head = 0;
        ktl->mem_usage = 0;
        ktl->gc_threshold = KTL_INIT_GC_THRESHOLD;
        ktl->vstack = malloc(sizeof(ktl_VStackNode) * KTL_VSTACK_CAPACITY);
        if(!ktl->vstack)
        {
            free(ktl);
            return 0;
        }
        ktl->vstack_size = 0;
    }
    return ktl;
}

void ktl_State_del(ktl_State *ktl)
{
    free(ktl);
}

unsigned ktl_top(ktl_State *ktl)
{
    return ktl->vstack_size;
}

void ktl_pop(ktl_State *ktl)
{
    if(ktl->vstack_size)
    {
        if(ktl->vstack[ktl->vstack_size - 1].name)
            free(ktl->vstack[ktl->vstack_size - 1].name);
        ktl->vstack_size--;
    }
}

void ktl_clear_vstack(ktl_State *ktl)
{
    for(unsigned i = 0; i < ktl->vstack_size; i++)
        if(ktl->vstack[i].name)
            free(ktl->vstack[i].name);
    ktl->vstack_size = 0;
}

void ktl_push_nil(ktl_State *ktl)
{
    ktl_push_anon_value(ktl, (ktl_Value){
        .type = KTL_NIL,
        .value = { .as_int = 0 }
    });
}

void ktl_push_boolean(ktl_State *ktl, char value)
{
    ktl_push_anon_value(ktl, (ktl_Value){
        .type = KTL_BOOLEAN,
        .value = { .as_int = value }
    });
}

void ktl_push_anon_value(ktl_State *ktl, ktl_Value value)
{
    if(ktl->vstack_size == KTL_VSTACK_CAPACITY)
    {
        ktl_clear_vstack(ktl);
        fprintf(stderr, "vstack overflow\n");
        abort();
    }

    ktl->vstack[ktl->vstack_size++] = (ktl_VStackNode){
        .value = value,
        .kind = KTL_ANON,
        .name = 0
    };
}
