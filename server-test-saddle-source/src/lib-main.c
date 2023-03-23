#include "../../include/saddle-function.h"
#include "../include/core.h"
#include "../include/server-state.h"

#include <string.h>
#include <unistd.h>

int lib_main(void *args)
{
    int                next_state;
    int                run;
    struct core_object *co = (struct core_object *) args;
    
    PRINT_STACK_TRACE(co->tracer);
    
    
    run        = 1;
    next_state = INITIALIZE_SERVER;
    while (run)
    {
        switch (next_state)
        {
            case INITIALIZE_SERVER:
            {
                next_state = initialize_server(co);
                break;
            }
            case RUN_SERVER:
            {
                next_state = run_server(co);
                break;
            }
            case CLOSE_SERVER:
            {
                next_state = close_server(co);
                break;
            }
            case ERROR:
            {
                pid_t pid;
                
                pid = getpid();
                // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
                (void) fprintf(stderr, "Fatal: error during process %d runtime: ", pid);
                GET_ERROR(co->err);
                next_state = close_server(co);
                break;
            }
            case EXIT:
            {
                run = 0;
                break;
            }
            default: // Should not get here.
            {
                run = 0;
            }
        }
    }
    
    return next_state;
}
