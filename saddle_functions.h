#ifndef CHAT_TEST_SADDLE_SADDLE_FUNCTIONS_H
#define CHAT_TEST_SADDLE_SADDLE_FUNCTIONS_H

/**
 * Function cast type for library function.
 */
#define SADDLE_FUNCTION (int (*)(void *))

/**
 * Saddle library function names.
 */
#define SADDLE_LIB_START_NAME "lib_start"
#define SADDLE_LIB_RUN_NAME "lib_run"
#define SADDLE_LIB_END_NAME "lib_end"

/**
 * Struct to store function pointers from library.
 */
struct saddle_lib
{
    int (*lib_start)(void *);
    
    int (*lib_run)(void *);
    
    int (*lib_end)(void *);
};

/**
 * lib_start
 * <p>
 * Start function to be defined by library.
 * </p>
 * @param args the args object
 * @return 0 on success, -1 on failure
 */
int lib_start(void *args);

/**
 * lib_run
 * <p>
 * Run function to be defined by library.
 * </p>
 * @param args the args object
 * @return 0 on success, -1 on failure
 */
int lib_run(void *args);

/**
 * lib_end
 * <p>
 * End function to be defined by library.
 * </p>
 * @param args the args object
 * @return 0 on success, -1 on failure
 */
int lib_end(void *args);


#endif //CHAT_TEST_SADDLE_SADDLE_FUNCTIONS_H
