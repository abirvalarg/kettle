#pragma once

typedef struct Result
{
    int code;
    char *message;
    char dyn_msg;
} Result;

Result state_creation();
