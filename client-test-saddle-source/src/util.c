#include "../include/util.h"

/**
 * Bit mask for four lowest order bits.
 */
#define MASK_b1111 15

int recv_parse_message(struct state_minor *state, struct client *client, struct dispatch *dispatch)
{
    PRINT_STACK_TRACE(state->tracer);
    
    ssize_t bytes_read;
    
    // Get version and type (4 bits each).
    uint8_t version_and_type;
    bytes_read = recv(client->socket_fd, &version_and_type, sizeof(version_and_type), 0);
    if (bytes_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    dispatch->type    = version_and_type & MASK_b1111;
    version_and_type >>= 4; // Bit shift right 4.
    dispatch->version = version_and_type & MASK_b1111;
    
    // Get the object (8 bits).
    bytes_read = recv(client->socket_fd, &dispatch->object, sizeof(dispatch->object), 0);
    if (bytes_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    // Get the body size (16 bits).
    bytes_read = recv(client->socket_fd, &dispatch->body_size, sizeof(dispatch->body_size), 0);
    if (bytes_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    dispatch->body_size = ntohs(dispatch->body_size);
    
    // Get the body.
    dispatch->body = (char *) Mmm_calloc(1, dispatch->body_size + 1, state->mm);
    if (!dispatch->body)
    {
        SET_ERROR(state->err);
        return -1;
    }
    bytes_read = recv(client->socket_fd, dispatch->body, dispatch->body_size, 0);
    if (bytes_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    return 0;
}

int assemble_message_send(struct state_minor *state, struct client *client, struct dispatch *dispatch)
{
    PRINT_STACK_TRACE(state->tracer);
    
    // pack the first byte
    // pack the second byte
    // pack third and fourth byte
    
    return 0;
}
