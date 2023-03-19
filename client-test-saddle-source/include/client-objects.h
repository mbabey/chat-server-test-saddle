#ifndef CLIENT_TEST_SADDLE_CLIENT_OBJECTS_H
#define CLIENT_TEST_SADDLE_CLIENT_OBJECTS_H

#include "../../include/error-handlers.h"
#include "../../include/manager.h"

#include <netinet/in.h>

/**
 * Information necessary to run the Client test saddle. A type of state struct.
 */
struct client_state
{
    struct sockaddr_in    addr;
    struct error_saver    err;
    
    TRACER_FUNCTION_AS(tracer);
    
    struct memory_manager *mm;
    
    int socket_fd;
    int test_number;
};

#endif //CLIENT_TEST_SADDLE_CLIENT_OBJECTS_H
