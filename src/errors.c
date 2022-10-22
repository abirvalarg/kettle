#include "kettle.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>

ktl_CFunction ktl_on_mem_err(ktl_State *ktl, ktl_CFunction handler)
{
    ktl_CFunction prev = ktl->on_mem_err;
    ktl->on_mem_err = handler;
    return prev;
}

void ktl_mem_err(ktl_State *ktl)
{
    if(ktl->on_mem_err)
        ktl->on_mem_err(ktl);
    else
        fprintf(stderr, "No memory error handler, terminating\n");
    abort();
}
