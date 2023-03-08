#include "../include/saddle.h"

int run_saddle(struct state *state)
{
    int ret_val;
    
    ret_val = state->lib_main(state);
    
    return ret_val;
}
