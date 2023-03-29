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
 * parse_body
 * <p>
 * Parse the body of a dispatch. Count the number ETX in the body and allocate
 * an array of strings in body_tokens of that count plus one. Then, tokenize
 * the body on ETX and store each token in the string array.
 * </p>
 * @param state the state object
 * @param client the client object
 * @param body_tokens the string array to store the body tokens
 * @param body_size the body size
 * @param body the body
 * @return 0 on success, -1 and set err on failure
 */
static int parse_body(struct state *state, char ***body_tokens, uint16_t body_size, char *body);

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

/**
 * object_to_string
 * <p>
 * Convert a Object numeric value to its string equivalent. If unknown Object, will return "unknown".
 * </p>
 * @param type the Object numeric to convert
 * @return the string equivalent.
 */
static const char *object_to_string(uint8_t object);

/**
 * type_to_string
 * <p>
 * Convert a Type numeric value to its string equivalent. If unknown Type, will return "unknown".
 * </p>
 * @param type the Type numeric to convert
 * @return the string equivalent.
 */
static const char *type_to_string(uint8_t type);

// TODO: this function crashes when the packet is in the wrong format; check bytes_read not 0
int recv_parse_message(struct state *state, int socket_fd, struct dispatch *dispatch, char ***body_tokens)
{
    PRINT_STACK_TRACE(state->tracer);
    
    ssize_t bytes_read;
    
    // Get version and type (4 bits each).
    uint8_t version_and_type;
    bytes_read = recv(socket_fd, &version_and_type, sizeof(version_and_type), 0);
    if (bytes_read == 0 || errno == ECONNRESET)
    {
        return 1;
    }
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
    if (bytes_read == 0 || errno == ECONNRESET)
    {
        return 1;
    }
    if (bytes_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    // Get the body size (16 bits).
    bytes_read = recv(socket_fd, &dispatch->body_size, sizeof(dispatch->body_size), 0);
    if (bytes_read == 0 || errno == ECONNRESET)
    {
        return 1;
    }
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
    if (bytes_read == 0 || errno == ECONNRESET)
    {
        return 1;
    }
    if (bytes_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    if (parse_body(state, body_tokens, dispatch->body_size, dispatch->body) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int parse_body(struct state *state, char ***body_tokens, uint16_t body_size, char *body)
{
    PRINT_STACK_TRACE(state->tracer);
    
    int  num_tokens;
    char *token;
    char *token_head;
    
    num_tokens = count_tokens(body_size, body, state->tracer);
    
    *body_tokens = mm_malloc((num_tokens + 1) * sizeof(char *), state->mm);
    if (!*body_tokens)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    token_head = strdup(body);
    token      = strtok(token_head, "\x03"); // NOLINT(concurrency-mt-unsafe) : No threads here
    
    **body_tokens = strdup(token);
    mm_add(state->mm, **body_tokens);
    for (size_t i = 1; token; ++i)
    {
        token = strtok(NULL, "\x03"); // NOLINT(concurrency-mt-unsafe) : No threads here
        if (token)
        {
            *(*body_tokens + i) = strdup(token);
            mm_add(state->mm, *(*body_tokens + i));
        }
    }
    
    *(*body_tokens + num_tokens) = NULL;
    
    free(token_head);
    
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
    if (dispatch->body_size)
    {
        memcpy(data + 4, dispatch->body, dispatch->body_size);
    }
    
    // Send the dispatch.
    bytes_sent = send(socket_fd, data, DATA_SIZE(dispatch->body_size), 0);
    free(data);
    if (bytes_sent == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    return 0;
}

void free_body_tokens(struct state *state, char **body_tokens)
{
    PRINT_STACK_TRACE(state->tracer);
    
    for (; *body_tokens != NULL; ++body_tokens)
    {
        mm_free(state->mm, *body_tokens);
    }
}

void print_dispatch(struct state *state, struct dispatch *dispatch, const char *req_res_str)
{
    PRINT_STACK_TRACE(state->tracer);
    
    const char *type_string;
    const char *object_string;
    
    type_string   = type_to_string(dispatch->type);
    object_string = object_to_string(dispatch->object);
    
//    (void) fprintf(stdout, "\n--- Dispatch %s ---\n"
//                           "Version:\t%d\n"
//                           "Type:\t\t%d (%s)\n"
//                           "Object:\t\t%d (%s)\n"
//                           "Body size:\t%d Bytes\n"
//                           "Body:\t\t%s\n\n",
//                           req_res_str,
//                           dispatch->version,
//                           dispatch->type, type_string,
//                           dispatch->object, object_string,
//                           dispatch->body_size,
//                           dispatch->body);
    
    (void) fprintf(stdout, "\n--- Dispatch %s ---\n", req_res_str);
    (void) fprintf(stdout, "Version:\t%d\n", dispatch->version);
    (void) fprintf(stdout, "Type:\t\t%d (%s)\n", dispatch->type, type_string);
    (void) fprintf(stdout, "Object:\t\t%d (%s)\n", dispatch->object, object_string);
    (void) fprintf(stdout, "Body size:\t%d Bytes\n", dispatch->body_size);
    (void) fprintf(stdout, "Body:\t\t%s\n\n", dispatch->body);
    
}

static const char *type_to_string(uint8_t type)
{
    const char *type_string;
    
    switch (type)
    {
        case CREATE:
        {
            type_string = "create";
            break;
        }
        case READ:
        {
            type_string = "read";
            break;
        }
        case UPDATE:
        {
            type_string = "update";
            break;
        }
        case DESTROY:
        {
            type_string = "destroy";
            break;
        }
        case PINGUSER:
        {
            type_string = "ping - user";
            break;
        }
        case PINGCHANNEL:
        {
            type_string = "ping - channel";
            break;
        }
        default:
        {
            type_string = "unknown";
        }
    }
    
    return type_string;
}

static const char *object_to_string(uint8_t object)
{
    const char *object_string;
    
    switch (object)
    {
        case USER:
        {
            object_string = "user";
            break;
        }
        case CHANNEL:
        {
            object_string = "channel";
            break;
        }
        case MESSAGE:
        {
            object_string = "message";
            break;
        }
        case AUTH:
        {
            object_string = "auth";
            break;
        }
        default:
        {
            object_string = "unknown";
        }
    }
    
    return object_string;
}
