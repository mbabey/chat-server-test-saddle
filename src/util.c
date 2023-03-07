#include "../include/util.h"

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
static int get_test_functions(const struct dc_env *env, void *lib, struct test_functions *test_functions);

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
static int get_create_tests(const struct dc_env *env, void *lib, struct test_functions *test_functions);

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
static int get_read_tests(const struct dc_env *env, void *lib, struct test_functions *test_functions);

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
static int get_update_tests(const struct dc_env *env, void *lib, struct test_functions *test_functions);

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
static int get_destroy_tests(const struct dc_env *env, void *lib, struct test_functions *test_functions);

int open_lib(const struct dc_env *env, void **lib, const char *lib_name, struct test_functions *test_functions)
{
    DC_TRACE(env);
    
    *lib = dlopen(lib_name, RTLD_LAZY);
    if (!*lib)
    {
        // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
        (void) fprintf(stderr, "Fatal: could not open API library %s: %s\n", lib_name, dlerror());
        return -1;
    }
    
    if (get_test_functions(env, *lib, test_functions) == -1)
    {
        close_lib(env, lib, lib_name);
        return -1;
    }
    
    return 0;
}

static int get_test_functions(const struct dc_env *env, void *lib, struct test_functions *test_functions)
{
    DC_TRACE(env);
    
    if (get_create_tests(env, lib, test_functions) == -1)
    {
        return -1;
    }
    if (get_read_tests(env, lib, test_functions) == -1)
    {
        return -1;
    }
    if (get_update_tests(env, lib, test_functions) == -1)
    {
        return -1;
    }
    if (get_destroy_tests(env, lib, test_functions) == -1)
    {
        return -1;
    }
    return 0;
}

static int get_create_tests(const struct dc_env *env, void *lib, struct test_functions *test_functions)
{
    DC_TRACE(env);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    test_functions->create_user_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_CREATE_USER);
    if (test_functions->create_user_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_CREATE_USER, strerror(errno));
        return -1;
    }
    test_functions->create_channel_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_CREATE_CHANNEL);
    if (test_functions->create_channel_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_CREATE_CHANNEL, strerror(errno));
        return -1;
    }
    test_functions->create_message_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_CREATE_MESSAGE);
    if (test_functions->create_message_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_CREATE_MESSAGE, strerror(errno));
        return -1;
    }
    test_functions->create_auth_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_CREATE_AUTH);
    if (test_functions->create_auth_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_CREATE_AUTH, strerror(errno));
        return -1;
    }
    // NOLINTEND(concurrency-mt-unsafe)
    
    return 0;
}

static int get_read_tests(const struct dc_env *env, void *lib, struct test_functions *test_functions)
{
    DC_TRACE(env);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    test_functions->read_user_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_READ_USER);
    if (test_functions->read_user_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_READ_USER, strerror(errno));
        return -1;
    }
    test_functions->read_channel_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_READ_CHANNEL);
    if (test_functions->read_channel_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_READ_CHANNEL, strerror(errno));
        return -1;
    }
    test_functions->read_message_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_READ_MESSAGE);
    if (test_functions->read_message_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_READ_MESSAGE, strerror(errno));
        return -1;
    }
    // NOLINTEND(concurrency-mt-unsafe)
    
    return 0;
}

static int get_update_tests(const struct dc_env *env, void *lib, struct test_functions *test_functions)
{
    DC_TRACE(env);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    test_functions->update_user_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_UPDATE_USER);
    if (test_functions->update_user_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_UPDATE_USER, strerror(errno));
        return -1;
    }
    test_functions->update_channel_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_UPDATE_CHANNEL);
    if (test_functions->update_channel_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_UPDATE_CHANNEL, strerror(errno));
        return -1;
    }
    test_functions->update_message_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_UPDATE_MESSAGE);
    if (test_functions->update_message_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_UPDATE_MESSAGE, strerror(errno));
        return -1;
    }
    test_functions->update_auth_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_UPDATE_AUTH);
    if (test_functions->update_auth_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_UPDATE_AUTH, strerror(errno));
        return -1;
    }
    // NOLINTEND(concurrency-mt-unsafe)
    
    return 0;
}

static int get_destroy_tests(const struct dc_env *env, void *lib, struct test_functions *test_functions)
{
    DC_TRACE(env);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    test_functions->destroy_user_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_DESTROY_USER);
    if (test_functions->destroy_user_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_DESTROY_USER, strerror(errno));
        return -1;
    }
    test_functions->destroy_channel_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_DESTROY_CHANNEL);
    if (test_functions->destroy_channel_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_DESTROY_CHANNEL, strerror(errno));
        return -1;
    }
    test_functions->destroy_message_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_DESTROY_MESSAGE);
    if (test_functions->destroy_message_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_DESTROY_MESSAGE, strerror(errno));
        return -1;
    }
    test_functions->destroy_auth_test = TEST_FUNCTION dlsym(lib, TEST_FUNCTION_DESTROY_AUTH);
    if (test_functions->destroy_auth_test == NULL)
    {
        (void) fprintf(stderr, "Fatal: could not load function %s: %s\n",
                       TEST_FUNCTION_DESTROY_AUTH, strerror(errno));
        return -1;
    }
    // NOLINTEND(concurrency-mt-unsafe)
    
    return 0;
}

int close_lib(const struct dc_env *env, void *lib, const char *lib_name)
{
    DC_TRACE(env);
    
    if (dlclose(lib) == -1)
    {
        (void) fprintf(stderr, "Fatal: could not close library %s: %s\n", lib_name, dlerror());
    }
    return 0;
}
