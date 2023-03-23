#include "../include/create.h"
#include "../include/db.h"

#include <stdlib.h>

#define BASE 10 // Base for strtol calls, DO NOT CHANGE

/**
 * generate_user_id
 * <p>
 * Generate a unique ID for a new User.
 * </p>
 * @param tracer tracer function
 * @return a unique ID
 */
static int generate_user_id(TRACER_FUNCTION_AS(tracer));

/**
 * generate_channel_id
 * <p>
 * Generate a unique ID for a new Channel.
 * </p>
 * @param tracer tracer function
 * @return a unique ID
 */
static int generate_channel_id(TRACER_FUNCTION_AS(tracer));

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

/**
 * generate_message_id
 * <p>
 * Generate a unique ID for a new Message.
 * </p>
 * @param tracer tracer function
 * @return a unique ID
 */
static int generate_message_id(TRACER_FUNCTION_AS(tracer));

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
    Auth   new_auth;
    size_t offset;
    int    insert_status_user;
    int    insert_status_auth;
    
    // TODO: check that a dispatch has the correct number of tokens
    
    offset = 0;
    new_auth.login_token  = *body_tokens;
    new_user.display_name = *(body_tokens + ++offset);
    new_auth.password     = *(body_tokens + ++offset); // is garbage when 2 tokens
    
    // Validate fields
    // If invalid, assemble 400
    if (!(VALIDATE_LOGIN_TOKEN(new_auth.login_token)
          && VALIDATE_DISPLAY_NAME(new_user.display_name)
          && VALIDATE_PASSWORD(new_auth.password)))
    {
        dispatch->body      = strdup("400\x03Invalid fields\x03");
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    
    // Create ID once the fields are validated.
    new_user.id              = generate_user_id(co->tracer);
    new_auth.user_id         = new_user.id;
    new_user.privilege_level = 0;
    new_user.online_status   = 0;
    
    insert_status_user = db_create(co, so, USER, &new_user);
    insert_status_auth = db_create(co, so, AUTH, &new_auth);
    if (insert_status_user == -1 || insert_status_auth == -1)
    {
        return -1;
    }
    
    if (insert_status_user == 1 && insert_status_auth == 1)
    {
        dispatch->body      = strdup("409\x03""3\x03Login token and display name already taken.\x03");
        dispatch->body_size = strlen(dispatch->body);
    } else if (insert_status_user == 1)
    {
        if (db_destroy(co, so, AUTH, &new_auth) == -1) // Remove the Auth from the database.
        {
            return -1;
        }
        dispatch->body      = strdup("409\x03""2\x03Display name already taken.\x03");
        dispatch->body_size = strlen(dispatch->body);
    } else if (insert_status_auth == 1)
    {
        if (db_destroy(co, so, USER, &new_user) == -1) // Remove the User from the database.
        {
            return -1;
        }
        dispatch->body      = strdup("409\x03""1\x03Login token already taken.\x03");
        dispatch->body_size = strlen(dispatch->body);
    } else
    {
        dispatch->body      = strdup("201\x03"); // Success.
        dispatch->body_size = strlen(dispatch->body);
    }
    
    return 0;
}

static int generate_user_id(TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    static int user_id = 1;
    return user_id++;
}

int handle_create_channel(struct core_object *co, struct server_object *so, struct dispatch *dispatch,
                          char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    Channel new_channel;
    size_t  offset;
    
    offset = 0;
    new_channel.id           = (int) strtol(*body_tokens, NULL, BASE);
    new_channel.channel_name = *(body_tokens + ++offset);
    new_channel.creator      = *(body_tokens + ++offset);
    
    new_channel.users_count = (size_t) strtol(*(body_tokens + ++offset), NULL, BASE);
    if (create_name_list(co, &new_channel.users, (body_tokens + ++offset),
                         new_channel.users_count, &new_channel.users_size_bytes) == -1)
    {
        return -1;
    }
    offset += new_channel.users_count;
    
    new_channel.administrators_count = (size_t) strtol(*(body_tokens + ++offset), NULL, BASE);
    if (create_name_list(co, &new_channel.administrators, (body_tokens + ++offset),
                         new_channel.administrators_count, &new_channel.administrators_size_bytes) == -1)
    {
        return -1;
    }
    offset += new_channel.administrators_count;
    
    new_channel.banned_users_count = (size_t) strtol(*(body_tokens + ++offset), NULL, BASE);
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

static int generate_channel_id(TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    static int channel_id = 1;
    return channel_id++;
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
    new_message.id              = (int) strtol(*body_tokens, NULL, BASE);
    new_message.user_id         = (int) strtol(*(body_tokens + ++offset), NULL, BASE);
    new_message.channel_id      = (int) strtol(*(body_tokens + ++offset), NULL, BASE);
    new_message.message_content = *(body_tokens + ++offset);
    new_message.timestamp       = strtol(*(body_tokens + ++offset), NULL, BASE);
    
    if (db_create(co, so, MESSAGE, &new_message) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int generate_message_id(TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    static int message_id = 1;
    return message_id++;
}

int handle_create_auth(struct core_object *co, struct server_object *so, struct dispatch *dispatch, char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    // validate auth fields
    if (!(VALIDATE_LOGIN_TOKEN(*body_tokens) && VALIDATE_PASSWORD(*(body_tokens + 1))))
    {
        dispatch->body      = strdup("400\x03Invalid fields\x03");
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    
    Auth *auth;
    
    // read db for auth
    if (db_read(co, so, AUTH, &auth, *body_tokens) == -1)
    {
        return -1;
    }
    if (!auth) // No login token exists
    {
        dispatch->body      = strdup("400\x03Invalid fields\x03");
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    if (strcmp(auth->password, *(body_tokens + 1)) != 0) // Wrong password
    {
        dispatch->body      = strdup("403\x03Invalid fields\x03");
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    
    datum user_key;
    datum user_value;
    
    // get user_id
    user_key.dptr = &auth->user_id;
    user_key.dsize = sizeof(auth->user_id);
    
    if (sem_wait(so->auth_db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    // get user with id
    user_value = dbm_fetch(so->auth_db, user_key); // NOLINT(concurrency-mt-unsafe) : Protected
    sem_post(so->auth_db_sem);
    
    if (user_value.dptr == NULL) // This should never happen.
    {
        (void) fprintf(stdout, "Create-Auth: User with id \"%d\" not found in User database.\n", auth->user_id);
    }
    // update user online status
    // add user socket to server
    
    // assemble body with user info
    
    return 0;
}
