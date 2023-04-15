#include "../include/chat.h"
#include "../include/create.h"
#include "../include/destroy.h"
#include "../include/db.h"

/**
 * handle_read
 * <p>
 * Switch on the Object of a READ Type Dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
static int handle_read(struct core_object *co, struct dispatch *dispatch);

/**
 * handle_update
 * <p>
 * Switch on the Object of an UPDATE Type Dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
static int handle_update(struct core_object *co, struct dispatch *dispatch);

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
static int handle_ping(struct core_object *co, struct dispatch *dispatch);

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
            handle_read(co, dispatch);
            break;
        }
        case UPDATE:
        {
            handle_update(co, dispatch);
            break;
        }
        case DESTROY:
        {
            handle_destroy(co, so, dispatch, body_tokens);
            break;
        }
        case PINGUSER:
        case PINGCHANNEL:
        {
            handle_ping(co, dispatch);
            break;
        }
        default:;
    }
    
    return 0;
}

static int handle_read(struct core_object *co, struct dispatch *dispatch)
{
    PRINT_STACK_TRACE(co->tracer);
    
    dispatch->body      = mm_strdup("501\x03Not implemented\x03", co->mm);
    dispatch->body_size = strlen(dispatch->body);
    
    return 0;
}

static int handle_update(struct core_object *co, struct dispatch *dispatch)
{
    PRINT_STACK_TRACE(co->tracer);
    
    dispatch->body      = mm_strdup("501\x03Not implemented\x03", co->mm);
    dispatch->body_size = strlen(dispatch->body);
    
    return 0;
}

static int handle_ping(struct core_object *co, struct dispatch *dispatch)
{
    PRINT_STACK_TRACE(co->tracer);
    
    dispatch->body      = mm_strdup("501\x03Not implemented\x03", co->mm);
    dispatch->body_size = strlen(dispatch->body);
    
    return 0;
}
