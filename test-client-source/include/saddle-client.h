#ifndef CLIENT_TEST_SADDLE_SADDLE_CLIENT_H
#define CLIENT_TEST_SADDLE_SADDLE_CLIENT_H

#include "client-objects.h"

enum TestNumber
{
    INVALID,
    
    CREATE_USER,
    CREATE_CHANNEL,
    CREATE_MESSAGE,
    CREATE_AUTH,
    
    READ_USER,
    READ_USER_ALL,
    READ_CHANNEL,
    READ_CHANNEL_USERS,
    READ_CHANNEL_ADMINS,
    READ_CHANNEL_BANNED_USERS,
    READ_CHANNEL_ALL,
    READ_MESSAGE,
    
    UPDATE_USER_DISPLAY_NAME,
    UPDATE_USER_DISPLAY_NAME_RESET,
    UPDATE_USER_PRIVILEGE_LEVEL,
    UPDATE_USER_PRIVILEGE_LEVEL_RESET,
    UPDATE_USER_ONLINE_STATUS,
    UPDATE_USER_ONLINE_STATUS_RESET,
    UPDATE_USER_ALL,
    UPDATE_USER_ALL_RESET,
    UPDATE_CHANNEL_NAME,
    UPDATE_CHANNEL_NAME_RESET,
    UPDATE_MESSAGE,
    UPDATE_AUTH,
    UPDATE_AUTH_NO_PASSWORD,
    
    DESTROY_USER,
    DESTROY_USER_NO_PASSWORD,
    DESTROY_CHANNEL,
    DESTROY_MESSAGE,
    DESTROY_AUTH,
    
    STOP
};

/**
 * run_client_saddle
 * <p>
 * Run the Client test saddle. Wait for user input and run a test based on that input. Print the results of each
 * test run.
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on and set err failure
 */
int run_client_saddle(struct client_state *state);

#endif //CLIENT_TEST_SADDLE_SADDLE_CLIENT_H
