#ifndef CHAT_TEST_SADDLE_OBJECTS_H
#define CHAT_TEST_SADDLE_OBJECTS_H

#include "error.h"

#include <mem_manager/manager.h>
#include <netinet/in.h>
#include <sys/types.h>

struct state {
    char *lib_name;
    void *lib;
    struct test_functions test_functions;
    
    struct sockaddr_in addr;
    in_port_t port_number;
    
    Error err;
    void (*tracer)(const char *, const char *, size_t);
    struct memory_manager *mm;
};

#endif //CHAT_TEST_SADDLE_OBJECTS_H
