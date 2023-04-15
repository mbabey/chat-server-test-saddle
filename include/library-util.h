#ifndef CHAT_SERVER_TEST_SADDLE_UTIL_H
#define CHAT_SERVER_TEST_SADDLE_UTIL_H

#include "global-objects.h"

#include <dc_env/env.h>

#if defined(__unix__) || defined(__unix) || defined(unix)
#define SERVER_SADDLE "../benchmark-server-source/build/libtest-server.so"
#define CLIENT_SADDLE "../test-client-source/build/libtest-client.so"
#elif defined(__APPLE__) && defined(__MACH__)
#define SERVER_SADDLE "../benchmark-server-source/build/libtest-server.dylib"
#define CLIENT_SADDLE "../test-client-source/build/libtest-client.dylib"
#endif

/**
 * get_api
 * <p>
 * Open a given library and attempt to load functions into the saddle_lib struct.
 * </p>
 * @param state state object
 * @return 0 on success, -1 and set err on failure
 */
int open_lib(struct state *state, struct library *library);

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
