#ifndef CHAT_SERVER_TEST_SADDLE_UTIL_H
#define CHAT_SERVER_TEST_SADDLE_UTIL_H



/**
 * get_api
 * <p>
 * Open a given library and attempt to load functions into the test_functions struct.
 * </p>
 * @param test_functions struct containing test functions.
 * @param lib_name name of the library.
 * @param env pointer to a dc_env struct.
 * @return 0 on success, -1 and set errno on failure
 */
int open_lib(void *lib, struct test_functions *test_functions, const char *lib_name, const struct dc_env *env);

/**
 * close_lib
 * <p>
 * Close a dynamic library.
 * </p>
 * @param lib the library to close.
 * @param lib_name the name of the library to close
 * @return 0 on success. On failure, -1 and set errno.
 */
int close_lib(void *lib, const char *lib_name);


// Close library


#endif //CHAT_SERVER_TEST_SADDLE_UTIL_H
