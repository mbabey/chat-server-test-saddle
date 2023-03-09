#ifndef CLIENT_TEST_SADDLE_STATE_H
#define CLIENT_TEST_SADDLE_STATE_H

#include "../../include/error-handlers.h"

#include <mem_manager/manager.h>
#include <netinet/in.h>

/**
 * Minor state struct. See chat-server-test-saddle/include/state.h for major.
 */
struct state_minor
{
    struct sockaddr_in addr;
    struct error_saver err;
    TRACER_FUNCTION_AS(tracer);
    struct memory_manager *mm;
};

/**
 * Information necessary to run the client.
 */
struct client
{
    int socket_fd;
    int test_number;
};

/**
 * A Network Dispatch.
 */
struct dispatch {
    uint8_t version : 4;
    uint8_t type : 4;
    uint8_t object;
    uint16_t body_size;
    char *body;
};

#endif //CLIENT_TEST_SADDLE_STATE_H
