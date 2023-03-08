#ifndef CHAT_TEST_SADDLE_TEST_FUNCTIONS_H
#define CHAT_TEST_SADDLE_TEST_FUNCTIONS_H

/**
 * Function signature type for test functions.
 */
#define TEST_FUNCTION (int (*)(void))

#define TEST_FUNCTION_CREATE_USER "create_user_test"
#define TEST_FUNCTION_CREATE_CHANNEL "create_channel_test"
#define TEST_FUNCTION_CREATE_MESSAGE "create_message_test"
#define TEST_FUNCTION_CREATE_AUTH "create_auth_test"
#define TEST_FUNCTION_READ_USER "read_user_test"
#define TEST_FUNCTION_READ_CHANNEL "read_channel_test"
#define TEST_FUNCTION_READ_MESSAGE "read_message_test"
#define TEST_FUNCTION_UPDATE_USER "update_user_test"
#define TEST_FUNCTION_UPDATE_CHANNEL "update_channel_test"
#define TEST_FUNCTION_UPDATE_MESSAGE "update_message_test"
#define TEST_FUNCTION_UPDATE_AUTH "update_auth_test"
#define TEST_FUNCTION_DESTROY_USER "destroy_user_test"
#define TEST_FUNCTION_DESTROY_CHANNEL "destroy_channel_test"
#define TEST_FUNCTION_DESTROY_MESSAGE "destroy_message_test"
#define TEST_FUNCTION_DESTROY_AUTH "destroy_auth_test"

/**
 * Contains function pointers for all tests.
 */
struct test_functions
{
    int (*create_user_test)(void);
    
    int (*create_channel_test)(void);
    
    int (*create_message_test)(void);
    
    int (*create_auth_test)(void);
    
    int (*read_user_test)(void);
    
    int (*read_channel_test)(void);
    
    int (*read_message_test)(void);
    
    // No read auth test.
    int (*update_user_test)(void);
    
    int (*update_channel_test)(void);
    
    int (*update_message_test)(void);
    
    int (*update_auth_test)(void);
    
    int (*destroy_user_test)(void);
    
    int (*destroy_channel_test)(void);
    
    int (*destroy_message_test)(void);
    
    int (*destroy_auth_test)(void);
};

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int create_user_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int create_channel_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int create_message_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int create_auth_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int read_user_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int read_channel_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int read_message_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int update_user_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int update_channel_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int update_message_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int update_auth_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int destroy_user_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int destroy_channel_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int destroy_message_test(void);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int destroy_auth_test(void);

#endif //CHAT_TEST_SADDLE_TEST_FUNCTIONS_H
