#include "../../include/global-objects.h"
#include "../include/db.h"

/**
 * insert_user
 * <p>
 * Insert a new User into the User database.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the User to insert
 * @return 0 on success, -1 and set err on failure.
 */
static int insert_user(struct core_object *co, struct server_object *so, const User *user);

/**
 * serialize_user
 * <p>
 * Serialize a User struct.
 * </p>
 * @param co the core object
 * @param serial_user the buffer into which to serialize the User
 * @param user the User to serialize
 * @return 0 on success, -1 and set err on failure
 */
static int serialize_user(struct core_object *co, uint8_t **serial_user, const User *user);

/**
 * insert_channel
 * <p>
 * Insert a new Channel into the Channel database.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the Channel to insert
 * @return 0 on success, -1 and set err on failure.
 */
static int insert_channel(struct core_object *co, struct server_object *so, Channel *channel);

/**
 * serialize_channel
 * <p>
 * Serialize a Channel struct.
 * </p>
 * @param co the core object
 * @param serial_channel the buffer into which to serialize the Channel
 * @param user the Channel to serialize
 * @return 0 on success, -1 and set err on failure
 */
static int serialize_channel(struct core_object *co, uint8_t **serial_channel, const Channel *channel);

/**
 * insert_message
 * <p>
 * Insert a new Message into the Message database.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the Message to insert
 * @return 0 on success, -1 and set err on failure.
 */
static int insert_message(struct core_object *co, struct server_object *so, Message *message);

/**
 * serialize_message
 * <p>
 * Serialize a Message struct.
 * </p>
 * @param co the core object
 * @param serial_message the buffer into which to serialize the Message
 * @param user the Message to serialize
 * @return 0 on success, -1 and set err on failure
 */
static int serialize_message(struct core_object *co, uint8_t **serial_message, const Message *message);

/**
 * insert_auth
 * <p>
 * Insert a new Auth into the Auth database.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the Auth to insert
 * @return 0 on success, -1 and set err on failure.
 */
static int insert_auth(struct core_object *co, struct server_object *so, Auth *auth);

/**
 * serialize_auth
 * <p>
 * Serialize a Auth struct.
 * </p>
 * @param co the core object
 * @param serial_auth the buffer into which to serialize the Auth
 * @param user the Auth to serialize
 * @return 0 on success, -1 and set err on failure
 */
static int serialize_auth(struct core_object *co, uint8_t **serial_auth, const Auth *auth);

/**
 * print_db_error
 * <p>
 * Print an error message based on the error code of passed.
 * </p>
 * @param err_code the error code
 */
static void print_db_error(int err_code);


int db_create(struct core_object *co, struct server_object *so, int type, void *object)
{
    PRINT_STACK_TRACE(co->tracer);
    
    switch (type)
    {
        case USER:
        {
            if (insert_user(co, so, (User *) object) == -1)
            {
                return -1;
            }
            break;
        }
        case CHANNEL:
        {
            if (insert_channel(co, so, (Channel *) object) == -1)
            {
                return -1;
            }
            break;
        }
        case MESSAGE:
        {
            if (insert_message(co, so, (Message *) object) == -1)
            {
                return -1;
            }
            break;
        }
        case AUTH:
        {
            if (insert_auth(co, so, (Auth *) object) == -1)
            {
                return -1;
            }
            break;
        }
        default:;
    }
    
    return 0;
}

