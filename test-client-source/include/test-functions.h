#ifndef CHAT_TEST_SADDLE_TEST_FUNCTIONS_H
#define CHAT_TEST_SADDLE_TEST_FUNCTIONS_H

#include "client-objects.h"

/**
 * create_user_test
 * <p>
 * Run a create user test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int create_user_test(struct client_state *state);

/**
 * create_channel_test
 * <p>
 * Run a create channel test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int create_channel_test(struct client_state *state);

/**
 * create_message_test
 * <p>
 * Run a create message test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int create_message_test(struct client_state *state);

/**
 * create_auth_test
 * <p>
 * Run a create auth test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int create_auth_test(struct client_state *state);

/**
 * read_user_test
 * <p>
 * Run a read user test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int read_user_test(struct client_state *state);

/**
 * read_user_all_test
 * <p>
 * Run a read all users test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int read_user_all_test(struct client_state *state);

/**
 * read_channel_test
 * <p>
 * Run a read channel test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int read_channel_test(struct client_state *state);

/**
 * read_channel_users_test
 * <p>
 * Run a read channel users test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int read_channel_users_test(struct client_state *state);

/**
 * read_channel_admins_test
 * <p>
 * Run a read channel admins test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int read_channel_admins_test(struct client_state *state);

/**
 * read_banned_users_test
 * <p>
 * Run a read banned users test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int read_channel_banned_users_test(struct client_state *state);

/**
 * read_channel_all_test
 * <p>
 * Run a read all channel items test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int read_channel_all_test(struct client_state *state);

/**
 * read_message_test
 * <p>
 * Run a read message test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int read_message_test(struct client_state *state);

/**
 * update_user_display_name_test
 * <p>
 * Run an update user display name test test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_user_display_name_test(struct client_state *state);

/**
 * update_user_display_name_test_reset
 * <p>
 * Run an update user display name test reset test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_user_display_name_test_reset(struct client_state *state);

/**
 * update_user_privilege_level_test
 * <p>
 * Run an update user privilege level test test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_user_privilege_level_test(struct client_state *state);

/**
 * update_user_privilege_level_test_reset
 * <p>
 * Run an update user privilege level test reset test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_user_privilege_level_test_reset(struct client_state *state);

/**
 * update_user_online_status_test
 * <p>
 * Run an update user online status test test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_user_online_status_test(struct client_state *state);

/**
 * update_user_online_status_test_reset
 * <p>
 * Run an update user online status test reset test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_user_online_status_test_reset(struct client_state *state);

/**
 * update_user_all_test
 * <p>
 * Run an update user all test test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_user_all_test(struct client_state *state);

/**
 * update_user_all_test_reset
 * <p>
 * Run an update user all test reset test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_user_all_test_reset(struct client_state *state);

/**
 * update_channel_name_test
 * <p>
 * Run an update channel name test test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_channel_name_test(struct client_state *state);

/**
 * update_channel_name_test_reset
 * <p>
 * Run an update channel name test reset test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_channel_name_test_reset(struct client_state *state);

/**
 * update_message_test
 * <p>
 * Run an update message test test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_message_test(struct client_state *state);

/**
 * update_auth_test
 * <p>
 * Run an update auth test test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_auth_test(struct client_state *state);

/**
 * update_auth_no_password_test_reset
 * <p>
 * Run an update auth no password test reset test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int update_auth_no_password_test_reset(struct client_state *state);

/**
 * destroy_user_test
 * <p>
 * Run a destroy user test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int destroy_user_test(struct client_state *state);

/**
 * destroy_user_no_password_test
 * <p>
 * Run a destroy user no password test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int destroy_user_no_password_test(struct client_state *state);

/**
 * destroy_channel_test
 * <p>
 * Run a destroy channel test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int destroy_channel_test(struct client_state *state);

/**
 * destroy_message_test
 * <p>
 * Run a destroy message test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int destroy_message_test(struct client_state *state);

/**
 * destroy_auth_test
 * <p>
 * Run a destroy auth test
 * </p>
 * @param state the state object
 * @return 0 on success, -1 on failure
 */
int destroy_auth_test(struct client_state *state);

#endif //CHAT_TEST_SADDLE_TEST_FUNCTIONS_H
