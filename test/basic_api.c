#include "test.h"
#include "../src/kettle.h"
#include <malloc.h>
#include <setjmp.h>

jmp_buf err_jmp_buf;

static void on_err(ktl_State *ktl);

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
    ASSERT_CLOSE(ktl_top(ktl) == 2, "Didn't insert 2 values");
    ktl_clear_vstack(ktl);
    ASSERT_CLOSE(ktl_top(ktl) == 0, "Didn't clear the stack by ktl_clear_vstack");
    ktl_State_del(ktl);
    return OK;
}

Result vstack_overflow()
{
    ktl_State *ktl = ktl_State_new();
    ASSERT(ktl, "couldn't create a state");
    int code = setjmp(err_jmp_buf);
    if(code == 0)
    {
        ktl_on_err(ktl, on_err);
        for(int i = 0; i < KTL_VSTACK_CAPACITY; i++)
            ktl_push_nil(ktl);
        
        // must raise an error
        ktl_push_nil(ktl);

        return (Result){
            .code = 1,
            .message = "Didn't raise an error",
            .dyn_msg = 0
        };
    }
    else
    {
        ktl_State_del(ktl);
        return OK;
    }
}

static void on_err(ktl_State *ktl)
{
    longjmp(err_jmp_buf, 1);
}
