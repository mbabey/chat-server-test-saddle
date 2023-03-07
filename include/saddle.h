#ifndef CHAT_TEST_SADDLE_SADDLE_H
#define CHAT_TEST_SADDLE_SADDLE_H

#include "../test_functions.h"

#include <netinet/in.h>
#include <sys/types.h>
#include <dc_error/error.h>
#include <mem_manager/manager.h>

struct state {
    struct dc_env *env;
    struct dc_error *err;
    struct memory_manager *mm;
    
    struct sockaddr_in addr;
    in_port_t port_number;
    
    char *lib_name;
    void *lib;
    struct test_functions test_functions;
};

/**
 * setup_saddle
 * <p>
 * Parse arguments and set up the saddle state object.
 * </p>
 * @param state the sate object
 * @param argc the number of arguments
 * @param argv the arguments
 * @return 0 on success, -1 and set errno on failure
 */
int setup_saddle(struct state *state, int argc, char **argv);

int run_saddle(struct state *state);



#endif //CHAT_TEST_SADDLE_SADDLE_H
