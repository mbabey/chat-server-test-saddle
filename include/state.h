#ifndef CHAT_TEST_SADDLE_STATE_H
#define CHAT_TEST_SADDLE_STATE_H

#include "saddle_functions.h"
#include "error-handlers.h"

#include <mem_manager/manager.h>
#include <netinet/in.h>
#include <sys/types.h>

struct state {
    char *lib_name;
    void *lib;
    struct saddle_lib saddle_lib;
    
    struct sockaddr_in addr;
    
    struct memory_manager *mm;
    struct error_saver err;
    TRACER_FUNCTION_AS(tracer);
};

#endif //CHAT_TEST_SADDLE_STATE_H
