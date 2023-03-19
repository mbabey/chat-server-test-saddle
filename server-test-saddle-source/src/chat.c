#include "../include/chat.h"

/**
 * handle_create
 * <p>
 * Switch on the Object of a CREATE Type Dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
static int handle_create(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_create_user
 * <p>
 * Create a User account.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure.
 */
static int handle_create_user(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_create_channel
 * <p>
 * Create a Channel.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure.
 */
static int handle_create_channel(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_create_message
 * <p>
 * Create a Message in a Channel.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure.
 */
static int handle_create_message(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_create_auth
 * <p>
 * Log a User in.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure.
 */
static int handle_create_auth(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

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
static int handle_read(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_read_user
 * <p>
 * Get User information and put it in the dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
static int handle_read_user(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_read_channel
 * <p>
 * Get Channel information and put it in the dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
static int handle_read_channel(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_read_message
 * <p>
 * Get Message information and put it in the dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
static int handle_read_message(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

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
static int handle_update(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_destroy
 * <p>
 * Switch on the Object of a DESTROY Type Dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
static int handle_destroy(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

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

int perform_dispatch_operation(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    switch (dispatch->type)
    {
        case CREATE:
        {
            handle_create(co, dispatch, body_tokens);
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

static int handle_create(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
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

static int handle_create_user(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

static int handle_create_channel(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

static int handle_create_message(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

static int handle_create_auth(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

static int handle_read(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{ return 0; }

static int handle_read_user(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    return 0;
}

static int handle_read_channel(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    return 0;
}

static int handle_read_message(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    return 0;
}

static int handle_update(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    return 0;
}

static int handle_update_user(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

static int handle_update_channel(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

static int handle_update_message(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

static int handle_update_auth(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

static int handle_destroy(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    return 0;
}

static int handle_destroy_user(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

static int handle_destroy_channel(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

static int handle_destroy_message(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

static int handle_destroy_auth(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

static int handle_ping(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    return 0;
}

