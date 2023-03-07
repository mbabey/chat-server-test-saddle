#ifndef CHAT_SERVER_TEST_SADDLE_UTIL_H
#define CHAT_SERVER_TEST_SADDLE_UTIL_H

#include "../test_functions.h"


#include <dc_env/env.h>

/**
 * get_api
 * <p>
 * Open a given library and attempt to load functions into the test_functions struct.
 * </p>
 * @param env pointer to a dc_env struct.
 * @param lib pointer at which to open the library
 * @param lib_name name of the library.
 * @param test_functions struct containing test functions.
 * @return 0 on success, -1 and set errno on failure
 */
int open_lib(const struct dc_env *env, void **lib, const char *lib_name, struct test_functions *test_functions);

/**
 * close_lib
 * <p>
 * Close a dynamic library.
 * </p>
 * @param env pointer to a dc_env struct.
 * @param lib the library to close.
 * @param lib_name the name of the library to close
 * @return 0 on success. On failure, -1 and set errno.
 */
int close_lib(const struct dc_env *env, void *lib, const char *lib_name);

#endif //CHAT_SERVER_TEST_SADDLE_UTIL_H
