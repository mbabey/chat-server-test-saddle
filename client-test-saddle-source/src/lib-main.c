#include "../../include/saddle-function.h"
#include "../include/state.h"
#include

#include <unistd.h>

/**
 * open_socket_connect
 * <p>
 * Create a socket and connect to an address on that socket.
 * </p>
 * @param state the state object
 * @param client the client object
 * @return 0 on success, -1 and set err on failure
 */
static int open_socket_connect(struct state_minor *state, struct client *client);

/**
 * disconnect_close_socket
 * <p>
 * Close the connection and the socket.
 * </p>
 * @param state the state object
 * @param client the client object
 * @return 0 on success, -1 and set err on failure
 */
static int disconnect_close_socket(struct state_minor *state, struct client *client);

int lib_main(void *args)
{
    struct state_minor *state = (struct state_minor *) args;
    PRINT_STACK_TRACE(state->tracer);
    struct client client;
    
    if (open_socket_connect(state, &client) == -1)
    {
        return -1;
    }
    
    if (run_client_saddle(state, &client) == -1)
    {
        return -1;
    }
    
    if (disconnect_close_socket(state, &client) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int open_socket_connect(struct state_minor *state, struct client *client)
{
    PRINT_STACK_TRACE(state->tracer);
    
    int       fd;
    socklen_t socklen;
    
    fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    socklen = sizeof(state->addr);
    if (connect(fd, (struct sockaddr *) &state->addr, socklen) == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    client->socket_fd = fd;
    
    return 0;
}

static int disconnect_close_socket(struct state_minor *state, struct client *client)
{
    PRINT_STACK_TRACE(state->tracer);
    
    if (close(client->socket_fd) == -1)
    {
        switch (errno)
        {
            case EBADF: // Not a problem.
            {
                errno = 0;
                break;
            }
            default:
            {
                SET_ERROR(state->err);
                return -1;
            }
        }
    }
    
    return 0;
}
