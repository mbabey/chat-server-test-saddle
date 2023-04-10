#include "../../include/util.h"
#include "../include/create.h"
#include "../include/db.h"
#include "../include/object-util.h"

#include <stdlib.h>

// NOLINTNEXTLINE(modernize-macro-to-enum) : No
#define BASE 10 // Base for strtol calls, DO NOT CHANGE

/** Number of tokens that should be present in Create Type Dispatches. */
enum BodyTokenSizes
{
    CREATE_USER_BODY_TOKEN_SIZE    = 3,
    CREATE_CHANNEL_BODY_TOKEN_SIZE = 3,
    CREATE_MESSAGE_BODY_TOKEN_SIZE = 4,
    CREATE_AUTH_BODY_TOKEN_SIZE    = 2
};

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
 * determine_request_sender
 * <p>
 * Given a socket address on which a dispatch was sent, find the User that sent the dispatch.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param request_sender the User object into which to store the request sender
 * @return 0 on success, -1 and set err on failure
 */
static int determine_request_sender(struct core_object *co, struct server_object *so, User *request_sender);

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

/**
 * assemble_200_create_auth_response
 * <p>
 * Assemble a 200 create auth response with the user information in the body.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch in which to assemble the body
 * @param user the user of which to send information
 * @return 0 on success, -1 and set err on failure
 */
static int assemble_200_create_auth_response(struct core_object *co, struct dispatch *dispatch, const User *user);

/**
 * log_in_user
 * <p>
 * Update a user's online status to 1 and update the user in the database. Add the user to the list of active users.
 * If the user is already logged in, remove the disconnect the currently connected socket address and replace it with
 * the new socket address.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the user to log in
 * @return 0 on success, -1 and set err on failure.
 */
