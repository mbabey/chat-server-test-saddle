#include "../../include/global-objects.h"
#include "../include/db.h"

/**
 * insert_user
 * <p>
 * Insert a new User into the User database. If a duplicate entry is found, will return 1.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the User to insert
 * @return 0 on success, 1 if duplicate exists, -1 and set err on failure.
 */
static int insert_user(struct core_object *co, struct server_object *so, User *user);

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
 * read_user
 * <p>
 * Read a User from the database into the memory pointed to by user_get. Will fill with NULL if the User does
 * not exist.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user_get memory in which to store pointer to read User
 * @param display_name name of the User for which to search
 * @return 0 on success, -1 and set err on failure
 */
static int read_user(struct core_object *co, struct server_object *so, User **user_get, const char *display_name);

/**
 * find_by_name
 * <p>
 * Find an entry in the database by a string name. The name must be the second parameter of the object following an int.
 * </p>
 * @param co the core object
 * @param db the database in which to search
 * @param db_sem the semaphore for the database
 * @param serial_object the array to store the result
 * @param name the
 * @return 0 on success, -1 and set err on failure
 */
static int find_by_name(struct core_object *co, DBM *db, sem_t *db_sem, uint8_t **serial_object, const char *name);

/**
 * deserialize_user
 * <p>
 * Deserialize a User into a User struct.
 * </p>
 * @param co the core object
 * @param user_get the User struct
 * @param serial_user the bytes to deserialize
 */
static void deserialize_user(struct core_object *co, User **user_get, uint8_t *serial_user);

/**
 * read_online_users
 * <p>
 * Read all online Users from the database into the memory pointed to by user_get.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param users_get memory in which to store pointers to read Users
 * @return 0 on success, -1 and set err on failure
 */
static int read_online_users(struct core_object *co, struct server_object *so, User ***users_get);

/**
 * read_channel
 * <p>
 * Read a Channel from the database into the memory pointed to by channel_get. Will fill with NULL if the Channel
 * cannot be found.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param channel_get the memory in which to store the pointer to the Channel.
 * @param channel_name the name of the channel to search for
 * @return 0 on success, -1 and set err on failure
 */
static int read_channel(struct core_object *co, struct server_object *so, Channel **channel_get,
                        const char *channel_name);

/**
 * read_messages
 * @param co the core object
 * @param so the server object
 * @param messages_get the memory in which to store pointers to the Messages
 * @param num_messages the number of messages to read
 * @param channel_id the channel in which to search for messages
 * @return 0 on success, -1 and set err on failure
 */
static int read_messages(struct core_object *co, struct server_object *so, Message ***messages_get,
                         int num_messages, int channel_id);

/**
 * delete_user
 * <p>
 * Delete a User from the User database.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the user to delete
 * @return 0 on success, -1 and set errno on failure
 */
static int delete_user(struct core_object *co, struct server_object *so, User *user);

/**
 * delete_channel
 * <p>
 * Delete a Channel from the Channel database.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the Channel to delete
 * @return 0 on success, -1 and set errno on failure
 */
static int delete_channel(struct core_object *co, struct server_object *so, Channel *channel);

/**
 * delete_message
 * <p>
 * Delete a Message from the Message database.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the Message to delete
 * @return 0 on success, -1 and set errno on failure
 */
static int delete_message(struct core_object *co, struct server_object *so, Message *message);

/**
 * delete_auth
 * <p>
 * Delete a Auth from the Auth database.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the Auth to delete
 * @return 0 on success, -1 and set errno on failure
 */
static int delete_auth(struct core_object *co, struct server_object *so, Auth *auth);

/**
 * print_db_error
 * <p>
 * Print an error message based on the error code of passed.
 * </p>
 * @param err_code the error code
 */
static void print_db_error(DBM *db);

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

