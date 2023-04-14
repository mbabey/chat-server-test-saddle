#include "../../include/global-objects.h"
#include "../include/db.h"
#include "../include/object-util.h"

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
 * <p>
 * Read a number of messages up to num_messages from the channel with channel_id.
 * </p>
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
 * read_auth
 * <p>
 * Read an Authorization from the database with the parameter login token.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param auth_get pointer in which to store the auth
 * @param login_token the login token for which to search
 * @return 0 on success, -1 and set err on failure
 */
static int read_auth(struct core_object *co, struct server_object *so, Auth **auth_get, const char *login_token);

/**
 * update_user
 * <p>
 * Provided the updated user in user, find a user with the same ID in the database and update the user with the
 * values of the fields in user.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param user the updated user; id will be used for query
 * @return 0 on success, -1 and set err on failure
 */
static int update_user(struct core_object *co, struct server_object *so, User *user);

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
 * save_dptr_to_serial_object
 * <p>
 * Return 1 if value->dptr exists and 0 if value.dptr does not exist.
 * If a byte array is provided and value->dptr exists, copy the value in a value->dptr into a byte array.
 * Otherwise if a byte array is provide and value->dptr does not exist, set the byte array to NULL.
 * </p>
 * @param co the core object
 * @param serial_object the byte array into which to copy value->dptr
 * @param value the datum to copy
 * @return 1 if value->dptr exists and 0 if value.dptr does not exist, -1 and set err on failure.
 */
static int save_dptr_to_serial_object(struct core_object *co, uint8_t **serial_object, datum *value);

int db_create(struct core_object *co, struct server_object *so, int type, void *object)
{
    PRINT_STACK_TRACE(co->tracer);
    
    int insert_status;
    
    switch (type)
    {
        case USER:
        {
            insert_status = insert_user(co, so, (User *) object);
            break;
        }
        case CHANNEL:
        {
            insert_status = insert_channel(co, so, (Channel *) object);
            break;
        }
        case MESSAGE:
        {
            insert_status = insert_message(co, so, (Message *) object);
            break;
        }
        case AUTH:
        {
            insert_status = insert_auth(co, so, (Auth *) object);
            break;
        }
        default:
        {
            insert_status = -1;
        }
    }
    
    return insert_status;
}

