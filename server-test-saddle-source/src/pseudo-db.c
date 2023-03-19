#include "../../include/global-objects.h"
#include "../include/pseudo-db.h"

int db_create(struct core_object *co, struct server_object *so, int type)
{
    PRINT_STACK_TRACE(co->tracer);
    
    // determine the type of db into which an insertion will be made
    switch (type)
    {
        case USER:
        {
            mm_malloc(sizeof(User), co->mm);
            break;
        }
        case CHANNEL:
        {
            break;
        }
        case MESSAGE:
        {
            break;
        }
        case AUTH:
        {
            break;
        }
        default:;
    }
    // make a new object of that type
    
    // realloc the db and insert the object.
    
    return 0;
}

int db_read(struct core_object *co, struct server_object *so, int type)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

int db_update(struct core_object *co, struct server_object *so, int type)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}

int db_destroy(struct core_object *co, struct server_object *so, int type)
{
    PRINT_STACK_TRACE(co->tracer);
    
    return 0;
}
