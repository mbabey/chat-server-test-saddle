#include "../../saddle_functions.h"

#include <stdlib.h>

/**
 * run_create_tests
 * <p>
 * Run tests for CREATE type dispatches on all objects.
 * </p>
 * @param state
 */
static void run_create_tests(struct state *state, struct test_functions *test_functions);

/**
 * run_read_tests
 * <p>
 * Run tests for READ type dispatches on all objects.
 * </p>
 */
static void run_read_tests(struct state *state, struct test_functions *test_functions);

/**
 * run_update_tests
 * <p>
 * Run tests for UPDATE type dispatches on all objects.
 * </p>
 */
static void run_update_tests(struct state *state, struct test_functions *test_functions);

/**
 * run_destroy_tests
 * <p>
 * Run tests for DESTROY type dispatches on all objects.
 * </p>
 */
static void run_destroy_tests(struct state *state, struct test_functions *test_functions);

int run_saddle(struct state *state)
{
    // Run all the tests.
    // Running tests should be atomic: if one fails, it should not affect the result of the rest of the program.
    
    run_create_tests(state, &state->test_functions);
    
    run_read_tests(state, &state->test_functions);
    
    run_update_tests(state, &state->test_functions);
    
    run_destroy_tests(state, &state->test_functions);
    
    return EXIT_SUCCESS;
}

static void run_create_tests(struct state *state, struct test_functions *test_functions)
{
    PRINT_STACK_TRACE(state->tracer);
    
    if (test_functions->create_user_test)
    {
        test_functions->create_user_test();
    }
    if (test_functions->create_channel_test)
    {
        test_functions->create_channel_test();
    }
    if (test_functions->create_message_test)
    {
        test_functions->create_message_test();
    }
    if (test_functions->create_auth_test)
    {
        test_functions->create_auth_test();
    }
}

static void run_read_tests(struct state *state, struct test_functions *test_functions)
{
    if (test_functions->read_user_test)
    {
        test_functions->read_user_test();
    }
    if (test_functions->read_channel_test)
    {
        test_functions->read_channel_test();
    }
    if (test_functions->read_message_test)
    {
        test_functions->read_message_test();
    }
}

static void run_update_tests(struct state *state, struct test_functions *test_functions)
{
    if (test_functions->update_user_test)
    {
        test_functions->update_user_test();
    }
    if (test_functions->update_channel_test)
    {
        test_functions->update_channel_test();
    }
    if (test_functions->update_message_test)
    {
        test_functions->update_message_test();
    }
    if (test_functions->update_auth_test)
    {
        test_functions->update_auth_test();
    }
}

static void run_destroy_tests(struct state *state, struct test_functions *test_functions)
{
    if (test_functions->destroy_user_test)
    {
        test_functions->destroy_user_test();
    }
    if (test_functions->destroy_channel_test)
    {
        test_functions->destroy_channel_test();
    }
    if (test_functions->destroy_message_test)
    {
        test_functions->destroy_message_test();
    }
    if (test_functions->destroy_auth_test)
    {
        test_functions->destroy_auth_test();
    }
}
