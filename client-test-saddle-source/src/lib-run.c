#include "../../saddle_functions.h"
#include "../include/test_functions.h"

#include <stdlib.h>

/**
 * run_create_tests
 * <p>
 * Run tests for CREATE type dispatches on all objects.
 * </p>
 * @param state
 */
static void run_create_tests(struct state *state);

/**
 * run_read_tests
 * <p>
 * Run tests for READ type dispatches on all objects.
 * </p>
 */
static void run_read_tests(struct state *state);

/**
 * run_update_tests
 * <p>
 * Run tests for UPDATE type dispatches on all objects.
 * </p>
 */
static void run_update_tests(struct state *state);

/**
 * run_destroy_tests
 * <p>
 * Run tests for DESTROY type dispatches on all objects.
 * </p>
 */
static void run_destroy_tests(struct state *state);

int run_saddle(struct state *state)
{
    // Run all the tests.
    // Running tests should be atomic: if one fails, it should not affect the result of the rest of the program.
    
    // Wait for user input on dispatch type
    // wait for user input on dispatch object
    
    run_create_tests(state);
    
    run_read_tests(state);
    
    run_update_tests(state);
    
    run_destroy_tests(state);
    
    return EXIT_SUCCESS;
}

static void run_create_tests(struct state *state)
{
    PRINT_STACK_TRACE(state->tracer);
    
    if (/* Appropriate Condition */)
    {
        create_user_test();
    }
    if (/* Appropriate Condition */)
    {
        create_channel_test();
    }
    if (/* Appropriate Condition */)
    {
        create_message_test();
    }
    if (/* Appropriate Condition */)
    {
        create_auth_test();
    }
}

static void run_read_tests(struct state *state)
{
    if (/* Appropriate Condition */)
    {
        read_user_test();
    }
    if (/* Appropriate Condition */)
    {
        read_channel_test();
    }
    if (/* Appropriate Condition */)
    {
        read_message_test();
    }
}

static void run_update_tests(struct state *state)
{
    if (/* Appropriate Condition */)
    {
        update_user_test();
    }
    if (/* Appropriate Condition */)
    {
        update_channel_test();
    }
    if (/* Appropriate Condition */)
    {
        update_message_test();
    }
    if (/* Appropriate Condition */)
    {
        update_auth_test();
    }
}

static void run_destroy_tests(struct state *state)
{
    if (/* Appropriate Condition */)
    {
        destroy_user_test();
    }
    if (/* Appropriate Condition */)
    {
        destroy_channel_test();
    }
    if (/* Appropriate Condition */)
    {
        destroy_message_test();
    }
    if (/* Appropriate Condition */)
    {
        destroy_auth_test();
    }
}
