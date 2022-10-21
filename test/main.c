#include <stdio.h>
#include <malloc.h>
#include "../src/kettle.h"
#include "test.h"

typedef struct Test
{
    const char *const name;
    Result (*func)();
} Test;

static Test tests[] = {
    "state creation", state_creation,
    "vstack ops", vstack_push_pop
};

int main()
{
    printf("running tests...\n");

    unsigned count = sizeof(tests) / sizeof(tests[0]);
    for(unsigned i = 0; i < count; i++)
    {
        printf("\t\e[93m%s\e[0m... ", tests[i].name);
        Result res = tests[i].func();
        if(res.code == 0)
            printf("\e[92mOK\e[0m\n");
        else
            printf("\e[91mFAIL\e[0m\n%s\n", res.message);
        
        if(res.dyn_msg)
            free(res.message);
        
        if(res.code)
            return res.code;
    }

    return 0;
}
