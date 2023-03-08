#include "../../include/saddle-function.h"
#include "../include/test-functions.h"
#include "../include/saddle-client.h"

#include <stdlib.h>

/**
 * run_create_test
 * <p>
 * Run tests for CREATE type dispatches on all objects.
 * </p>
 * @param state
 */
static int run_create_test(struct state_minor *state, struct client *client);

/**
 * run_read_tests
 * <p>
 * Run tests for READ type dispatches on all objects.
 * </p>
 */
static void run_read_tests(struct state_minor *state, struct client *client);

/**
 * run_update_tests
 * <p>
 * Run tests for UPDATE type dispatches on all objects.
 * </p>
 */
static void run_update_tests(struct state_minor *state, struct client *client);

/**
 * run_destroy_tests
 * <p>
 * Run tests for DESTROY type dispatches on all objects.
 * </p>
 */
static void run_destroy_tests(struct state_minor *state, struct client *client);

int run_client_saddle(struct state_minor *state, struct client *client)
{
    // Run all the tests.
    // Running tests should be atomic: if one fails, it should not affect the result of the rest of the program.
    
    // Wait for user input on dispatch type
    // wait for user input on dispatch object
    
    // TODO: Loop on this switch
    switch(client->test_number)
    {
        case INVALID:
        {
            //
            (void) fprintf(stdout, "Invalid test, try again\n");
            break;
        }
        case CREATE_USER:
        case CREATE_CHANNEL:
        case CREATE_MESSAGE:
        case CREATE_AUTH:
        {
            if (run_create_test(state, client) == -1)
            {
                return -1;
            }
            break;
        }
        case READ_USER:
        case READ_CHANNEL:
        case READ_MESSAGE:
        {
            if (run_read_tests(state, client) == -1)
            {
                return -1;
            }
            break;
        }
        case UPDATE_USER:
        case UPDATE_CHANNEL:
        case UPDATE_MESSAGE:
        case UPDATE_AUTH:
        {
            if (run_update_tests(state, client) == -1)
            {
                return -1;
            }
            break;
        }
        case DESTROY_USER:
        case DESTROY_CHANNEL:
        case DESTROY_MESSAGE:
        case DESTROY_AUTH:
        {
            if (run_destroy_tests(state, client) == -1)
            {
                return -1;
            }
            break;
        }
        case STOP:
        {
            // TODO: Stop looping.
            break;
        }
    }
    
    return 0;
}

static int run_create_test(struct state_minor *state, struct client *client)
{
    PRINT_STACK_TRACE(state->tracer);
    
    switch(client->test_number)
    {
        case CREATE_USER:
        {
            create_user_test();
            break;
        }
        case CREATE_CHANNEL:
        {
            create_channel_test();
            break;
        }
        case CREATE_MESSAGE:
        {
            create_message_test();
            break;
        }
        case CREATE_AUTH:
        {
            create_auth_test();
            break;
        }
    }
    
    return 0;
}

static void run_read_tests(struct state_minor *state, struct client *client)
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

static void run_update_tests(struct state_minor *state, struct client *client)
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

static void run_destroy_tests(struct state_minor *state, struct client *client)
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
