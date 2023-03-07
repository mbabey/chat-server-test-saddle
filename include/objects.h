#ifndef CHAT_TEST_SADDLE_OBJECTS_H
#define CHAT_TEST_SADDLE_OBJECTS_H

#include "error.h"

#include <dc_env/env.h>
#include <mem_manager/manager.h>
#include <netinet/in.h>

struct state {
    Error err;
    struct dc_env *env;
    struct memory_manager *mm;
    
    struct sockaddr_in addr;
    in_port_t port_number;
    
    char *lib_name;
    void *lib;
    struct test_functions test_functions;
};



#endif //CHAT_TEST_SADDLE_OBJECTS_H
