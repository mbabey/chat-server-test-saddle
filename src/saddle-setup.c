#include "../include/saddle.h"
#include "../include/util.h"

#include <dlfcn.h>
#include <string.h>

static int create_state(struct state *state);

static void trace_reporter(const char *file, const char *func, size_t line);

static int parse_args(struct state *state, int argc, char **argv);

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
        SET_ERROR(state->err);
        return -1;
    }
    
    return 0;
}

static int create_state(struct state *state)
{
    
    
    
    
    
    return 0;
}

static void trace_reporter(const char *file, const char *func, size_t line)
{
    (void) fprintf(stdout, "TRACE: %s : %s : @ %zu\n", file, func, line);
}

static int parse_args(struct state *state, int argc, char **argv)
{
    state->tracer = trace_reporter;
    
    return 0;
}

