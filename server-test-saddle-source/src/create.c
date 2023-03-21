#include "../include/create.h"
#include "../include/db.h"

#include <stdlib.h>

/**
 * create_name_list
 * <p>
 * Allocate memory for a list of count names. Fill the list with names from src_list up to count. Add a NULL
 * terminating element. Record the number of bytes in the destination list.
 * </p>
 * @param co the core object
 * @param dst_list the destination list
 * @param src_list the source list
 * @param count the number of items to copy
 * @param byte_count the number of bytes copied
 * @return 0 on success, -1 and set err on failure
 */
static int create_name_list(struct core_object *co, const char ***dst_list, char **src_list,
                            size_t count, size_t *byte_count);

int handle_create(struct core_object *co, struct server_object *so, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    switch (dispatch->object)
    {
        case USER:
        {
            if (handle_create_user(co, so, dispatch, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        case CHANNEL:
        {
            if (handle_create_channel(co, so, dispatch, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        case MESSAGE:
        {
            if (handle_create_message(co, so, dispatch, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        case AUTH:
        {
            if (handle_create_auth(co, so, dispatch, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        default:;
    }
    
    return 0;
}

int handle_create_user(struct core_object *co, struct server_object *so, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    User   new_user;
    size_t offset;
    int insert_status;
    
    offset = 0;
    new_user.id              = (int) strtol(*body_tokens, NULL, 10);
    new_user.display_name    = *(body_tokens + ++offset);
    new_user.privilege_level = 0;
    new_user.online_status   = 0;
    
    // Validate fields
    // If invalid, assemble 400
    
    insert_status = db_create(co, so, USER, &new_user);
    if (insert_status == -1)
    {
        return -1;
    }
    
    if (insert_status == 1)
    {
        // assemble 409
    }
    
    // assemble 201
    
    return 0;
}

int handle_create_channel(struct core_object *co, struct server_object *so, struct dispatch *dispatch,
                          char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    Channel new_channel;
    size_t  offset;
    
    offset = 0;
    new_channel.id           = (int) strtol(*body_tokens, NULL, 10);
    new_channel.channel_name = *(body_tokens + ++offset);
    new_channel.creator      = *(body_tokens + ++offset);
    
    new_channel.users_count = (size_t) strtol(*(body_tokens + ++offset), NULL, 10);
    if (create_name_list(co, &new_channel.users, (body_tokens + ++offset),
                         new_channel.users_count, &new_channel.users_size_bytes) == -1)
    {
        return -1;
    }
    offset += new_channel.users_count;
    
    new_channel.administrators_count = (size_t) strtol(*(body_tokens + ++offset), NULL, 10);
    if (create_name_list(co, &new_channel.administrators, (body_tokens + ++offset),
                         new_channel.administrators_count, &new_channel.administrators_size_bytes) == -1)
    {
        return -1;
    }
    offset += new_channel.administrators_count;
    
    new_channel.banned_users_count = (size_t) strtol(*(body_tokens + ++offset), NULL, 10);
    if (create_name_list(co, &new_channel.banned_users, (body_tokens + ++offset),
                         new_channel.banned_users_count, &new_channel.banned_users_size_bytes) == -1)
    {
        return -1;
    }
    
    if (db_create(co, so, CHANNEL, &new_channel) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int create_name_list(struct core_object *co, const char ***dst_list, char **src_list,
                            size_t count, size_t *byte_count)
{
    PRINT_STACK_TRACE(co->tracer);
    
    *dst_list = mm_malloc((count + 1) * sizeof(const char *), co->mm);
    if (!*dst_list)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    *byte_count = 0;
    for (size_t i = 0; i < count; ++i)
    {
        *(*dst_list + i) = *(src_list + i);
        *byte_count += strlen(*(src_list + i) + 1);
    }
    
    *(*dst_list + count) = NULL;
    
    return 0;
}

int handle_create_message(struct core_object *co, struct server_object *so, struct dispatch *dispatch,
                          char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    Message new_message;
    size_t  offset;
    
    offset = 0;
    new_message.id              = (int) strtol(*body_tokens, NULL, 10);
    new_message.user_id         = (int) strtol(*(body_tokens + ++offset), NULL, 10);
    new_message.channel_id      = (int) strtol(*(body_tokens + ++offset), NULL, 10);
    new_message.message_content = *(body_tokens + ++offset);
    new_message.timestamp       = strtol(*(body_tokens + ++offset), NULL, 10);
    
    if (db_create(co, so, MESSAGE, &new_message) == -1)
    {
        return -1;
    }
    
    return 0;
}

int handle_create_auth(struct core_object *co, struct server_object *so, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    Auth   new_auth;
    size_t offset;
    
    offset = 0;
    new_auth.user_id     = (int) strtol(*body_tokens, NULL, 10);
    new_auth.login_token = *(body_tokens + ++offset);
    new_auth.password    = *(body_tokens + ++offset);
    
    if (db_create(co, so, AUTH, &new_auth) == -1)
    {
        return -1;
    }
    
    return 0;
}
