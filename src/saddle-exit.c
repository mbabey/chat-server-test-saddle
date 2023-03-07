#include "../include/saddle.h"
#include "../include/util.h"

int exit_saddle(struct state *state)
{
    close_lib(state->lib, state->lib_name, state->tracer);
    
    free_mem_manager(state->mm);
    
    return 0;
}
