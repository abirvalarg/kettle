#pragma once

#define ASSERT(cond, msg) if(!(cond)) { return (Result){ .code = 1, .message = (msg), .dyn_msg = 0 }; }
#define ASSERT_CLOSE(cond, msg) if(!(cond)) { ktl_State_del(ktl); return (Result){ .code = 1, .message = (msg), .dyn_msg = 0 }; }

#define OK ((Result){ \
        .code = 0,  \
        .message = 0,   \
        .dyn_msg = 0    \
    })

typedef struct Result
{
    int code;
    char *message;
    char dyn_msg;
} Result;

Result state_creation();
Result vstack_push_pop();
Result strmap();
Result strmap_with_capacity();
Result vstack_overflow();
Result strings();
