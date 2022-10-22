#include "kettle.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h>

const char *const KTL_ERR_CODES[2] = {
    "stack_corruption",
    "vstack_overflow"
};

const char *const KTL_ERR_MSGS[2] = {
    "Stack was corrupted",
    "Too many values on the vstack"
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

ktl_CFunction ktl_on_err(ktl_State *ktl, ktl_CFunction handler)
{
    ktl_CFunction prev = ktl->on_err;
    ktl->on_err = handler;
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

void ktl_err(ktl_State *ktl)
{
    if(ktl->on_err)
        ktl->on_err(ktl);
    else
        fprintf(stderr, "No error handler, terminating\n");
    abort();
}
