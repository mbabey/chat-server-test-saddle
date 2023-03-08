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
 * @param saddle_lib struct to store function pointers
 * @return
 */
static void get_saddle_lib(void *lib, struct saddle_lib *saddle_lib, TRACER_FUNCTION_AS(tracer));

/**
 * get_create_tests
 * <p>
 * Get CREATE type network operation tests.
 * </p>
 * @param env the environment object
 * @param lib the library in which to find the functions
 * @param saddle_lib struct to store function pointers
 * @return 0 on success, -1 and set errno on failure.
 */
static int get_create_tests(void *lib, struct saddle_lib *saddle_lib, TRACER_FUNCTION_AS(tracer));

int open_lib(void **lib, const char *lib_name, struct saddle_lib *saddle_lib, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    *lib = dlopen(lib_name, RTLD_LAZY);
    if (!*lib)
    {
        // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
        (void) fprintf(stderr, "Fatal: Could not open test library %s: %s\n", lib_name, dlerror());
        return -1;
    }
    
    get_saddle_lib(*lib, saddle_lib, tracer);

    return 0;
}

static void get_saddle_lib(void *lib, struct saddle_lib *saddle_lib, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    get_create_tests(lib, saddle_lib, tracer);
}

static int get_create_tests(void *lib, struct saddle_lib *saddle_lib, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    saddle_lib->create_user_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_CREATE_USER);
    if (saddle_lib->create_user_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_CREATE_USER, dlerror());
        return -1;
    }
    
    saddle_lib->create_channel_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_CREATE_CHANNEL);
    if (saddle_lib->create_channel_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_CREATE_CHANNEL, dlerror());
        return -1;
    }
    
    saddle_lib->create_message_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_CREATE_MESSAGE);
    if (saddle_lib->create_message_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_CREATE_MESSAGE, dlerror());
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
