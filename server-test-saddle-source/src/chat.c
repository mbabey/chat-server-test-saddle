#include "../include/chat.h"
#include "../include/create.h"
#include "../include/destroy.h"
#include "../include/read.h"
#include "../include/update.h"

/**
 * handle_ping
 * <p>
 * Handle a PING type dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
static int handle_ping(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

int perform_dispatch_operation(struct core_object *co, struct server_object *so, struct dispatch *dispatch,
                               char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    switch (dispatch->type)
    {
        case CREATE:
        {
            handle_create(co, so, dispatch, body_tokens);
            break;
        }
        case READ:
        {
            handle_read(co, dispatch, body_tokens);
            break;
        }
        case UPDATE:
        {
            handle_update(co, dispatch, body_tokens);
            break;
        }
        case DESTROY:
        {
            handle_destroy(co, dispatch, body_tokens);
            break;
        }
        case PINGUSER:
        case PINGCHANNEL:
        {
            handle_ping(co, dispatch, body_tokens);
            break;
        }
        default:;
    }
    
    return 0;
}

static int handle_ping(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    return 0;
}

