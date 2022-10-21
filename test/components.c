#include "test.h"
#include "../src/strmap.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

static Result _strmap(size_t capacity)
{
    ktl_StrMap map;
    ktl_StrMap_init(&map, capacity);

    ASSERT(ktl_StrMap_set(&map, "aaa", (ktl_Value){
        .type = KTL_INT,
        .value = { .as_int = 42 }
    }), "out of memory");
    ASSERT(ktl_StrMap_set(&map, "bbb", (ktl_Value){
        .type = KTL_INT,
        .value = { .as_int = 2 }
    }), "out of memory");
    ASSERT(ktl_StrMap_set(&map, "ccc", (ktl_Value){
        .type = KTL_INT,
        .value = { .as_int = 3 }
    }), "out of memory");
    ASSERT(ktl_StrMap_set(&map, "aaa", (ktl_Value){
        .type = KTL_INT,
        .value = { .as_int = 1 }
    }), "out of memory");

    ktl_Value res = ktl_StrMap_get(&map, "aaa");
    ASSERT(res.type == KTL_INT && res.value.as_int == 1, "Bad result for key `aaa`");
    res = ktl_StrMap_get(&map, "bbb");
    ASSERT(res.type == KTL_INT && res.value.as_int == 2, "Bad result for key `bbb`");
    res = ktl_StrMap_get(&map, "ccc");
    ASSERT(res.type == KTL_INT && res.value.as_int == 3, "Bad result for key `ccc`");

    char saw_aaa = 0,
        saw_bbb = 0,
        saw_ccc = 0;
    
    ktl_StrMapIter iter = ktl_StrMap_iter(&map);
    ktl_StrMapNode *node;

    while((node = ktl_StrMap_next(&iter)))
    {
        if(!strcmp(node->key, "aaa"))
            saw_aaa = 1;
        else if(!strcmp(node->key, "bbb"))
            saw_bbb = 1;
        else if(!strcmp(node->key, "ccc"))
            saw_ccc = 1;
        else
        {
            char *buffer = malloc(1024);
            sprintf(buffer, "Found unexpected key '%s'", node->key);
            return (Result){
                .code = 1,
                .message = buffer,
                .dyn_msg = 1
            };
        }
    }

    ktl_StrMap_deinit(&map);

    ASSERT(saw_aaa && saw_bbb && saw_ccc, "Didn't find some key in map");

    return OK;
}

Result strmap()
{
    return _strmap(0);
}


Result strmap_with_capacity()
{
    return _strmap(3);
}
