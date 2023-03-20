#include "../../include/global-objects.h"
#include "../include/db.h"

/**
 * serialize_user
 * <p>
 * Serialize a User struct.
 * </p>
 * @param co the core object
 * @param serial_user the buffer into which to serialize the user
 * @param user the user to serialize
 * @return 0 on success, -1 and set err on failure
 */
static int serialize_user(struct core_object *co, uint8_t **serial_user, User *user);

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
            User    *user;
            uint8_t *serial_user;
            int     serial_user_size;
            int     insert_status;
            datum   key;
            datum   value;
            
            user = (User *) object;
            
            serial_user_size = serialize_user(&serial_user, user);
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
            if (insert_status == 1)
            {
                (void) fprintf(stdout, "Database error occurred: entry with key \"%s\" exists.\n", (char *) key.dptr);
            } else if (insert_status == -1)
            {
                print_db_error(dbm_error(so->user_db));
            }
            
            sem_post(so->user_db_sem);
            
            break;
        }
        case CHANNEL:
        {
            Channel *channel;
            
            channel = (Channel *) object;
            
            if (sem_wait(so->channel_db_sem) == -1)
            {
                SET_ERROR(co->err);
                return -1;
            }
            
            sem_post(so->channel_db_sem);
            
            break;
        }
        case MESSAGE:
        {
            Message *message;
            
            message = (Message *) object;
            
            if (sem_wait(so->message_db_sem) == -1)
            {
                SET_ERROR(co->err);
                return -1;
            }
            
            sem_post(so->message_db_sem);
            
            break;
        }
        case AUTH:
        {
            Auth *auth;
            
            auth = (Auth *) object;
            
            if (sem_wait(so->auth_db_sem) == -1)
            {
                SET_ERROR(co->err);
                return -1;
            }
            
            sem_post(so->auth_db_sem);
            
            break;
        }
        default:;
    }
    
    return 0;
}

static int serialize_user(struct core_object *co, uint8_t **serial_user, User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    size_t byte_offset;
    
    *serial_user = mm_malloc(sizeof(user->id) + strlen(user->display_name) + 1 + sizeof(user->privilege_level), co->mm);
    if (!*serial_user)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    memcpy(*serial_user, &user->id, sizeof(user->id));
    byte_offset = sizeof(user->id);
    memcpy(*(serial_user + byte_offset), user->display_name, strlen(user->display_name) + 1);
    byte_offset += strlen(user->display_name) + 1;
    memcpy(*(serial_user + byte_offset), &user->privilege_level, sizeof(user->privilege_level));
    
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
