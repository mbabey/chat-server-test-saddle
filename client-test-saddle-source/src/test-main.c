#include "../../include/saddle-function.h"
#include "../../include/error-handlers.h"
#include "../include/state.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>


static void trace_reporter(const char *file, const char *func, size_t line);

/**
 * client-test-saddle:main
 * <p>
 * Test main function for debugging.
 * </p>
 * @param argc number of arguments
 * @param argv argument vector
 * @return 0 good, 1 bad.
 */
int main(int argc, char **argv)
{
    struct state_minor state;
    in_port_t port_number;
    
    inet_pton(AF_INET, argv[1], &state.addr.sin_addr.s_addr);
    port_number = strtol(argv[2], NULL, 10);
    state.addr.sin_port   = htons(port_number);
    state.addr.sin_family = AF_INET;
    
    state.tracer = trace_reporter;
//    state.tracer = NULL;

    state.mm = init_mem_manager();
    
    if (lib_main(&state) == -1)
    {
        return EXIT_FAILURE;
    }
    
    free_mem_manager(state.mm);
    
    return EXIT_SUCCESS;
}

static void trace_reporter(const char *file, const char *func, size_t line)
{
    (void) fprintf(stdout, "TRACE: %s : %s : @ %zu\n", file, func, line);
}
