#ifndef CHAT_TEST_SADDLE_SADDLE_H
#define CHAT_TEST_SADDLE_SADDLE_H

#include "global-objects.h"

/**
 * setup_saddle
 * <p>
 * Parse arguments and set up the saddle state object.
 * </p>
 * @param state the sate object
 * @param library the library object
 * @param argc the number of arguments
 * @param argv the arguments
 * @return 0 on success, -1 and set errno on failure
 */
int setup_saddle(struct state *state, struct library *library, int argc, char **argv);

/**
 * run_saddle
 * <p>
 * Run the test saddle
 * </p>
 * @param state the state object
 * @param library the library
 * @return 0 on success, -1 and set err on failure
 */
int run_saddle(struct state *state, struct library *library);

/**
 * exit_saddle
 * <p>
 * Close the library and clean up any memory.
 * </p>
 * @param state the state object
 * @param library the library object
 */
void exit_saddle(struct state *state, struct library *library);

#endif //CHAT_TEST_SADDLE_SADDLE_H
