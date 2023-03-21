#include "../include/create.h"
#include "../include/db.h"

#include <stdlib.h>

int handle_create(struct core_object *co, struct server_object *so, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    switch (dispatch->object)
    {
        case USER:
        {
            if (handle_create_user(co, so, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        case CHANNEL:
        {
            if (handle_create_channel(co, so, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        case MESSAGE:
        {
            if (handle_create_message(co, so, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        case AUTH:
        {
            if (handle_create_auth(co, so, body_tokens) == -1)
            {
                return -1;
            }
            break;
        }
        default:;
    }
    
    return 0;
}

int handle_create_user(struct core_object *co, struct server_object *so, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    User   new_user;
    size_t offset;
    
    offset = 0;
    new_user.id              = (int) strtol(*body_tokens, NULL, 10);
    new_user.display_name    = *(body_tokens + ++offset);
    new_user.privilege_level = (int) strtol(*(body_tokens + ++offset), NULL, 10);
    new_user.online_status   = (int) strtol(*(body_tokens + ++offset), NULL, 10);
    
    if (db_create(co, so, USER, &new_user) == -1)
    {
        return -1;
    }
    
    return 0;
}

int handle_create_channel(struct core_object *co, struct server_object *so, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    Channel new_channel;
    size_t  offset;
    
    offset = 0;
    new_channel.id           = (int) strtol(*body_tokens, NULL, 10);
    new_channel.channel_name = *(body_tokens + ++offset);
    new_channel.creator      = *(body_tokens + ++offset);
    // TODO: get channel lists, byte size
    // body tokens go count, name, name, name, name
    // as the names are collected, count the number of bytes
    
    new_channel.users_count = (size_t) strtol(*(body_tokens + ++offset), NULL, 10);
    for (size_t u = 0; u < new_channel.users_count; ++u)
    {
        // copy each token into the list of users....
    }
    
    new_channel.administrators_count = (size_t) strtol(*(body_tokens + ++offset), NULL, 10);
    
    new_channel.banned_users_count = (size_t) strtol(*(body_tokens + ++offset), NULL, 10);
    
    if (db_create(co, so, CHANNEL, &new_channel) == -1)
    {
        return -1;
    }
    
    return 0;
}

int handle_create_message(struct core_object *co, struct server_object *so, char **body_tokens)
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

int handle_create_auth(struct core_object *co, struct server_object *so, char **body_tokens)
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
