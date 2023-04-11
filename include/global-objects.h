#ifndef CHAT_TEST_SADDLE_STATE_H
#define CHAT_TEST_SADDLE_STATE_H

#include "saddle-function.h"
#include "error-handlers.h"
#include "manager.h"

#include <netinet/in.h>
#include <sys/types.h>

/**
 * State struct containing information necessary for the execution of the program.
 * MUST not alter order of fields unless similar order is kept in test-client-source/include/state.h and
 * benchmark-server-source/include/state.h.
 */
struct state {
    struct sockaddr_in addr;
    struct error_saver err;
    TRACER_FUNCTION_AS(tracer);
    struct memory_manager *mm;
    void *stiff;
};

/**
 * Information necessary to load and run a test library.
 */
struct library {
    void *lib;
    char *lib_name;
    int (*lib_main)(void *);
};

/**
 * A Network Dispatch.
 */
struct dispatch
{
    uint8_t  version: 4;
    uint8_t  type: 4;
    uint8_t  object;
    uint16_t body_size;
    char     *body;
};

/**
 * Dispatch Types.
 */
enum Type
{
    CREATE = 1,
    READ,
    UPDATE,
    DESTROY,
    PINGUSER = 9,
    PINGCHANNEL
};

/**
 * Dispatch Objects.
 */
enum Object
{
    USER = 1,
    CHANNEL,
    MESSAGE,
    AUTH,
    CONN_USER
};

#endif //CHAT_TEST_SADDLE_STATE_H
