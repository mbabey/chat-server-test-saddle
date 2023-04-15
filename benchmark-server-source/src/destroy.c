#include "../include/destroy.h"
#include "../include/db.h"

/**
 * log_out_user
 * <p>
 * Log a user out by destroying the addr id pair associated with that user.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the user to log out
 * @return 0 on success, -1 and set err on failure.
 */
static int log_out_user(struct core_object *co, struct server_object *so, User *user);

int handle_destroy(struct core_object *co, struct server_object *so, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    if (dispatch->object == AUTH)
    {
        if (handle_destroy_auth(co, so, dispatch, body_tokens) == -1)
        {
            return -1;
        }
    } else
    {
        dispatch->body      = mm_strdup("501\x03Not implemented\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
    }
    
    return 0;
}

int handle_destroy_auth(struct core_object *co, struct server_object *so, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    char **body_tokens_cpy;
    size_t count;
    User request_sender;
    
    body_tokens_cpy = body_tokens;
    count = 0;
    
    COUNT_TOKENS(count, body_tokens_cpy);
    if (count > 1 || !VALIDATE_NAME(*body_tokens))
    {
        dispatch->body      = mm_strdup("400\x03Invalid fields.\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
        return -1;
    }
   
    determine_request_sender(co, so, &request_sender);
    
    return 0;
}

static int log_out_user(struct core_object *co, struct server_object *so, User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    datum key;
    int   status;
    
    user->online_status = 0;
    if (db_update(co, so, USER, user) == -1)
    {
        return -1;
    }
    
    key.dptr  = user->display_name;
    key.dsize = strlen(user->display_name) + 1;
    
    status = safe_dbm_delete(co, ADDR_ID_DB_NAME, so->addr_id_db_sem, &key);
    
    return status;
}
