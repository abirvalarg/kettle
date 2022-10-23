#include "string.h"
#include "gc.h"
#include "state.h"
#include <malloc.h>
#include <string.h>

ktl_String *ktl_String_new(ktl_State *ktl, const char *const content, size_t len)
{
    ktl_gc_check(ktl);
    ktl_String *str = malloc(sizeof(ktl_String));
    if(!str)
        ktl_mem_err(ktl);
    str->gc.next = ktl->gc_head;
    ktl->gc_head = (ktl_GCHeader*)str;
    str->gc.type = KTL_STRING;
    ktl->mem_usage += sizeof(ktl_String);
    str->content = len ? malloc(len) : 0;
    if(len && !str->content)
        ktl_mem_err(ktl);
    ktl->mem_usage += len;
    memcpy(str->content, content, len);
    str->len = len;
    return str;
}

void ktl_String_del(ktl_State *ktl, ktl_GCHeader *_obj)
{
    ktl_String *str = (ktl_String*)_obj;
    if(str->content)
        free(str->content);
    ktl->mem_usage -= str->len + sizeof(ktl_String);
    free(str);
}
