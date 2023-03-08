#include "../../include/saddle-functions.h"
#include "../include/state.h"

/**
 * open_socket_connect
 * <p>
 * Create a socket and connect to an IP address on that socket.
 * </p>
 * @param state the state object
 * @param client the client object
 * @return 0 on success, -1 and set err on failure
 */
static int open_socket_connect(struct state_minor *state, struct client *client);

int lib_main(void *args)
{
    struct state_minor *state = (struct state_minor *) args;
    struct client client;
    
    if (open_socket_connect(state, &client) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int open_socket_connect(struct state_minor *state, struct client *client)
{
    int fd;
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
