#ifndef CLIENT_TEST_SADDLE_STATE_H
#define CLIENT_TEST_SADDLE_STATE_H

#include "../../include/error-handlers.h"

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

struct client
{
    int socket_fd;
};

#endif //CLIENT_TEST_SADDLE_STATE_H
