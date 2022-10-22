#include "gc.h"
#include "kettle.h"
#include "state.h"
#include "strmap.h"
#include <malloc.h>
#include <stdlib.h>

void ktl_gc_check(ktl_State *ktl)
{
    if(ktl->gc_state == KTL_GC_RUNNING && ktl->mem_usage >= ktl->gc_threshold)
        ktl_gc(ktl);
}

void ktl_gc(ktl_State *ktl)
{
    // clear all marks
    ktl_GCHeader *cur = ktl->gc_head;
    while(cur)
    {
        cur->mark = 0;
        cur = cur->next;
    }

    // mark vstack
    for(unsigned i = 0; i < ktl->vstack_size; i++)
        ktl_Value_mark(ktl->vstack[i].value);

    // mark globals
    ktl_StrMapIter iter = ktl_StrMap_iter(&ktl->global);
    ktl_StrMapNode *map_node;
    while((map_node = ktl_StrMap_next(&iter)))
        ktl_Value_mark(map_node->value);
    
    // mark contexts
    ktl_Context *ctx = ktl->context;
    while(ctx)
    {
        // vstack
        for(unsigned i = 0; i < ctx->next_vstack_size; i++)
            ktl_Value_mark(ctx->next_vstack[i].value);
        
        // locals
        for(unsigned i = 0; i < ctx->num_locals; i++)
            ktl_Value_mark(ctx->locals[i]->value);

        ctx = ctx->next;
    }

    cur = ktl->gc_head;
    while(cur)
    {
        if(!cur->mark)
        {
            switch(cur->type)
            {
            case KTL_OBJECT:
                ktl_Object_gc(cur);
                break;
            
            default:
                fprintf(stderr, "Attempt to GC an unsupported type\n");
                abort();
            }
        }
    }

    // sweep
    cur = ktl->gc_head;
    ktl_GCHeader **last_slot = &ktl->gc_head;
    while(cur)
    {
        if(!cur->mark)
        {
            ktl_GCHeader *next = *last_slot = cur->next;
            switch(cur->type)
            {
            case KTL_OBJECT:
                ktl_Object_del(cur);
                break;
            
            default:
                fprintf(stderr, "Attempt to GC an unsupported type\n");
                abort();
            }
            cur = next;
        }
        else
            cur = cur->next;
    }
}
