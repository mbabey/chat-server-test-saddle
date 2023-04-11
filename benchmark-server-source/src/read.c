#include "../include/read.h"

int handle_read(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{ return 0; }

int handle_read_user(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    return 0;
}

int handle_read_channel(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    return 0;
}

int handle_read_message(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    return 0;
}
