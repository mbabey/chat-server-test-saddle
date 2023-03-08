#ifndef CHAT_TEST_SADDLE_SADDLE_FUNCTIONS_H
#define CHAT_TEST_SADDLE_SADDLE_FUNCTIONS_H

/**
 * Function cast type for library function.
 */
#define SADDLE_FUNCTION (int (*)(void *))

/**
 * Saddle library function name.
 */
#define SADDLE_FUNCTION_NAME "lib_main"

/**
 * lib_main
 * <p>
 * Start function to be defined by library.
 * </p>
 * @param args the args object
 * @return 0 on success, -1 on failure
 */
int lib_main(void *args);

#endif //CHAT_TEST_SADDLE_SADDLE_FUNCTIONS_H
