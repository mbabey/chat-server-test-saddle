#include "../include/util.h"

#include <stdlib.h>

/**
 * Bit mask for four lowest order bits.
 */
#define MASK_b1111 (unsigned int) 15

/**
 * Byte size of the data buffer to send.
 */
#define DATA_SIZE(body_size) (4 + (body_size))

/**
 * count_tokens
 * <p>
 * Count the number of ETX characters in the body.
 * </p>
 * @param body_size the number of bytes in the body
 * @param body the body
 * @param tracer tracer function
 * @return the number of ETX tokens
 */
static int count_tokens(uint16_t body_size, const char *body, void (*tracer)(const char *, const char *, size_t));

int recv_parse_message(struct state *state, int socket_fd, struct dispatch *dispatch)
{
    PRINT_STACK_TRACE(state->tracer);
    
    ssize_t bytes_read;
    
    // Get version and type (4 bits each).
    uint8_t version_and_type;
    bytes_read = recv(socket_fd, &version_and_type, sizeof(version_and_type), 0);
    if (bytes_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    dispatch->type = version_and_type & MASK_b1111;
    version_and_type >>= (unsigned int) 4; // Bit shift right 4.
    dispatch->version = version_and_type & MASK_b1111;
    
    // Get the object (8 bits).
    bytes_read = recv(socket_fd, &dispatch->object, sizeof(dispatch->object), 0);
    if (bytes_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    // Get the body size (16 bits).
    bytes_read = recv(socket_fd, &dispatch->body_size, sizeof(dispatch->body_size), 0);
    if (bytes_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    dispatch->body_size = ntohs(dispatch->body_size);
    
    // Get the body.
    dispatch->body = (char *) mm_calloc(1, dispatch->body_size + 1, state->mm);
    if (!dispatch->body)
    {
        SET_ERROR(state->err);
        return -1;
    }
    bytes_read = recv(socket_fd, dispatch->body, dispatch->body_size, 0);
    if (bytes_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    return 0;
}

int assemble_message_send(struct state *state, int socket_fd, struct dispatch *dispatch)
{
    PRINT_STACK_TRACE(state->tracer);
    
    uint8_t  *data;
    uint8_t  version_and_type;
    uint16_t body_size_network_order;
    ssize_t  bytes_sent;
    
    data = (uint8_t *) malloc(DATA_SIZE(dispatch->body_size));
    if (!data)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    // Pack the version and type.
    version_and_type = dispatch->version;
    version_and_type <<= (unsigned int) 4; // Bit shift 4 left.
    version_and_type += dispatch->type;
    *data = version_and_type;
    
    // Pack the object.
    *(data + 1) = dispatch->object;
    
    // Path the body size.
    body_size_network_order = htons(dispatch->body_size);
    memcpy(data + 2, &body_size_network_order, sizeof(body_size_network_order));
    
    // Pack the body.
    memcpy(data + 4, dispatch->body, dispatch->body_size);
    
    // Send the dispatch.
    bytes_sent = send(client->socket_fd, data, DATA_SIZE(dispatch->body_size), 0);
    free(data);
    if (bytes_sent == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    return 0;
}

int parse_body(struct state *state, char ***body_tokens, uint16_t body_size, char *body)
{
    PRINT_STACK_TRACE(state->tracer);
    
    int num_tokens;
    char *token;
    
    num_tokens = count_tokens(body_size, body, state->tracer);
    
    *body_tokens = mm_malloc((num_tokens + 1) * sizeof(char *), state->mm);
    if (!*body_tokens)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    token = strdup(body);
    mm_add(state->mm, token);
    token = strtok( token, "\x03");
    **body_tokens = token;
    for (size_t i = 1; token; ++i)
    {
        token = strtok( NULL, "\x03");
        if (token)
        {
            *(*body_tokens + i) = token;
        }
    }
    
    *(*body_tokens + num_tokens) = NULL;
    
    mm_free(state->mm, token);
    
    return 0;
}

static int count_tokens(uint16_t body_size, const char *body, void (*tracer)(const char *, const char *, size_t))
{
    PRINT_STACK_TRACE(tracer);
    
    int count;
    
    count = 0;
    for (size_t i = 0; i < body_size; ++i)
    {
        if (*(body + i) == '\x03')
        {
            ++count;
        }
    }
    
    return count;
}
