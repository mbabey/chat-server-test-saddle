#include "../../include/global-objects.h"
#include "../include/broadcast.h"
#include "../include/db.h"

#include <pthread.h>
#include <stdlib.h>

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
    
    
    // Join each thread.
    for (size_t i = 0; i < channel->users_count; ++i)
    {
        pthread_join(*(thread_array + i), NULL);
    }
    
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
    uint8_t              *serial_user;
    AddrIdPair           *addr_id_pair;
    int                  read_status;
    struct sockaddr_in   socket;
    
    co           = ((struct forward_message_args *) args)->co;
    so           = ((struct forward_message_args *) args)->so;
    dispatch     = ((struct forward_message_args *) args)->dispatch;
    display_name = ((struct forward_message_args *) args)->display_name;
    
    PRINT_STACK_TRACE(co->tracer);
    
    // Get the user by name.
    read_status = find_by_name(co, USER_DB_NAME, so->user_db_sem, &serial_user, display_name);
    if (read_status == -1)
    {
        // return -1; TODO ??
    }
    if (read_status == 0)
    {
        // return 0; TODO ??
    }
    // Get the IP address of by user ID.
    addr_id_pair = malloc(sizeof(AddrIdPair));
    if (!addr_id_pair)
    {
        SET_ERROR(co->err);
        // return -1; TODO ??
    }
    if (find_addr_id_pair_by_id(co, so, addr_id_pair, *(int *) serial_user) == -1)
    {
        // return -1; TODO ??
    }
    
    // Send to that IP address.
    for (size_t i = 0; i < MAX_CONNECTIONS; ++i) // Haha version 1
    {
        if (so->parent->client_addrs[i].sin_addr.s_addr == addr_id_pair->socket_ip
            && so->parent->client_addrs[i].sin_port == addr_id_pair->socket_port) // If the port and the ip match...
        {
            // this fucking process doesn't even have the sockets of all the dudes.
            // are you fucking serious
            // this is ridiculous. This is unimplementable.
        }
    }
    
    // Wait for the response.
    
    return NULL;
}

