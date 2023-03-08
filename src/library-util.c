#include "../include/library-util.h"

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

/**
 * get_saddle_lib
 * <p>
 * Get the test functions from the provided library
 * </p>
 * @param state struct to store function pointers
 * @return 0 on success, -1 on failure
 */
static int get_saddle_lib(struct state *state);

int open_lib(struct state *state)
{
    PRINT_STACK_TRACE(state->tracer);
    
    state->lib = dlopen(state->lib_name, RTLD_LAZY);
    if (!state->lib)
    {
        // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
        (void) fprintf(stderr, "Fatal: Could not open test library %s: %s\n", state->lib_name, dlerror());
        return -1;
    }
    
    if (get_saddle_lib(state) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int get_saddle_lib(struct state *state)
{
    PRINT_STACK_TRACE(state->tracer);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    state->lib_main = SADDLE_FUNCTION dlsym(state->lib, SADDLE_FUNCTION_NAME);
    if (state->lib_main == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", SADDLE_FUNCTION_NAME, dlerror());
        return -1;
    }
    // NOLINTEND(concurrency-mt-unsafe)
    
    return 0;
}

int close_lib(void *lib, const char *lib_name, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    if (dlclose(lib) == -1)
    {
        // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
        (void) fprintf(stderr, "Could not close library %s: %s\n", lib_name, dlerror());
    }
    return 0;
}
