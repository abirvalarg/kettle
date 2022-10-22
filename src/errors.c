#include "kettle.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>

const char *const KTL_ERR_CODES[1] = {
    "stack_corruption"
};

const char *const KTL_ERR_MSGS[1] = {
    "Stack was corrupted"
};

void ktl_push_std_err(ktl_State *ktl, ktl_StdErr err)
{
    ktl_create_object(ktl, 2);
    // TODO set fields
}

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
