#ifndef CHAT_TEST_SADDLE_STATE_H
#define CHAT_TEST_SADDLE_STATE_H

#include "saddle-functions.h"
#include "error-handlers.h"

#include <mem_manager/manager.h>
#include <netinet/in.h>
#include <sys/types.h>

/**
 * State struct containing information necessary for the execution of the program.
 * MUST not alter order of fields unless similar order is kept in client-test-saddle-source/include/state.h and
 * client-test-saddle-source/include/state.h.
 */
struct state {
    struct sockaddr_in addr;
    struct error_saver err;
    TRACER_FUNCTION_AS(tracer);
    struct memory_manager *mm;
    
    void *lib;
    char *lib_name;
    int (*lib_main)(void *);
};

#endif //CHAT_TEST_SADDLE_STATE_H
