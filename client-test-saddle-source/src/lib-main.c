#include "../../include/saddle-function.h"
#include "../include/saddle-client.h"

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
static int open_socket_connect(struct client_state *state);

/**
 * disconnect_close_socket
 * <p>
 * Close the connection and the socket.
 * </p>
 * @param state the state object
 * @param client the client object
 * @return 0 on success, -1 and set err on failure
 */
static int disconnect_close_socket(struct client_state *state);

int lib_main(void *args)
{
    struct client_state *state = (struct client_state *) args;
    PRINT_STACK_TRACE(state->tracer);
    
    if (open_socket_connect(state) == -1)
    {
        return -1;
    }
    
    if (run_client_saddle(state) == -1)
    {
        return -1;
    }
    
    if (disconnect_close_socket(state) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int open_socket_connect(struct client_state *state)
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
    
    state->socket_fd = fd;
    
    return 0;
}

static int disconnect_close_socket(struct client_state *state)
{
    PRINT_STACK_TRACE(state->tracer);
    
    if (close(state->socket_fd) == -1)
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
