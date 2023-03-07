#include "../include/saddle.h"
#include "../include/library-util.h"

void exit_saddle(struct state *state)
{
    close_lib(state->lib, state->lib_name, state->tracer);
    
    free_mem_manager(state->mm);
}
