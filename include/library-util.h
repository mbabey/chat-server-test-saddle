#ifndef CHAT_SERVER_TEST_SADDLE_UTIL_H
#define CHAT_SERVER_TEST_SADDLE_UTIL_H

#include "state.h"

#include <dc_env/env.h>

#define SERVER_SADDLE "../server-test-saddle-source/build/libserver-test-saddle.dylib"
#define CLIENT_SADDLE "../client-test-saddle-source/build/libclient-test-saddle.dylib"

/**
 * get_api
 * <p>
 * Open a given library and attempt to load functions into the saddle_lib struct.
 * </p>
 * @param lib pointer at which to open the library
 * @param lib_name name of the library.
 * @param saddle_lib struct containing test functions.
 * @param tracer tracer function
 * @return 0 on success, -1 and set errno on failure
 */
int open_lib(void **lib, const char *lib_name, struct state *state, TRACER_FUNCTION_AS(tracer));

/**
 * close_lib
 * <p>
 * Close a dynamic library.
 * </p>
 * @param lib the library to close.
 * @param lib_name the name of the library to close
 * @param tracer tracer function
 * @return 0 on success. On failure, -1 and set errno.
 */
int close_lib(void *lib, const char *lib_name, TRACER_FUNCTION_AS(tracer));

#endif //CHAT_SERVER_TEST_SADDLE_UTIL_H
