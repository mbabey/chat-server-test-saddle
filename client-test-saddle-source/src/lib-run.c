#include "../include/test-functions.h"
#include "../include/saddle-client.h"
#include "../include/ui.h"

/**
 * run_create_test
 * <p>
 * Run tests for CREATE type dispatches on all objects.
 * </p>
 * @param state
 */
static int run_create_test(struct client_state *state);

/**
 * run_read_test
 * <p>
 * Run tests for READ type dispatches on all objects.
 * </p>
 */
static int run_read_test(struct client_state *state);

/**
 * run_update_test
 * <p>
 * Run tests for UPDATE type dispatches on all objects.
 * </p>
 */
static int run_update_test(struct client_state *state);

/**
 * run_destroy_test
 * <p>
 * Run tests for DESTROY type dispatches on all objects.
 * </p>
 */
static int run_destroy_test(struct client_state *state);

int run_client_saddle(struct client_state *state)
{
    PRINT_STACK_TRACE(state->tracer);
    
    int run;
    
    run = 1;
    while (run)
    {
        if (run_ui(state) == -1)
        {
            SET_ERROR(state->err);
            return -1;
        }
        
        switch (state->test_number)
        {
            case INVALID:
            {
                (void) fprintf(stdout, "Invalid test, try again.\n");
                break;
            }
            case CREATE_USER:
            case CREATE_CHANNEL:
            case CREATE_MESSAGE:
            case CREATE_AUTH:
            {
                if (run_create_test(state) == -1)
                {
                    return -1;
                }
                break;
            }
            case READ_USER:
            case READ_CHANNEL:
            case READ_MESSAGE:
            {
                if (run_read_test(state) == -1)
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
                if (run_update_test(state) == -1)
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
                if (run_destroy_test(state) == -1)
                {
                    return -1;
                }
                break;
            }
            case STOP:
            {
                run = 0;
                break;
            }
        }
    
        if (state->test_number != STOP)
        {
            display_results(state);
        }
    }
    
    return 0;
}

static int run_create_test(struct client_state *state)
{
    PRINT_STACK_TRACE(state->tracer);
    
    switch (state->test_number)
    {
        case CREATE_USER:
        {
            if (create_user_test(state) == -1)
            {
                return -1;
            }
            break;
        }
        case CREATE_CHANNEL:
        {
            if (create_channel_test(state) == -1)
            {
                return -1;
            }
            break;
        }
        case CREATE_MESSAGE:
        {
            if (create_message_test(state) == -1)
            {
                return -1;
            }
            break;
        }
        case CREATE_AUTH:
        {
            if (create_auth_test(state) == -1)
            {
                return -1;
            }
            break;
        }
    }
    
    return 0;
}

static int run_read_test(struct client_state *state)
{
    PRINT_STACK_TRACE(state->tracer);
    
    switch (state->test_number)
    {
        case READ_USER:
        {
            if (read_user_test(state) == -1)
            {
                return -1;
            }
            break;
        }
        case READ_CHANNEL:
        {
            if (read_channel_test(state) == -1)
            {
                return -1;
            }
            break;
        }
        case READ_MESSAGE:
        {
            if (read_message_test(state) == -1)
            {
                return -1;
            }
            break;
        }
    }
    
    return 0;
}

static int run_update_test(struct client_state *state)
{
    PRINT_STACK_TRACE(state->tracer);
    
    switch (state->test_number)
    {
        case UPDATE_USER:
        {
            if (update_user_display_name_test(state) == -1)
            {
                return -1;
            }
            break;
        }
        case UPDATE_CHANNEL:
        {
            if (update_channel_test(state) == -1)
            {
                return -1;
            }
            break;
        }
        case UPDATE_MESSAGE:
        {
            if (update_message_test(state) == -1)
            {
                return -1;
            }
            break;
        }
        case UPDATE_AUTH:
        {
            if (update_auth_test(state) == -1)
            {
                return -1;
            }
            break;
        }
    }
    
    return 0;
}

static int run_destroy_test(struct client_state *state)
{
    PRINT_STACK_TRACE(state->tracer);
    
    switch (state->test_number)
    {
        case DESTROY_USER:
        {
            if (destroy_user_test(state) == -1)
            {
                return -1;
            }
            break;
        }
        case DESTROY_CHANNEL:
        {
            if (destroy_channel_test(state) == -1)
            {
                return -1;
            }
            break;
        }
        case DESTROY_MESSAGE:
        {
            if (destroy_message_test(state) == -1)
            {
                return -1;
            }
            break;
        }
        case DESTROY_AUTH:
        {
            if (destroy_auth_test(state) == -1)
            {
                return -1;
            }
            break;
        }
    }
    
    return 0;
}
