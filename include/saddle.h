#ifndef CHAT_TEST_SADDLE_SADDLE_H
#define CHAT_TEST_SADDLE_SADDLE_H

#include "global-objects.h"

/**
 * setup_saddle
 * <p>
 * Parse arguments and set up the saddle state object.
 * </p>
 * @param state the sate object
 * @param argc the number of arguments
 * @param argv the arguments
 * @return 0 on success, -1 and set errno on failure
 */
int setup_saddle(struct state *state, int argc, char **argv);

int run_saddle(struct state *state);

void exit_saddle(struct state *state);

#endif //CHAT_TEST_SADDLE_SADDLE_H
