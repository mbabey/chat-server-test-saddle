#ifndef CLIENT_TEST_SADDLE_CLIENT_OBJECTS_H
#define CLIENT_TEST_SADDLE_CLIENT_OBJECTS_H

#include "../../include/error-handlers.h"
#include "../../include/manager.h"

#include <netinet/in.h>

/**
 * Minor state struct. See chat-server-test-saddle/include/state.h for major.
 */
struct state_minor
{
    struct sockaddr_in    addr;
    struct error_saver    err;
    
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

#endif //CLIENT_TEST_SADDLE_CLIENT_OBJECTS_H
