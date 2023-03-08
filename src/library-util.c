#include "../include/library-util.h"

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

/**
 * get_saddle_lib
 * <p>
 * Get the test functions from the provided library
 * </p>
 * @param env environment object
 * @param lib the library in which to search
 * @param state struct to store function pointers
 * @return
 */
static int get_saddle_lib(void *lib, struct state *state, TRACER_FUNCTION_AS(tracer));

int open_lib(void **lib, const char *lib_name, struct state *state, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    *lib = dlopen(lib_name, RTLD_LAZY);
    if (!*lib)
    {
        // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
        (void) fprintf(stderr, "Fatal: Could not open test library %s: %s\n", lib_name, dlerror());
        return -1;
    }
    
    if (get_saddle_lib(*lib, state, tracer) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int get_saddle_lib(void *lib, struct state *state, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    state->lib_main = SADDLE_FUNCTION dlsym(lib, SADDLE_FUNCTION_NAME);
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
