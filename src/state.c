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
        ktl_StrMap_init(&ktl->global, 0);
        ktl->context = 0;
        ktl->on_mem_err = 0;
    }
    return ktl;
}

void ktl_State_del(ktl_State *ktl)
{
    // remove all contexts
    while(ktl->context)
        ktl_pop_context(ktl);
    
    // 1st GC pass
    ktl_gc(ktl);

    // delete all globals
    ktl_StrMap_deinit(&ktl->global);

    // 2nd GC pass
    ktl_gc(ktl);

    // clean up the leftovers
    while(ktl->gc_head)
    {
        ktl_GCHeader *next = ktl->gc_head->next;
        free(ktl->gc_head);
        ktl->gc_head = next;
    }

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

void ktl_pop_context(ktl_State *ktl)
{
    if(ktl->context)
    {
        // clear local variables
        for(unsigned i = 0; i < ktl->context->num_locals; i++)
        {
            free(ktl->context->local_names[i]);
            ktl_Var_del(ktl->context->locals[i]);
        }
        free(ktl->context->locals);
        free(ktl->context->local_names);

        // clear and restore vstack
        ktl_clear_vstack(ktl);
        free(ktl->vstack);
        ktl->vstack = ktl->context->next_vstack;
        ktl->vstack_size = ktl->context->next_vstack_size;

        // pop the context
        ktl_Context *next = ktl->context->next;
        free(ktl->context);
        ktl->context = next;
    }
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
