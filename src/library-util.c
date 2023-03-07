#include "../include/library-util.h"

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

/**
 * Function signature type for test functions.
 */
#define TEST_FUNCTION (int (*)(void))

/**
 * get_test_functions
 * <p>
 * Get the test functions from the provided library
 * </p>
 * @param env environment object
 * @param lib the library in which to search
 * @param test_functions struct to store function pointers
 * @return
 */
static void get_test_functions(void *lib, struct test_functions *test_functions, TRACER_FUNCTION_AS(tracer));

/**
 * get_create_tests
 * <p>
 * Get CREATE type network operation tests.
 * </p>
 * @param env the environment object
 * @param lib the library in which to find the functions
 * @param test_functions struct to store function pointers
 * @return 0 on success, -1 and set errno on failure.
 */
static void get_create_tests(void *lib, struct test_functions *test_functions, TRACER_FUNCTION_AS(tracer));

/**
 * get_read_tests
 * <p>
 * Get READ type network operation tests.
 * </p>
 * @param env the environment object
 * @param lib the library in which to find the functions
 * @param test_functions struct to store function pointers
 * @return 0 on success, -1 and set errno on failure.
 */
static void get_read_tests(void *lib, struct test_functions *test_functions, TRACER_FUNCTION_AS(tracer));

/**
 * get_update_tests
 * <p>
 * Get UPDATE type network operation tests.
 * </p>
 * @param env the environment object
 * @param lib the library in which to find the functions
 * @param test_functions struct to store function pointers
 * @return 0 on success, -1 and set errno on failure.
 */
static void get_update_tests(void *lib, struct test_functions *test_functions, TRACER_FUNCTION_AS(tracer));

/**
 * get_destroy_tests
 * <p>
 * Get DESTROY type network operation tests.
 * </p>
 * @param env the environment object
 * @param lib the library in which to find the functions
 * @param test_functions struct to store function pointers
 * @return 0 on success, -1 and set errno on failure.
 */
static void get_destroy_tests(void *lib, struct test_functions *test_functions, TRACER_FUNCTION_AS(tracer));

int open_lib(void **lib, const char *lib_name, struct test_functions *test_functions, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    *lib = dlopen(lib_name, RTLD_LAZY);
    if (!*lib)
    {
        // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
        (void) fprintf(stderr, "Fatal: Could not open test library %s: %s\n", lib_name, dlerror());
        return -1;
    }
    
    get_test_functions(*lib, test_functions, tracer);

    return 0;
}

static void get_test_functions(void *lib, struct test_functions *test_functions, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    get_create_tests(lib, test_functions, tracer);
    get_read_tests(lib, test_functions, tracer);
    get_update_tests(lib, test_functions, tracer);
    get_destroy_tests(lib, test_functions, tracer);
}

static void get_create_tests(void *lib, struct test_functions *test_functions, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    test_functions->create_user_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_CREATE_USER);
    if (test_functions->create_user_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_CREATE_USER, dlerror());
    }
    
    test_functions->create_channel_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_CREATE_CHANNEL);
    if (test_functions->create_channel_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_CREATE_CHANNEL, dlerror());
    }
    
    test_functions->create_message_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_CREATE_MESSAGE);
    if (test_functions->create_message_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_CREATE_MESSAGE, dlerror());
    }
    
    test_functions->create_auth_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_CREATE_AUTH);
    if (test_functions->create_auth_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_CREATE_AUTH, dlerror());
    }
    // NOLINTEND(concurrency-mt-unsafe)
}

static void get_read_tests(void *lib, struct test_functions *test_functions, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    
    test_functions->read_user_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_READ_USER);
    if (test_functions->read_user_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_READ_USER, dlerror());
    }
    
    test_functions->read_channel_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_READ_CHANNEL);
    if (test_functions->read_channel_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_READ_CHANNEL, dlerror());
    }
    
    test_functions->read_message_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_READ_MESSAGE);
    if (test_functions->read_message_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_READ_MESSAGE, dlerror());
    }
    // NOLINTEND(concurrency-mt-unsafe)
}

static void get_update_tests(void *lib, struct test_functions *test_functions, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    
    test_functions->update_user_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_UPDATE_USER);
    if (test_functions->update_user_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_UPDATE_USER, dlerror());
    }
    
    test_functions->update_channel_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_UPDATE_CHANNEL);
    if (test_functions->update_channel_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_UPDATE_CHANNEL, dlerror());
    }
    
    test_functions->update_message_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_UPDATE_MESSAGE);
    if (test_functions->update_message_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_UPDATE_MESSAGE, dlerror());
    }
    
    test_functions->update_auth_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_UPDATE_AUTH);
    if (test_functions->update_auth_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_UPDATE_AUTH, dlerror());
    }
    // NOLINTEND(concurrency-mt-unsafe)
}

static void get_destroy_tests(void *lib, struct test_functions *test_functions, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    
    test_functions->destroy_user_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_DESTROY_USER);
    if (test_functions->destroy_user_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_DESTROY_USER, dlerror());
    }
    
    test_functions->destroy_channel_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_DESTROY_CHANNEL);
    if (test_functions->destroy_channel_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_DESTROY_CHANNEL, dlerror());
    }
    
    test_functions->destroy_message_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_DESTROY_MESSAGE);
    if (test_functions->destroy_message_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_DESTROY_MESSAGE, dlerror());
    }
    
    test_functions->destroy_auth_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_DESTROY_AUTH);
    if (test_functions->destroy_auth_test == NULL)
    {
        (void) fprintf(stdout, "Could not load function %s: %s\n", TEST_FUNCTION_DESTROY_AUTH, dlerror());
    }
    // NOLINTEND(concurrency-mt-unsafe)
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
