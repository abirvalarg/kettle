#include "object.h"
#include "gc.h"
#include "state.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

// ktl_Object *ktl_Object_new(ktl_State *ktl)
// {
//     ktl_gc_check(ktl);
//     ktl_Object *obj = malloc(sizeof(ktl_Object));
//     if(!obj)
//         ktl_mem_err(ktl);
//     obj->gc.next = ktl->gc_head;
//     ktl->gc_head = (ktl_GCHeader*)obj;
//     obj->gc.type = KTL_OBJECT;
//     ktl_StrMap_init(&obj->map, 0);
//     ktl->mem_usage += sizeof(ktl_Object);
//     return obj;
// }

ktl_Object *ktl_Object_with_capacity(ktl_State *ktl, size_t capacity)
{
    ktl_gc_check(ktl);
    ktl_Object *obj = malloc(sizeof(ktl_Object));
    if(!obj)
        ktl_mem_err(ktl);
    obj->gc.next = ktl->gc_head;
    ktl->gc_head = (ktl_GCHeader*)obj;
    obj->gc.type = KTL_OBJECT;
    ktl_StrMap_init(&obj->map, capacity);
    ktl->mem_usage += sizeof(ktl_Object);
    return obj;
}

void ktl_Object_gc(ktl_State *ktl, ktl_GCHeader *_obj)
{
    ktl_Object *obj = (ktl_Object*)_obj;
    ktl_Value gc_func = ktl_StrMap_get(&obj->map, "__gc");
    if(gc_func.type == KTL_FUNCTION)
    {
        fprintf(stderr, "__gc method isn't supported yet\n");
        abort();
    }
}

void ktl_Object_del(ktl_State *ktl, ktl_GCHeader *_obj)
{
    ktl_Object *obj = (ktl_Object*)_obj;
    ktl_StrMap_deinit(&obj->map);
    free(obj);
    ktl->mem_usage -= sizeof(ktl_Object);
}
