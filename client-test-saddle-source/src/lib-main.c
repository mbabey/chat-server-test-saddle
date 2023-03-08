#include "../../include/saddle-functions.h"
#include "../include/state.h"

#include <stdio.h>
#include <stdlib.h>

int lib_main(void *args)
{
    struct state_minor *state = (struct state_minor *) args;
    
    // Create a socket with the address passed in state
    
    // connect to the server
    
    return 0;
}

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
    if (lib_main(NULL) == -1)
    {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
