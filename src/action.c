#include "action.h"
#include <malloc.h>

void ktl_Action_del(ktl_Action *act)
{
    if(--act->rc == 0)
    {
        switch(act->kind)
        {
        case KTL_A_NIL:
            break;
        
        case KTL_A_IDENT:
            free(act->content.as_ident);
            break;
        
        case KTL_A_PATH:
            ktl_Action_del(act->content.as_path.prefix);
            free(act->content.as_path.field);
            break;
        
        case KTL_A_CALL:
            ktl_Action_del(act->content.as_call.func);
            for(unsigned i = 0; i < act->content.as_call.num_args; i++)
                ktl_Action_del(act->content.as_call.args[i]);
            break;
        }

        free(act);
    }
}
