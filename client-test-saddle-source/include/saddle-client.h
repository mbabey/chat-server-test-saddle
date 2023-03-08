#ifndef CLIENT_TEST_SADDLE_SADDLE_CLIENT_H
#define CLIENT_TEST_SADDLE_SADDLE_CLIENT_H

#include "state.h"

/**
 * run_client_saddle
 * <p>
 * Run the Client test saddle. Wait for user input and run a test based on that input. Print the results of each
 * test run.
 * </p>
 * @param state the state object
 * @param client the client object
 * @return 0 on success, -1 on and set err failure
 */
int run_client_saddle(struct state_minor *state, struct client *client);

#endif //CLIENT_TEST_SADDLE_SADDLE_CLIENT_H
