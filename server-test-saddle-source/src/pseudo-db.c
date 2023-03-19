#include "../../include/global-objects.h"
#include "../include/pseudo-db.h"

int db_create(struct core_object *co, struct server_object *so, int type, void *object)
{
    PRINT_STACK_TRACE(co->tracer);
    
    switch (type)
    {
        case USER:
        {
            User *new_user;
            
            new_user = mm_malloc(sizeof(User), co->mm);
            if (!new_user)
            {
                SET_ERROR(co->err);
                return -1;
            }
            
            if (sem_wait(so->user_db_sem) == -1)
            {
                SET_ERROR(co->err);
                return -1;
            }
            
            so->user_db = mm_realloc(so->user_db, (so->user_db_size + 1) * sizeof(User), co->mm);
            if (!so->user_db)
            {
                SET_ERROR(co->err);
                return -1;
            }
    
            *(so->user_db + ++so->user_db_size) = new_user;
            
            break;
        }
        case CHANNEL:
        {
            Channel *new_channel;
    
            new_channel = mm_malloc(sizeof(Channel), co->mm);
            if (!new_channel)
            {
                SET_ERROR(co->err);
                return -1;
            }
    
            if (sem_wait(so->channel_db_sem) == -1)
            {
                SET_ERROR(co->err);
                return -1;
            }
    
            so->channel_db = mm_realloc(so->channel_db, (so->channel_db_size + 1) * sizeof(Channel), co->mm);
            if (!so->channel_db)
            {
                SET_ERROR(co->err);
                return -1;
            }
    
            *(so->channel_db + ++so->channel_db_size) = new_channel;
            
            break;
        }
        case MESSAGE:
        {
            Message *new_message;
    
            new_message = mm_malloc(sizeof(Message), co->mm);
            if (!new_message)
            {
                SET_ERROR(co->err);
                return -1;
            }
    
            if (sem_wait(so->message_db_sem) == -1)
            {
                SET_ERROR(co->err);
                return -1;
            }
    
            so->message_db = mm_realloc(so->message_db, (so->message_db_size + 1) * sizeof(Message), co->mm);
            if (!so->message_db)
            {
                SET_ERROR(co->err);
                return -1;
            }
    
            *(so->message_db + ++so->message_db_size) = new_message;
            
            break;
        }
        case AUTH:
        {
            Auth *new_auth;
    
            new_auth = mm_malloc(sizeof(Auth), co->mm);
            if (!new_auth)
            {
                SET_ERROR(co->err);
                return -1;
            }
    
            if (sem_wait(so->auth_db_sem) == -1)
            {
                SET_ERROR(co->err);
                return -1;
            }
    
            so->auth_db = mm_realloc(so->auth_db, (so->auth_db_size + 1) * sizeof(Auth), co->mm);
            if (!so->auth_db)
            {
                SET_ERROR(co->err);
                return -1;
            }
    
            *(so->auth_db + ++so->auth_db_size) = new_auth;
            break;
        }
        default:;
    }
    
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
