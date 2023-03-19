#include "../include/chat.h"

/**
 * handle_create
 * <p>
 * Handle a CREATE type dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
static int handle_create(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_read
 * <p>
 * Handle a READ type dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
static int handle_read(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_update
 * <p>
 * Handle a UPDATE type dispatch.
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
 * Handle a DESTROY type dispatch.
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
{ return 0; }

static int handle_read(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{ return 0; }

static int handle_update(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{ return 0; }

static int handle_destroy(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{ return 0; }

static int handle_ping(struct core_object *co, struct dispatch *dispatch, char **body_tokens)
{ return 0; }