static int log_in_user(struct core_object *co, struct server_object *so, User *user);

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
    size_t count;
    char   **body_tokens_cpy;
    
    count           = 0;
    body_tokens_cpy = body_tokens;
    COUNT_TOKENS(count, body_tokens_cpy);
    if (count != CREATE_USER_BODY_TOKEN_SIZE)
    {
        dispatch->body      = mm_strdup("400\x03Invalid number of fields\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    
    offset = 0;
    new_auth.login_token  = *body_tokens;
    new_user.display_name = *(body_tokens + ++offset);
    new_auth.password     = *(body_tokens + ++offset); // is garbage when 2 tokens
    
    // Validate fields
    // If invalid, assemble 400
    if (!(VALIDATE_LOGIN_TOKEN(new_auth.login_token)
          && VALIDATE_NAME(new_user.display_name)
          && VALIDATE_PASSWORD(new_auth.password)))
    {
        dispatch->body      = mm_strdup("400\x03Invalid fields\x03", co->mm);
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
        dispatch->body      = mm_strdup("409\x03""3\x03Login token and display name already taken.\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
    } else if (insert_status_user == 1)
    {
        if (db_destroy(co, so, AUTH, &new_auth) == -1) // Remove the Auth from the database.
        {
            return -1;
        }
        dispatch->body      = mm_strdup("409\x03""2\x03Display name already taken.\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
    } else if (insert_status_auth == 1)
    {
        if (db_destroy(co, so, USER, &new_user) == -1) // Remove the User from the database.
        {
            return -1;
        }
        dispatch->body      = mm_strdup("409\x03""1\x03Login token already taken.\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
    } else
    {
        dispatch->body      = mm_strdup("201\x03", co->mm); // Success.
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
    size_t  count;
    char    **body_tokens_cpy;
    User    request_sender;
    
    count           = 0;
    body_tokens_cpy = body_tokens;
    COUNT_TOKENS(count, body_tokens_cpy);
    if (count != CREATE_CHANNEL_BODY_TOKEN_SIZE)
    {
        dispatch->body      = mm_strdup("400\x03Invalid number of fields\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    
    offset = 0;
    new_channel.id           = generate_channel_id(co->tracer);
    new_channel.channel_name = *(body_tokens);
    new_channel.creator      = *(body_tokens + ++offset);
    if (**(body_tokens + ++offset) == '1') // Publicity is set to 1 in the dispatch.
    {
        (void) fprintf(stdout, "Note: Private channels are not supported. Channel \"%s\" defaulted to public.\n",
                       new_channel.channel_name);
    }
    
    if (!(VALIDATE_NAME(new_channel.creator) && VALIDATE_NAME(new_channel.channel_name)))
    {
        dispatch->body      = mm_strdup("400\x03Invalid fields\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    
    if (determine_request_sender(co, so, &request_sender) == -1)
    {
        SET_ERROR(co->err);
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

static int determine_request_sender(struct core_object *co, struct server_object *so, User *request_sender)
{
    PRINT_STACK_TRACE(co->tracer);
    
    // Use the socket address to find the user id.
    
    // Use the user id to find the user.
    
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
    
    size_t count;
    char   **body_tokens_cpy;
    
    // Validate Auth message.
    
    count           = 0;
    body_tokens_cpy = body_tokens;
    COUNT_TOKENS(count, body_tokens_cpy);
    if (count != CREATE_AUTH_BODY_TOKEN_SIZE)
    {
        dispatch->body      = mm_strdup("400\x03Invalid number of fields\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    if (!(VALIDATE_LOGIN_TOKEN(*body_tokens) && VALIDATE_PASSWORD(*(body_tokens + 1))))
    {
        dispatch->body      = mm_strdup("400\x03Invalid fields\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    
    Auth *auth;
    
    // Read database to find Auth.
    if (db_read(co, so, AUTH, &auth, *body_tokens) == -1)
    {
        return -1;
    }
    if (!auth) // No login token exists
    {
        dispatch->body      = mm_strdup("403\x03No account found with provided login token.\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    if (strcmp(auth->password, *(body_tokens + 1)) != 0) // Wrong password
    {
        dispatch->body      = mm_strdup("403\x03Incorrect password.\x03", co->mm);
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    
    int     ret_val;
    uint8_t *serial_user;
    datum   user_key;
    
    // get user_id, get user with id
    user_key.dptr  = &auth->user_id;
    user_key.dsize = sizeof(auth->user_id);
    
    ret_val = safe_dbm_fetch(co, USER_DB_NAME, so->user_db_sem, &user_key, &serial_user);
    free_auth(co, auth);
    if (ret_val == -1)
    {
        return -1;
    }
    if (ret_val == 1) // This should never happen, but just in case.
    {
        (void) fprintf(stdout, "Create-Auth: User with id \"%d\" not found in User database.\n", auth->user_id);
        dispatch->body      = mm_strdup("500\x03""Database Error: Auth exists with no existing referenced User.\x03",
                                        co->mm);
        dispatch->body_size = strlen(dispatch->body);
        return 0;
    }
    
    User *user;
    
    user = mm_malloc(sizeof(User), co->mm);
    if (!user)
    {
        SET_ERROR(co->err);
        return -1;
    }
    deserialize_user(co, &user, serial_user);
    if (log_in_user(co, so, user) == -1)
    {
        return -1;
    }
    
    if (assemble_200_create_auth_response(co, dispatch, user) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int log_in_user(struct core_object *co, struct server_object *so, User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    int     status;
    uint8_t *name_addr;
    size_t  name_addr_size;
    
    name_addr_size = serialize_addr_id_pair(co, &name_addr, &so->child->client_addr, &user->id);
    if (name_addr_size == 0)
    {
        return -1;
    }
    
    datum key;
    datum value;
    
    key.dptr    = name_addr;
    key.dsize   = SOCKET_ADDR_SIZE; // The first thing in here is a socket address.
    value.dptr  = name_addr;
    value.dsize = name_addr_size;
    
    // If the user is not logged in, just log them in and add them to the name-addr database.
    if (user->online_status == 0)
    {
        user->online_status = 1;
        if (db_update(co, so, USER, user) == -1)
        {
            return -1;
        }
        
        status = safe_dbm_store(co, ADDR_ID_DB_NAME, so->addr_id_db_sem, &key, &value, DBM_INSERT);
        
    } else // If the user is already logged in, update the name-addr database. The last connected user will be refused on all routes.
    {
        status = safe_dbm_store(co, ADDR_ID_DB_NAME, so->addr_id_db_sem, &key, &value, DBM_REPLACE);
    }
    
    return status;
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


static int assemble_200_create_auth_response(struct core_object *co, struct dispatch *dispatch, const User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    // assemble body with user info
    char   *body_buffer;
    int    id_size;
    int    privilege_size;
    size_t body_size;
    
    id_size        = snprintf(NULL, 0, "%d", user->id);
    privilege_size = snprintf(NULL, 0, "%d", user->privilege_level);
    if (id_size == -1 || privilege_size == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    // 9 is the 3 digit status code, the online status, and the 5 terminating ETXs
    body_size = id_size + strlen(user->display_name) + privilege_size + 9; // NOLINT : Magic number contained
    
    body_buffer = mm_malloc(body_size + 1, co->mm);
    if (!body_buffer)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    if (snprintf(body_buffer, body_size, "200\x03%d\x03%s\x03%d\x03%d\x03",
                 user->id, user->display_name, user->privilege_level, user->online_status) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    dispatch->body      = body_buffer;
    dispatch->body_size = body_size;
    
    return 0;
}
