#ifndef CLIENT_TEST_SADDLE_UI_H
#define CLIENT_TEST_SADDLE_UI_H

#include "client-objects.h"

/**
 * run_ui
 * <p>
 * Display the user interface menu on stdout. Get user input.
 * </p>
 * @param state the state object
 * @param client tne client object
 * @return 0 on success, -1 and set err on failure.
 */
int run_ui(struct state_minor *state, struct client *client);

/**
 * display_results
 * <p>
 * Display the results of a test. Wait for a key press before exiting the function.
 * </p>
 * @param state the state object
 * @return 0 on success, -1 and set err on failure.
 */
int display_results(struct state_minor *state);

#endif //CLIENT_TEST_SADDLE_UI_H
