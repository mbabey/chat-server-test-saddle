#include "../include/db.h"
#include "../include/destroy.h"
#include "../include/object-util.h"

#include <stdlib.h>

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
    
    char   **body_tokens_cpy;
    size_t count;
    User   request_sender;
    
    body_tokens_cpy = body_tokens;
    count           = 0;
    // NOLINTNEXTLINE(clang-analyzer-unix.cstring.NullArg,clang-analyzer-core.NonNullParamChecker): Nope
    COUNT_TOKENS(count, body_tokens_cpy);
    if (count > 1 || !(*body_tokens && VALIDATE_NAME(*body_tokens)))
    {
        dispatch->body      = mm_strdup("400\x03Invalid fields.\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
        return -1;
    }
    
    if (determine_request_sender(co, so, &request_sender) == -1)
    {
        return -1;
    }
    
    int  read_status;
    User *user_to_log_out;
    
    user_to_log_out = mm_malloc(sizeof(User), co->mm);
    if (!user_to_log_out)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    read_status = db_read(co, so, USER, &user_to_log_out, *body_tokens);
    if (read_status == -1)
    {
        free_user(co, user_to_log_out);
        return -1;
    }
    
    if (request_sender.privilege_level == GLOBAL_ADMIN)
    {
        if (read_status == 0)
        {
            dispatch->body      = mm_strdup("404\x03User not found.\x03", co->mm);
            dispatch->body_size = strlen(dispatch->body);
        }
    }
    
    if (log_out_user(co, so, user_to_log_out) == -1)
    {
        free_user(co, user_to_log_out);
        return -1;
    }
    
    dispatch->body      = mm_strdup("200\x03Logged out.\x03", co->mm);
    dispatch->body_size = strlen(dispatch->body);
    
    free_user(co, user_to_log_out);
    return 0;
}

static int log_out_user(struct core_object *co, struct server_object *so, User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    datum      key;
    AddrIdPair *addr_id_pair;
    uint8_t    *addr_key;
    int        status;
    
    user->online_status = 0;
    if (db_update(co, so, USER, user) == -1)
    {
        return -1;
    }
    
    addr_id_pair = mm_calloc(1, sizeof(AddrIdPair), co->mm);
    if (!addr_id_pair)
    {
        SET_ERROR(co->err);
        return -1;
    }
    if (find_addr_id_pair_by_id(co, so, &addr_id_pair, user->id) == -1)
    {
        return -1;
    }
    
    addr_key = malloc(sizeof(in_addr_t) + sizeof(in_port_t));
    if (!addr_key)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    memcpy(addr_key, &addr_id_pair->socket_ip, sizeof(in_addr_t));
    memcpy(addr_key + sizeof(in_addr_t), &addr_id_pair->socket_port, sizeof(in_port_t));
    
    key.dptr  = (void *) addr_key;
    key.dsize = SOCKET_ADDR_SIZE;

    status = safe_dbm_delete(co, ADDR_ID_DB_NAME, so->addr_id_db_sem, &key);
    
    free(addr_key);
    
    return status;
}
