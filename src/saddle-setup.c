#include "../include/saddle.h"
#include "../include/util.h"

#include <string.h>
#include <dlfcn.h>

int setup_saddle(struct state *state, int argc, char **argv)
{
    if (create_state(state) == -1)
    {
        return -1;
    }
    
    if (parse_args(state, argc, argv) == -1)
    {
        return -1;
    }
    
    memset(&state->test_functions, 0, sizeof(state->test_functions));
    if (open_lib(state->env, &state->lib, state->lib_name, &state->test_functions) == -1)
    {
        return -1;
    }
    
    return 0;
}


