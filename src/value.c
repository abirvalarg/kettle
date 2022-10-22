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

void ktl_Value_mark(ktl_Value value)
{
    switch(value.type)
    {
    case KTL_NIL:
    case KTL_BOOLEAN:
    case KTL_INT:
    case KTL_FLOAT:
        break;
    
    case KTL_STRING:
    case KTL_TABLE:
    case KTL_OBJECT:
    case KTL_FUNCTION:
    case KTL_USERDATA:
        value.value.as_obj->mark = 1;
        break;
    }
}
