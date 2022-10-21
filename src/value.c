#include "value.h"
#include <malloc.h>

ktl_Var *ktl_Var_new(ktl_Value value)
{
    ktl_Var *var = malloc(sizeof(ktl_Var));
    if(var)
    {
        var->value = value;
        var->rc = 1;
    }
    return var;
}

void ktl_Var_del(ktl_Var *var)
{
    if(--var->rc == 0)
        free(var);
}
