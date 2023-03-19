#include "../include/create.h"

int handle_create(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    switch (dispatch->object)
    {
        case USER:
        {
            if (handle_create_user(co, dispatch, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        case CHANNEL:
        {
            if (handle_create_channel(co, dispatch, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        case MESSAGE:
        {
            if (handle_create_message(co, dispatch, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        case AUTH:
        {
            if (handle_create_auth(co, dispatch, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        default:;
    }
    
    return 0;
}

int handle_create_user(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

int handle_create_channel(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

int handle_create_message(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

int handle_create_auth(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}