static int insert_user(struct core_object *co, struct server_object *so, User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    uint8_t       *serial_user;
    unsigned long serial_user_size;
    int           status;
    datum         key;
    datum         value;
    
    // Determine if a user with the username already exists in the database.
    status = find_by_name(co, USER_DB_NAME, so->user_db_sem, NULL, user->display_name);
    if (status == -1)
    {
        return -1;
    }
    if (status == 1)
    {
        (void) fprintf(stdout, "Database error occurred: User with name \"%s\" already exists in User database.\n",
                       user->display_name);
        return 1;
    }
    
    serial_user_size = serialize_user(co, &serial_user, user);
    if (serial_user_size == 0)
    {
        return -1;
    }
    
    printf("serial user size: %lu\n", serial_user_size);
    
    key.dptr    = serial_user;
    key.dsize   = sizeof(user->id);
    value.dptr  = serial_user;
    value.dsize = serial_user_size;
    
    status = safe_dbm_store(co, USER_DB_NAME, so->user_db_sem, &key, &value, DBM_INSERT);
    
    if (status == 1)
    {
        (void) fprintf(stdout, "Database error occurred: User with ID \"%d\" already exists in User database.\n",
                       *(int *) key.dptr);
        return 1;
    }
    if (status == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    return 0;
}

static int insert_channel(struct core_object *co, struct server_object *so, Channel *channel)
{
    PRINT_STACK_TRACE(co->tracer);
    
    uint8_t       *serial_channel;
    unsigned long serial_channel_size;
    int           status;
    datum         key;
    datum         value;
    
    // Determine if a channel with the channel name already exists in the database.
    status = find_by_name(co, CHANNEL_DB_NAME, so->channel_db_sem, NULL, channel->channel_name);
    if (status == -1)
    {
        return -1;
    }
    if (status == 1)
    {
        (void) fprintf(stdout,
                       "Database error occurred: Channel with name \"%s\" already exists in Channel database.\n",
                       channel->channel_name);
        return 1;
    }
    
    serial_channel_size = serialize_channel(co, &serial_channel, channel);
    if (serial_channel_size == 0)
    {
        return -1;
    }
    
    key.dptr    = serial_channel;
    key.dsize   = sizeof(channel->id);
    value.dptr  = serial_channel;
    value.dsize = serial_channel_size;
    
    status = safe_dbm_store(co, CHANNEL_DB_NAME, so->channel_db_sem, &key, &value, DBM_INSERT);
    
    if (status == 1)
    {
        (void) fprintf(stdout, "Database error occurred: Channel with ID \"%d\" already exists in Channel database.\n",
                       *(int *) key.dptr);
    } else if (status == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    return 0;
}

static int insert_message(struct core_object *co, struct server_object *so, Message *message)
{
    PRINT_STACK_TRACE(co->tracer);
    
    uint8_t       *serial_message;
    unsigned long serial_message_size;
    int           status;
    datum         key;
    datum         value;
    
    serial_message_size = serialize_message(co, &serial_message, message);
    if (serial_message_size == 0)
    {
        return -1;
    }
    
    key.dptr    = serial_message;
    key.dsize   = sizeof(message->id);
    value.dptr  = serial_message;
    value.dsize = serial_message_size;
    
    status = safe_dbm_store(co, MESSAGE_DB_NAME, so->message_db_sem, &key, &value, DBM_INSERT);
    
    if (status == 1)
    {
        (void) fprintf(stdout, "Database error occurred: Message with ID \"%d\" already exists in Message database.\n",
                       *(int *) key.dptr);
    } else if (status == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    return 0;
}

static int insert_auth(struct core_object *co, struct server_object *so, Auth *auth)
{
    PRINT_STACK_TRACE(co->tracer);
    
    uint8_t       *serial_auth;
    unsigned long serial_auth_size;
    int           status;
    datum         key;
    datum         value;
    
    // Determine if an auth with the login token already exists in the database.
    status = find_by_name(co, AUTH_DB_NAME, so->auth_db_sem, NULL, auth->login_token);
    if (status == -1)
    {
        return -1;
    }
    if (status == 1)
    {
        (void) fprintf(stdout, "Database error occurred: Auth with token \"%s\" already exists in Auth database.\n",
                       auth->login_token);
        return 1;
    }
    
    serial_auth_size = serialize_auth(co, &serial_auth, auth);
    if (serial_auth_size == 0)
    {
        return -1;
    }
    
    printf("serial auth size: %lu\n", serial_auth_size);
    
    key.dptr    = serial_auth;
    key.dsize   = sizeof(auth->user_id);
    value.dptr  = serial_auth;
    value.dsize = serial_auth_size;
    
    status = safe_dbm_store(co, AUTH_DB_NAME, so->auth_db_sem, &key, &value, DBM_INSERT);
    
    if (status == 1)
    {
        (void) fprintf(stdout, "Database error occurred: Auth with ID \"%d\" already exists in Auth database.\n",
                       *(int *) key.dptr);
        return 1;
    }
    if (status == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    return 0;
}

int db_read(struct core_object *co, struct server_object *so, int type, void *object_dst, void *object_query)
{
    PRINT_STACK_TRACE(co->tracer);
    
    int read_status;
    
    switch (type)
    {
        case USER:
        {
            if (object_query)
            {
                read_status = read_user(co, so, (User **) object_dst, (const char *) object_query);
            } else
            {
                read_status = read_online_users(co, so, (User ***) object_dst);
            }
            break;
        }
        case CHANNEL:
        {
            read_status = read_channel(co, so, (Channel **) object_dst, (const char *) object_query);
            break;
        }
        case MESSAGE:
        {
            read_status = read_messages(co, so, (Message ***) object_dst,
                                        *(int *) object_query, *(((int *) object_query) + 1));
            break;
        }
        case AUTH:
        {
            read_status = read_auth(co, so, (Auth **) object_dst, (const char *) object_query);
            break;
        }
        default:
        {
            read_status = -1;
        }
    }
    
    return read_status;
}

static int read_user(struct core_object *co, struct server_object *so, User **user_get, const char *display_name)
{
    PRINT_STACK_TRACE(co->tracer);
    
    uint8_t *serial_user;
    int     read_status;
    
    // Read the database to find the User
    read_status = find_by_name(co, USER_DB_NAME, so->user_db_sem, &serial_user, display_name);
    if (read_status == -1) // Error
    {
        return -1;
    }
    if (user_get) // If the query must return something, return something.
    {
        if (read_status == 1) // User found.
        {
            *user_get = mm_malloc(sizeof(User), co->mm);
            if (!*user_get)
            {
                SET_ERROR(co->err);
                return -1;
            }
            deserialize_user(co, user_get, serial_user);
        } else if (read_status == 0) // User not found.
        {
            *user_get = NULL;
        }
    }
    
    // Return whether the object was found.
    return read_status;
}

static int read_online_users(struct core_object *co, struct server_object *so, User ***users_get)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

static int read_channel(struct core_object *co, struct server_object *so, Channel **channel_get,
                        const char *channel_name)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

static int read_messages(struct core_object *co, struct server_object *so, Message ***messages_get,
                         int num_messages, int channel_id)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

static int read_auth(struct core_object *co, struct server_object *so, Auth **auth_get, const char *login_token)
{
    PRINT_STACK_TRACE(co->tracer);
    
    uint8_t *serial_auth;
    
    if (find_by_name(co, AUTH_DB_NAME, so->auth_db_sem, &serial_auth, login_token) == -1)
    {
        return -1;
    }
    if (!serial_auth)
    {
        *auth_get = NULL;
        return 0;
    }
    
    *auth_get = mm_malloc(sizeof(Auth), co->mm);
    if (!*auth_get)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    deserialize_auth(co, auth_get, serial_auth);
    
    return 0;
}

int db_update(struct core_object *co, struct server_object *so, int type, void *object_src)
{
    PRINT_STACK_TRACE(co->tracer);
    
    switch (type) // NOLINT(hicpp-multiway-paths-covered) : TODO: temp nolint while other routes commented.
    {
        case USER:
        {
            if (update_user(co, so, (User *) object_src) == -1)
            {
                return -1;
            }
            break;
        }
//        case CHANNEL:
//        {
//            if (update_channel(co, so, (Channel *) object_src) == -1)
//            {
//                return -1;
//            }
//            break;
//        }
//        case MESSAGE:
//        {
//            if (update_message(co, so, (Channel *) object_src) == -1)
//            {
//                return -1;
//            }
//            break;
//        }
//        case AUTH:
//        {
//            if (update_auth(co, so, (Channel *) object_src) == -1)
//            {
//                return -1;
//            }
//            break;
//        }
        default:;
    }
    
    return 0;
}

static int update_user(struct core_object *co, struct server_object *so, User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    unsigned long serial_user_size;
    uint8_t       *serial_user;
    datum         key;
    datum         value;
    int           status;
    
    serial_user_size = serialize_user(co, &serial_user, user);
    if (serial_user_size == 0)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    key.dptr    = serial_user;
    key.dsize   = sizeof(int);
    value.dptr  = serial_user;
    value.dsize = serial_user_size;
    
    status = safe_dbm_store(co, USER_DB_NAME, so->user_db_sem, &key, &value, DBM_REPLACE);
    
    return status;
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
    
    key.dptr  = &user->id;
    key.dsize = sizeof(user->id);
    
    if (safe_dbm_delete(co, USER_DB_NAME, so->user_db_sem, &key) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int delete_channel(struct core_object *co, struct server_object *so, Channel *channel)
{
    PRINT_STACK_TRACE(co->tracer);
    
    datum key;
    
    key.dptr  = &channel->id;
    key.dsize = sizeof(channel->id);
    
    if (safe_dbm_delete(co, CHANNEL_DB_NAME, so->channel_db_sem, &key) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int delete_message(struct core_object *co, struct server_object *so, Message *message)
{
    PRINT_STACK_TRACE(co->tracer);
    
    datum key;
    
    key.dptr  = &message->id;
    key.dsize = sizeof(message->id);
    
    if (safe_dbm_delete(co, MESSAGE_DB_NAME, so->message_db_sem, &key) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int delete_auth(struct core_object *co, struct server_object *so, Auth *auth)
{
    PRINT_STACK_TRACE(co->tracer);
    datum key;
    
    key.dptr  = &auth->user_id;
    key.dsize = sizeof(auth->user_id);
    
    if (safe_dbm_delete(co, AUTH_DB_NAME, so->auth_db_sem, &key) == -1)
    {
        return -1;
    }
    
    return 0;
}

int safe_dbm_store(struct core_object *co, const char *db_name, sem_t *sem, datum *key, datum *value, int store_flags)
{
    DBM *db;
    int status;
    
    if (sem_wait(sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    // NOLINTBEGIN(concurrency-mt-unsafe) : Protected
    db = dbm_open(db_name, DB_FLAGS, DB_FILE_MODE);
    if (db == (DBM *) 0)
    {
        SET_ERROR(co->err);
        sem_post(sem);
        return -1;
    }
    status = dbm_store(db, *key, *value, store_flags);
    if (!key->dptr && dbm_error(db)) // NOLINT(concurrency-mt-unsafe) : No threads here
    {
        print_db_error(db);
    }
    dbm_close(db);
    // NOLINTEND(concurrency-mt-unsafe)
    sem_post(sem);
    
    return status;
}

int safe_dbm_fetch(struct core_object *co, const char *db_name, sem_t *sem, datum *key, uint8_t **serial_buffer)
{
    PRINT_STACK_TRACE(co->tracer);
    
    int   ret_val;
    DBM   *db;
    datum value;
    
    if (sem_wait(sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    // NOLINTBEGIN(concurrency-mt-unsafe) : Protected
    db = dbm_open(db_name, DB_FLAGS, DB_FILE_MODE);
    if (db == (DBM *) 0)
    {
        SET_ERROR(co->err);
        sem_post(sem);
        return -1;
    }
    value = dbm_fetch(db, (*key));
    if (!value.dptr && dbm_error(db))
    {
        print_db_error(db);
    }
    ret_val = copy_dptr_to_buffer(co, serial_buffer, &value);
    dbm_close(db);
    // NOLINTEND(concurrency-mt-unsafe) : Protected
    sem_post(sem);
    
    return ret_val;
}

int safe_dbm_delete(struct core_object *co, const char *db_name, sem_t *sem, datum *key)
{
    DBM *db;
    
    if (sem_wait(sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    // NOLINTBEGIN(concurrency-mt-unsafe) : Protected
    db = dbm_open(db_name, DB_FLAGS, DB_FILE_MODE);
    if (db == (DBM *) 0)
    {
        SET_ERROR(co->err);
        sem_post(sem);
        return -1;
    }
    if (dbm_delete(db, *key) == -1)
    {
        SET_ERROR(co->err);
        dbm_close(db);
        sem_post(sem);
        return -1;
    }
    if (!(*key).dptr && dbm_error(db))
    {
        print_db_error(db);
    }
    dbm_close(db);
    // NOLINTEND(concurrency-mt-unsafe)
    sem_post(sem);
    return 0;
}


int find_by_name(struct core_object *co, const char *db_name, sem_t *db_sem,
                 uint8_t **serial_object, const char *name)
{
    PRINT_STACK_TRACE(co->tracer);
    
    DBM   *db;
    datum key;
    datum value;
    
    // Get first thing in the db
    if (sem_wait(db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    // NOLINTBEGIN(concurrency-mt-unsafe) : Protected
    db = dbm_open(db_name, DB_FLAGS, DB_FILE_MODE);
    if (db == (DBM *) 0)
    {
        SET_ERROR(co->err);
        return -1;
    }
    key   = dbm_firstkey(db);
    value = dbm_fetch(db, key);
    if (!key.dptr && dbm_error(db))
    {
        print_db_error(db);
    }
    
    // Compare the display name to the name in the db
    while (key.dptr && strcmp((char *) (((int *) value.dptr) + 1), name) != 0)
    {
        key   = dbm_nextkey(db);
        value = dbm_fetch(db, key);
        if (!key.dptr && dbm_error(db))
        {
            print_db_error(db);
        }
    }
    
    // Returns 0 if no value.dptr, returns 1 if value.dptr, returns -1 if error.
    int ret_val = save_dptr_to_serial_object(co, serial_object, &value);
    
    dbm_close(db);
    // NOLINTEND(concurrency-mt-unsafe) : Protected
    sem_post(db_sem);
    
    //    if (key.dptr)
//    {
//        printf("id: %d\n", *(int *) key.dptr);
//        printf("size: %zu\n", value.dsize);
//        printf("name: %s\n", (char *) (((int *) value.dptr) + 1));
//    } else
//    {
//        printf("key.dptr is null\n");
//    }
    
    return ret_val;
}

int find_addr_id_pair_by_id(struct core_object *co, struct server_object *so, AddrIdPair *addr_id_pair, int id)
{
    PRINT_STACK_TRACE(co->tracer);
    
    DBM   *db;
    datum key;
    datum value;
    
    // Get first thing in the db
    if (sem_wait(so->addr_id_db_sem) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    // NOLINTBEGIN(concurrency-mt-unsafe) : Protected
    db = dbm_open(ADDR_ID_DB_NAME, DB_FLAGS, DB_FILE_MODE);
    if (db == (DBM *) 0)
    {
        SET_ERROR(co->err);
        return -1;
    }
    key   = dbm_firstkey(db);
    value = dbm_fetch(db, key);
    if (!key.dptr && dbm_error(db))
    {
        print_db_error(db);
    }
    
    // Compare the ID to the name in the db
    while (key.dptr && *(int *) ((uint8_t *) key.dptr + SOCKET_ADDR_SIZE) == id)
    {
        key   = dbm_nextkey(db);
        value = dbm_fetch(db, key);
        if (!key.dptr && dbm_error(db))
        {
            print_db_error(db);
        }
    }
    
    uint8_t *serial_object;
    // Returns 0 if no value.dptr, returns 1 if value.dptr, returns -1 if error.
    int     ret_val = save_dptr_to_serial_object(co, &serial_object, &value);
    
    dbm_close(db);
    // NOLINTEND(concurrency-mt-unsafe) : Protected
    sem_post(so->addr_id_db_sem);
    if (ret_val == 0)
    {
        deserialize_addr_id_pair(co, &addr_id_pair, serial_object);
    }
    
    return ret_val;
}

static int save_dptr_to_serial_object(struct core_object *co, uint8_t **serial_object, datum *value)
{
    PRINT_STACK_TRACE(co->tracer);
    
    int ret_val;
    
    if (value->dptr)
    {
        ret_val = 1;
        if (serial_object)
        {
            if (copy_dptr_to_buffer(co, serial_object, value) == -1)
            {
                return -1;
            }
        }
    } else
    {
        ret_val = 0;
        if (serial_object)
        {
            *serial_object = NULL;
        }
    }
    
    return ret_val;
}

int copy_dptr_to_buffer(struct core_object *co, uint8_t **buffer, datum *value)
{
    PRINT_STACK_TRACE(co->tracer);
    
    int ret_val;
    
    if (value->dptr)
    {
        *buffer = mm_malloc(value->dsize, co->mm);
        if (!*buffer)
        {
            SET_ERROR(co->err);
            return -1;
        }
        memcpy(*buffer, value->dptr, value->dsize);
        
        ret_val = 0;
    } else
    {
        ret_val = 1;
    }
    
    return ret_val;
}

void print_db_error(DBM *db)
{
    int err_code;
    
    err_code = dbm_error(db); // NOLINT(concurrency-mt-unsafe) : No threads here
    dbm_clearerr(db);         // NOLINT(concurrency-mt-unsafe) : No threads here
    
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers) : Numbers fine here.
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
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
}
