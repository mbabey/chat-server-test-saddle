#include "../include/saddle.h"

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
    
    run = 1;
    next_state = SETUP;
    while (run)
    {
        switch (next_state)
        {
            case SETUP:
            {
                if (setup_state)
                break;
            }
            case RUN:
            {
                break;
            }
            case ERROR:
            {
                next_state = EXIT;
                break;
            }
            case EXIT:
            {
                run = 0;
                break;
            }
            default:
            {
                // Should not get here.
                break;
            }
        }
    }
    
    
    run_saddle();
    
    return EXIT_SUCCESS;
}