static int insert_user(struct core_object *co, struct server_object *so, const User *user)
{
    uint8_t *serial_user;
    int     serial_user_size;
    int     insert_status;
    datum   key;
    datum   value;
    
    serial_user_size = serialize_user(co, &serial_user, user);
    if (serial_user_size == -1)
    {
        return -1;
    }
    
    key.dptr    = user->display_name;
    key.dsize   = strlen(user->display_name + 1);
    value.dptr  = serial_user;
    value.dsize = serial_user_size;
    
    if (sem_wait(so->user_db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    insert_status = dbm_store(so->user_db, key, value, DBM_INSERT);
    
    sem_post(so->user_db_sem);
    
    if (insert_status == 1)
    {
        (void) fprintf(stdout, "Database error occurred: entry with key \"%s\" already exists in User database.\n",
                       (char *) key.dptr);
    } else if (insert_status == -1)
    {
        print_db_error(dbm_error(so->user_db));
    }
    
    return 0;
}

static int serialize_user(struct core_object *co, uint8_t **serial_user, const User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    *serial_user = mm_malloc(sizeof(user->id)
                             + strlen(user->display_name) + 1
                             + sizeof(user->privilege_level), co->mm);
    if (!*serial_user)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    size_t byte_offset;
    
    memcpy(*serial_user, &user->id, sizeof(user->id));
    byte_offset = sizeof(user->id);
    memcpy(*(serial_user + byte_offset), user->display_name, strlen(user->display_name) + 1);
    byte_offset += strlen(user->display_name) + 1;
    memcpy(*(serial_user + byte_offset), &user->privilege_level, sizeof(user->privilege_level));
    
    return 0;
}

static int insert_channel(struct core_object *co, struct server_object *so, Channel *channel)
{
    PRINT_STACK_TRACE(co->tracer);
    
    uint8_t *serial_channel;
    int     serial_channel_size;
    int     insert_status;
    datum   key;
    datum   value;
    
    serial_channel_size = serialize_channel(co, &serial_channel, channel);
    if (serial_channel_size == -1)
    {
        return -1;
    }
    
    key.dptr    = channel->channel_name;
    key.dsize   = strlen(channel->channel_name + 1);
    value.dptr  = serial_channel;
    value.dsize = serial_channel_size;
    
    if (sem_wait(so->channel_db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    insert_status = dbm_store(so->channel_db, key, value, DBM_INSERT);
    
    sem_post(so->channel_db_sem);
    
    if (insert_status == 1)
    {
        (void) fprintf(stdout, "Database error occurred: entry with key \"%s\" already exists in Channel database.\n",
                       (char *) key.dptr);
    } else if (insert_status == -1)
    {
        print_db_error(dbm_error(so->channel_db));
    }
    
    return 0;
}

static int serialize_channel(struct core_object *co, uint8_t **serial_channel, const Channel *channel)
{
    PRINT_STACK_TRACE(co->tracer);
    
    *serial_channel = mm_malloc(sizeof(channel->id)
                                + strlen(channel->channel_name) + 1
                                + strlen(channel->creator) + 1
                                + channel->users_size
                                + channel->administrators_size
                                + channel->banned_users_size, co->mm);
    if (!*serial_channel)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    size_t byte_offset;
    User   **list;
    
    memcpy(*serial_channel, &channel->id, sizeof(channel->id));
    byte_offset = sizeof(channel->id);
    memcpy(*(serial_channel + byte_offset), channel->channel_name, strlen(channel->channel_name) + 1);
    byte_offset += strlen(channel->channel_name) + 1;
    memcpy(*(serial_channel + byte_offset), channel->creator, strlen(channel->creator) + 1);
    byte_offset += strlen(channel->creator) + 1;
    
    list = channel->users;
    for (; *list != NULL; ++list)
    {
        memcpy(*(serial_channel + byte_offset), (*list)->display_name, strlen((*list)->display_name) + 1);
        byte_offset += strlen((*list)->display_name) + 1;
    }
    list = channel->administrators;
    for (; *list != NULL; ++list)
    {
        memcpy(*(serial_channel + byte_offset), (*list)->display_name, strlen((*list)->display_name) + 1);
        byte_offset += strlen((*list)->display_name) + 1;
    }
    list = channel->banned_users;
    for (; *list != NULL; ++list)
    {
        memcpy(*(serial_channel + byte_offset), (*list)->display_name, strlen((*list)->display_name) + 1);
        byte_offset += strlen((*list)->display_name) + 1;
    }
    
    return 0;
}

static int insert_message(struct core_object *co, struct server_object *so, Message *message)
{
    PRINT_STACK_TRACE(co->tracer);
    
    uint8_t *serial_message;
    int     serial_message_size;
    int     insert_status;
    datum   key;
    datum   value;
    
    serial_message_size = serialize_message(co, &serial_message, message);
    if (serial_message_size == -1)
    {
        return -1;
    }
    
    key.dptr    = &message->id;
    key.dsize   = sizeof(message->id);
    value.dptr  = serial_message;
    value.dsize = serial_message_size;
    
    if (sem_wait(so->message_db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    insert_status = dbm_store(so->message_db, key, value, DBM_INSERT);
    
    sem_post(so->message_db_sem);
    
    if (insert_status == 1)
    {
        (void) fprintf(stdout, "Database error occurred: entry with key \"%s\" already exists in Message database.\n",
                       (char *) key.dptr);
    } else if (insert_status == -1)
    {
        print_db_error(dbm_error(so->message_db));
    }
    
    return 0;
}

static int serialize_message(struct core_object *co, uint8_t **serial_message, const Message *message)
{
    PRINT_STACK_TRACE(co->tracer);
    
    *serial_message = mm_malloc(sizeof(message->id)
                                + sizeof(message->user_id)
                                + sizeof(message->channel_id)
                                  * strlen(message->message_content) + 1
                                + sizeof(message->timestamp), co->mm);
    if (!*serial_message)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    size_t byte_offset;
    
    memcpy(*serial_message, &message->id, sizeof(message->id));
    byte_offset = sizeof(message->id);
    memcpy(*(serial_message + byte_offset), &message->user_id, sizeof(message->user_id));
    byte_offset += sizeof(message->user_id);
    memcpy(*(serial_message + byte_offset), &message->channel_id, sizeof(message->channel_id));
    byte_offset += sizeof(message->channel_id);
    memcpy(*(serial_message + byte_offset), message->message_content, strlen(message->message_content) + 1);
    byte_offset += strlen(message->message_content) + 1;
    memcpy(*(serial_message + byte_offset), &message->timestamp, sizeof(message->timestamp));
    
    return 0;
}

static int insert_auth(struct core_object *co, struct server_object *so, Auth *auth)
{
    PRINT_STACK_TRACE(co->tracer);
    
    uint8_t *serial_auth;
    int     serial_auth_size;
    int     insert_status;
    datum   key;
    datum   value;
    
    serial_auth_size = serialize_auth(co, &serial_auth, auth);
    if (serial_auth_size == -1)
    {
        return -1;
    }
    
    key.dptr    = &auth->user_id;
    key.dsize   = sizeof(auth->user_id);
    value.dptr  = serial_auth;
    value.dsize = serial_auth_size;
    
    if (sem_wait(so->auth_db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    insert_status = dbm_store(so->auth_db, key, value, DBM_INSERT);
    
    sem_post(so->auth_db_sem);
    
    if (insert_status == 1)
    {
        (void) fprintf(stdout, "Database error occurred: entry with key \"%s\" already exists in Auth database.\n",
                       (char *) key.dptr);
    } else if (insert_status == -1)
    {
        print_db_error(dbm_error(so->auth_db));
    }
    
    return 0;
}

static int serialize_auth(struct core_object *co, uint8_t **serial_auth, const Auth *auth)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

int db_read(struct core_object *co, struct server_object *so, int type, void *object)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

int db_update(struct core_object *co, struct server_object *so, int type, void *object)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

int db_destroy(struct core_object *co, struct server_object *so, int type, void *object)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

static void print_db_error(int err_code)
{
    switch (err_code)
    {
        case 1:
        {
            (void) fprintf(stdout, "Database error occurred: The specified key was not found in the database.\n");
            break;
        }
        case 2:
        {
            (void) fprintf(stdout, "Database error occurred: The database file could not be opened.\n");
            break;
        }
        case 3:
        {
            (void) fprintf(stdout, "Database error occurred: The database file could not be created.\n");
            break;
        }
        case 4:
        {
            (void) fprintf(stdout,
                           "Database error occurred: An I/O error occurred while reading or writing the database file.\n");
            break;
        }
        case 5:
        {
            (void) fprintf(stdout, "Database error occurred: The database was not opened in read-write mode.\n");
            break;
        }
        case 6:
        {
            (void) fprintf(stdout, "Database error occurred: The database is already open and cannot be reopened.\n");
            break;
        }
        case 7:
        {
            (void) fprintf(stdout,
                           "Database error occurred: The specified key or value was too long to be stored in the database.\n");
            break;
        }
        case 8:
        {
            (void) fprintf(stdout, "Database error occurred: A memory allocation error occurred.\n");
            break;
        }
        case 9:
        {
            (void) fprintf(stdout, "Database error occurred: The database file format is invalid.\n");
            break;
        }
        case 10:
        {
            (void) fprintf(stdout, "Database error occurred: The database file is too old and needs to be rebuilt.\n");
            break;
        }
        case 11:
        {
            (void) fprintf(stdout, "Database error occurred: An unknown error occurred.\n");
            break;
        }
        default:;
    }
}
