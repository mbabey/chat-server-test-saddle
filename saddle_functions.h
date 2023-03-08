#ifndef CHAT_TEST_SADDLE_SADDLE_FUNCTIONS_H
#define CHAT_TEST_SADDLE_SADDLE_FUNCTIONS_H

#include "./include/state.h"

/**
 * Function cast type for library function.
 */
#define LIB_FUNCTION (int (*)(struct state *state))

/**
 * Struct to store function pointers from library.
 */
struct saddle_lib
{
    int (*lib_start)(struct state *state);
    
    int (*lib_run)(struct state *state);
    
    int (*lib_end)(struct state *state);
};

/**
 * lib_start
 * <p>
 * Start function to be defined by library.
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int lib_start(struct state *state);

/**
 * lib_run
 * <p>
 * Run function to be defined by library.
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int lib_run(struct state *state);

/**
 * lib_end
 * <p>
 * End function to be defined by library.
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int lib_end(struct state *state);

#endif //CHAT_TEST_SADDLE_SADDLE_FUNCTIONS_H
