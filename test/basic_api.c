#include "test.h"
#include "../src/kettle.h"
#include <malloc.h>
#include <setjmp.h>
#include <string.h>

jmp_buf err_jmp_buf;

static void on_err_in_overflow(ktl_State *ktl);
static void on_err_in_strings(ktl_State *ktl);

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
        ktl_on_err(ktl, on_err_in_overflow);
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
    else if(code == 1)
    {
        ktl_State_del(ktl);
        return OK;
    }
    else
    {
        ktl_State_del(ktl);
        return (Result){
            .code = 1,
            .message = "Wrong error object",
            .dyn_msg = 0
        };
    }
}

Result strings()
{
    ktl_State *ktl = ktl_State_new();
    ASSERT(ktl, "couldn't create a state");

    static const char *const test_str = "hello, Kettle";
    ktl_push_cstring(ktl, test_str);

    ASSERT_CLOSE(
        ktl_top(ktl) == 1 && ktl_get_type(ktl, 0) == KTL_STRING,
        "Failed to push a string"
    );

    size_t len;
    const char *inner_str = ktl_get_string(ktl, 0, &len);
    ASSERT_CLOSE(
        len == strlen(test_str) && !memcmp(test_str, inner_str, len),
        "Pushed a wrong value to stack"
    );

    ktl_clear_vstack(ktl);
    ktl_push_nil(ktl);

    int code = setjmp(err_jmp_buf);
    if(code == 0)
    {
        ktl_on_err(ktl, on_err_in_strings);
        ktl_get_string(ktl, 0, 0);
        ktl_State_del(ktl);
        return (Result){
            .code = 1,
            .message = "Didn't case an error",
            .dyn_msg = 0
        };
    }
    else if(code == 1)
    {
        ktl_State_del(ktl);
        return OK;
    }
    else
    {
        ktl_State_del(ktl);
        return (Result){
            .code = 1,
            .message = "Wrong error object",
            .dyn_msg = 0
        };
    }
}

static void on_err_in_overflow(ktl_State *ktl)
{
    longjmp(err_jmp_buf, 1);
}

static void on_err_in_strings(ktl_State *ktl)
{
    longjmp(err_jmp_buf, 1);
}
