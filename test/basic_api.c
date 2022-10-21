#include "test.h"
#include "../src/kettle.h"
#include <malloc.h>

Result state_creation()
{
    ktl_State *ktl = ktl_State_new();
    ASSERT(ktl, "couldn't create a state");
    ktl_State_del(ktl);
    return OK;
}

Result vstack_push_pop()
{
    ktl_State *ktl = ktl_State_new();
    ASSERT(ktl, "couldn't create a state");
    ASSERT_CLOSE(ktl_top(ktl) == 0, "Initial lstack is not empty");
    ktl_push_nil(ktl);
    ASSERT_CLOSE(ktl_top(ktl) == 1, "Didn't push nil to the vstack or pushed multiple times");
    ktl_pop(ktl);
    ASSERT_CLOSE(ktl_top(ktl) == 0, "Didn't clear the stack by popping");
    ktl_push_boolean(ktl, 0);
    ktl_push_boolean(ktl, 1);
    ktl_clear_vstack(ktl);
    ASSERT_CLOSE(ktl_top(ktl) == 0, "Didn't clear the stack by ktl_clear_vstack");
    ktl_State_del(ktl);
    return OK;
}
