#pragma once

typedef enum ktl_ActionKind
{
    KTL_A_NIL,
    KTL_A_IDENT,
    KTL_A_PATH,
    KTL_A_CALL
} ktl_ActionKind;

typedef struct ktl_Action
{
    union {
        char *as_ident;

        struct {
            struct ktl_Action *prefix;
            char *field;
        } as_path;

        struct {
            struct ktl_Action *func;
            struct ktl_Action **args;
            unsigned char num_args;
        } as_call;
    } content;
    unsigned rc;
    ktl_ActionKind kind;
} ktl_Action;

void ktl_Action_del(ktl_Action *act);
