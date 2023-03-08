#include "saddle.h"

#include <stdlib.h>

enum States {
    SETUP,
    RUN,
    ERROR,
    EXIT
};

int main(int argc, char **argv)
{
    int run;
    int next_state;
    struct state state;

    run = 1;
    next_state = SETUP;
    while (run)
    {
        switch (next_state)
        {
            case SETUP:
            {
                next_state = (setup_saddle(&state, argc, argv) == -1) ? ERROR : RUN;
                break;
            }
            case RUN:
            {
                next_state = (run_saddle(&state) == -1) ? ERROR : EXIT;
                break;
            }
            case ERROR:
            {
                // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
                GET_ERROR(state.err);
                next_state = EXIT;
                break;
            }
            case EXIT:
            {
                exit_saddle(&state);
                run = 0;
                break;
            }
            default:; // Should not get here.
        }
    }
    
    return EXIT_SUCCESS;
}
