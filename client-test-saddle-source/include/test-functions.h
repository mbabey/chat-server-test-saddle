#ifndef CHAT_TEST_SADDLE_TEST_FUNCTIONS_H
#define CHAT_TEST_SADDLE_TEST_FUNCTIONS_H

#include "client-objects.h"

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int create_user_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int create_channel_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int create_message_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int create_auth_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int read_user_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int read_channel_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int read_message_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int update_user_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int update_channel_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int update_message_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int update_auth_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int destroy_user_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int destroy_channel_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int destroy_message_test(struct client_state *state);

/**
 *
 * <p>
 *
 * </p>
 *
 * @return 0 on success, -1 and set errno on failure
 */
int destroy_auth_test(struct client_state *state);

#endif //CHAT_TEST_SADDLE_TEST_FUNCTIONS_H
