#include "state.h"
#include <malloc.h>
#include <stdio.h>

ktl_State *ktl_State_new()
{
    ktl_State *ktl = malloc(sizeof(ktl_State));
    return ktl;
}

void ktl_State_del(ktl_State *ktl)
{
    free(ktl);
}