static int insert_user(struct core_object *co, struct server_object *so, User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    uint8_t *serial_user;
    int     serial_user_size;
    int     insert_status;
    datum   key;
    datum   value;
    
    // Determine if a user with the username already exists in the database.
    if (find_by_name(co, so->user_db, so->user_db_sem, &serial_user, user->display_name) == -1)
    {
        return -1;
    }
    if (serial_user)
    {
        (void) fprintf(stdout, "Database error occurred: User with name \"%s\" already exists in User database.\n",
                       user->display_name);
        return 1;
    }
    
    serial_user_size = serialize_user(co, &serial_user, user);
    if (serial_user_size == -1)
    {
        return -1;
    }
    
    key.dptr    = serial_user;
    key.dsize   = sizeof(user->id);
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
        (void) fprintf(stdout, "Database error occurred: User with ID \"%d\" already exists in User database.\n",
                       *(int *) key.dptr);
        return 1;
    } else if (insert_status == -1)
    {
        print_db_error(so->user_db);
    }
    
    return 0;
}

static int find_by_name(struct core_object *co, DBM *db, sem_t *db_sem, uint8_t **serial_object, const char *name)
{
    PRINT_STACK_TRACE(co->tracer);
    
    datum key;
    datum value;
    
    if (sem_wait(db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    key   = dbm_firstkey(db); // Get first thing in the db
    value = dbm_fetch(db, key);
    
    sem_post(db_sem); // strcmps are done outside of db time.
    
    // Compare the display name to the name in the db
    while (key.dptr && strcmp((char *) ((uint8_t *) value.dptr + sizeof(int)), name) != 0)
    {
        if (sem_wait(db_sem) == -1)
        {
            SET_ERROR(co->err);
            return -1;
        }
        
        key   = dbm_nextkey(db);
        value = dbm_fetch(db, key);
        
        sem_post(db_sem);
    }
    
    if (!key.dptr && dbm_error(db))
    {
        print_db_error(db);
    }
    *serial_object = value.dptr; // Will be null if not found.
    
    return 0;
}

static int serialize_user(struct core_object *co, uint8_t **serial_user, const User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    *serial_user = mm_malloc(sizeof(user->id)
                             + strlen(user->display_name) + 1
                             + sizeof(user->privilege_level)
                             + sizeof(user->online_status), co->mm);
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
    byte_offset += sizeof(user->privilege_level);
    memcpy(*(serial_user + byte_offset), &user->online_status, sizeof(user->online_status));
    
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
    
    // Determine if a channel with the channel name already exists in the database.
    if (find_by_name(co, so->user_db, so->user_db_sem, &serial_channel, channel->channel_name) == -1)
    {
        return -1;
    }
    if (serial_channel)
    {
        (void) fprintf(stdout,
                       "Database error occurred: Channel with name \"%s\" already exists in Channel database.\n",
                       channel->channel_name);
        return 1;
    }
    
    serial_channel_size = serialize_channel(co, &serial_channel, channel);
    if (serial_channel_size == -1)
    {
        return -1;
    }
    
    key.dptr    = serial_channel;
    key.dsize   = sizeof(channel->id);
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
        (void) fprintf(stdout, "Database error occurred: Channel with ID \"%d\" already exists in Channel database.\n",
                       *(int *) key.dptr);
    } else if (insert_status == -1)
    {
        print_db_error(so->channel_db);
    }
    
    return 0;
}

static int serialize_channel(struct core_object *co, uint8_t **serial_channel, const Channel *channel)
{
    PRINT_STACK_TRACE(co->tracer);
    
    *serial_channel = mm_malloc(sizeof(channel->id)
                                + strlen(channel->channel_name) + 1
                                + strlen(channel->creator) + 1
                                + channel->users_size_bytes
                                + channel->administrators_size_bytes
                                + channel->banned_users_size_bytes, co->mm);
    if (!*serial_channel)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    size_t     byte_offset;
    const char **list;
    
    memcpy(*serial_channel, &channel->id, sizeof(channel->id));
    byte_offset = sizeof(channel->id);
    memcpy(*(serial_channel + byte_offset), channel->channel_name, strlen(channel->channel_name) + 1);
    byte_offset += strlen(channel->channel_name) + 1;
    memcpy(*(serial_channel + byte_offset), channel->creator, strlen(channel->creator) + 1);
    byte_offset += strlen(channel->creator) + 1;
    
    list = channel->users;
    for (; *list != NULL; ++list)
    {
        memcpy(*(serial_channel + byte_offset), *list, strlen(*list) + 1);
        byte_offset += strlen(*list) + 1;
    }
    list = channel->administrators;
    for (; *list != NULL; ++list)
    {
        memcpy(*(serial_channel + byte_offset), *list, strlen(*list) + 1);
        byte_offset += strlen(*list) + 1;
    }
    list = channel->banned_users;
    for (; *list != NULL; ++list)
    {
        memcpy(*(serial_channel + byte_offset), *list, strlen(*list) + 1);
        byte_offset += strlen(*list) + 1;
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
    
    key.dptr    = serial_message;
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
        (void) fprintf(stdout, "Database error occurred: Message with ID \"%d\" already exists in Message database.\n",
                       *(int *) key.dptr);
    } else if (insert_status == -1)
    {
        print_db_error(so->message_db);
    }
    
    return 0;
}

static int serialize_message(struct core_object *co, uint8_t **serial_message, const Message *message)
{
    PRINT_STACK_TRACE(co->tracer);
    
    *serial_message = mm_malloc(sizeof(message->id)
                                + sizeof(message->user_id)
                                + sizeof(message->channel_id)
                                + strlen(message->message_content) + 1
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
    
    // Determine if an auth with the login token already exists in the database.
    if (find_by_name(co, so->user_db, so->user_db_sem, &serial_auth, auth->login_token) == -1)
    {
        return -1;
    }
    if (serial_auth)
    {
        (void) fprintf(stdout, "Database error occurred: Auth with token \"%s\" already exists in Auth database.\n",
                       auth->login_token);
        return 1;
    }
    
    serial_auth_size = serialize_auth(co, &serial_auth, auth);
    if (serial_auth_size == -1)
    {
        return -1;
    }
    
    key.dptr    = serial_auth;
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
        (void) fprintf(stdout, "Database error occurred: Auth with ID \"%d\" already exists in Auth database.\n",
                       *(int *) key.dptr);
        return 1;
    } else if (insert_status == -1)
    {
        print_db_error(so->auth_db);
    }
    
    return 0;
}

static int serialize_auth(struct core_object *co, uint8_t **serial_auth, const Auth *auth)
{
    PRINT_STACK_TRACE(co->tracer);
    
    *serial_auth = mm_malloc(sizeof(auth->user_id)
                             * strlen(auth->login_token) + 1
                             + strlen(auth->password) + 1, co->mm);
    if (!*serial_auth)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    size_t byte_offset;
    
    memcpy(*serial_auth, &auth->user_id, sizeof(auth->user_id));
    byte_offset = sizeof(auth->user_id);
    memcpy(*(serial_auth + byte_offset), auth->login_token, strlen(auth->login_token) + 1);
    byte_offset += strlen(auth->login_token) + 1;
    memcpy(*(serial_auth + byte_offset), auth->password, strlen(auth->password) + 1); // TODO: hash this lol
    
    return 0;
}

int db_read(struct core_object *co, struct server_object *so, int type, void ***object_dst, void *object_query)
{
    PRINT_STACK_TRACE(co->tracer);
    
    switch (type)
    {
        case USER:
        {
            int status;
            
            if (object_query)
            {
                status = read_user(co, so, *(User ***) object_dst, (const char *) object_query);
            } else
            {
                status = read_online_users(co, so, (User ***) object_dst);
            }
            if (status == -1)
            {
                return -1;
            }
            break;
        }
        case CHANNEL:
        {
            if (read_channel(co, so, *(Channel ***) object_dst, (const char *) object_query) == -1)
            {
                return -1;
            }
            break;
        }
        case MESSAGE:
        {
            if (read_messages(co, so, (Message ***) object_dst,
                              *(int *) object_query, *((int *) object_query + 1)) == -1)
            {
                return -1;
            }
            break;
        }
        default:;
    }
    
    return 0;
}

static int read_user(struct core_object *co, struct server_object *so, User **user_get, const char *display_name)
{
    PRINT_STACK_TRACE(co->tracer);
    
    uint8_t *serial_user;
    
    // Read the database to find the User
    if (find_by_name(co, so->user_db, so->user_db_sem, &serial_user, display_name) == -1)
    {
        return -1;
    }
    
    if (!serial_user) // User not found.
    {
        *user_get = NULL;
        return 0;
    }
    
    // deserialize the user and put it in *user_get
    *user_get = mm_malloc(sizeof(User), co->mm);
    if (!*user_get)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    deserialize_user(co, user_get, serial_user);
    
    return 0;
}

static void deserialize_user(struct core_object *co, User **user_get, uint8_t *serial_user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    size_t byte_offset;
    
    memcpy(&(*user_get)->id, serial_user, sizeof((*user_get)->id));
    byte_offset = sizeof((*user_get)->id);
    strcpy((*user_get)->display_name, (char *) (serial_user + byte_offset));
    byte_offset += strlen((*user_get)->display_name);
    memcpy(&(*user_get)->privilege_level, (serial_user + byte_offset), sizeof((*user_get)->privilege_level));
    byte_offset += sizeof((*user_get)->privilege_level);
    memcpy(&(*user_get)->online_status, (serial_user + byte_offset), sizeof((*user_get)->online_status));
}

int db_update(struct core_object *co, struct server_object *so, int type, void *object)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

int db_destroy(struct core_object *co, struct server_object *so, int type, void *object)
{
    PRINT_STACK_TRACE(co->tracer);
    
    switch (type)
    {
        case USER:
        {
            delete_user(co, so, (User *) object);
            break;
        }
        case CHANNEL:
        {
            delete_channel(co, so, (Channel *) object);
            break;
        }
        case MESSAGE:
        {
            delete_message(co, so, (Message *) object);
            break;
        }
        case AUTH:
        {
            delete_auth(co, so, (Auth *) object);
            break;
        }
        default:;
    }
    
    return 0;
}

static int delete_user(struct core_object *co, struct server_object *so, User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    datum key;
    int delete_status;
    
    key.dptr  = &user->id;
    key.dsize = sizeof(user->id);
    
    if(sem_wait(so->user_db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    delete_status = dbm_delete(so->user_db, key);
    
    sem_post(so->user_db_sem);
    
    if (delete_status == -1)
    {
        print_db_error(so->user_db);
    }
    
    return 0;
}

static int delete_channel(struct core_object *co, struct server_object *so, Channel *channel)
{
    PRINT_STACK_TRACE(co->tracer);
    
    datum key;
    int delete_status;
    
    key.dptr  = &channel->id;
    key.dsize = sizeof(channel->id);
    
    if(sem_wait(so->channel_db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    delete_status = dbm_delete(so->channel_db, key);
    
    sem_post(so->channel_db_sem);
    
    if (delete_status == -1)
    {
        print_db_error(so->channel_db);
    }
    
    return 0;
}

static int delete_message(struct core_object *co, struct server_object *so, Message *message)
{
    PRINT_STACK_TRACE(co->tracer);
    
    datum key;
    int delete_status;
    
    key.dptr  = &message->id;
    key.dsize = sizeof(message->id);
    
    if(sem_wait(so->message_db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    delete_status = dbm_delete(so->message_db, key);
    
    sem_post(so->message_db_sem);
    
    if (delete_status == -1)
    {
        print_db_error(so->message_db);
    }
    
    return 0;
}

static int delete_auth(struct core_object *co, struct server_object *so, Auth *auth)
{
    PRINT_STACK_TRACE(co->tracer);
    datum key;
    int delete_status;
    
    key.dptr  = &auth->user_id;
    key.dsize = sizeof(auth->user_id);
    
    if(sem_wait(so->auth_db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    delete_status = dbm_delete(so->auth_db, key);
    
    sem_post(so->auth_db_sem);
    
    if (delete_status == -1)
    {
        print_db_error(so->auth_db);
    }
    return 0;
}

static void print_db_error(DBM *db)
{
    int err_code;
    
    err_code = dbm_error(db);
    dbm_clearerr(db);
    
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
