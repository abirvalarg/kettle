#include "test.h"
#include "../src/kettle.h"
#include <malloc.h>

Result state_creation()
{
    ktl_State *ktl = ktl_State_new();
    if(ktl)
    {
        ktl_State_del(ktl);
        return (Result){
            .code = 0,
            .message = 0,
            .dyn_msg = 0
        };
    }
    else
    {
        return (Result){
            .code =1,
            .message = "couldn't create a state",
            .dyn_msg = 0
        };
    }
}
