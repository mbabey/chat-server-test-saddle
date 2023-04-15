#include "../../include/global-objects.h"
#include "../include/broadcast.h"
#include "../include/db.h"

#include <pthread.h>

static void *forward_message(void *);

int broadcast_message_to_channel(struct core_object *co, struct server_object *so, struct dispatch *dispatch,
                                 char **body_tokens)
{
    PRINT_STACK_TRACE(co->tracer);
    
    char    *channel_name;
    Channel *channel;
    int     read_status;
    
    channel_name = *(body_tokens + 1);
    
    channel = mm_malloc(sizeof(Channel), co->mm);
    if (!channel)
    {
        SET_ERROR(co->err);
        return -1;
    }
    // Read the channel from the database
    read_status = db_read(co, so, CHANNEL, &channel, channel_name);
    if (read_status == -1)
    {
        mm_free(co->mm, channel);
        return -1;
    }
    
    // Make a thread for each user in the database
    pthread_t                   *thread_array;
    struct forward_message_args *args_array;
    
    
    thread_array = mm_malloc(channel->users_count * sizeof(pthread_t), co->mm);
    args_array   = mm_malloc(channel->users_count * sizeof(struct forward_message_args), co->mm);
    if (!thread_array || !args_array)
    {
        SET_ERROR(co->err);
        mm_free(co->mm, channel);
        mm_free(co->mm, thread_array);
        mm_free(co->mm, args_array);
        return -1;
    }
    
    for (size_t i = 0; i < channel->users_count; ++i)
    {
        struct forward_message_args *args;
        // set up forward message args.
        args = args_array + i;
        
        args->co           = co;
        args->so           = so;
        args->dispatch     = dispatch;
        args->display_name = *(channel->users + i);
        
        pthread_create(thread_array + i, NULL, forward_message, args);
    }
    
    // Get the user by name.
    // Get the IP address of by user ID.
    // Send to that IP address.
    // Wait for the response.
    
    // Join each thread.
    mm_free(co->mm, channel);
    mm_free(co->mm, thread_array);
    mm_free(co->mm, args_array);
    return 0;
}

void *forward_message(void *args)
{
    struct core_object   *co;
    struct server_object *so;
    struct dispatch      *dispatch;
    const char           *display_name;
    
    co           = ((struct forward_message_args *) args)->co;
    so           = ((struct forward_message_args *) args)->so;
    dispatch     = ((struct forward_message_args *) args)->dispatch;
    display_name = ((struct forward_message_args *) args)->display_name;
    
    PRINT_STACK_TRACE(co->tracer);
    
    
    
    return NULL;
}

