#ifndef CHAT_TEST_SADDLE_TEST_FUNCTIONS_H
#define CHAT_TEST_SADDLE_TEST_FUNCTIONS_H

#include "client-objects.h"

int create_user_test(struct client_state *state);

int create_channel_test(struct client_state *state);

int create_message_test(struct client_state *state);

int create_auth_test(struct client_state *state);

int read_user_test(struct client_state *state);

int read_user_all_test(struct client_state *state);

int read_channel_test(struct client_state *state);

int read_channel_users_test(struct client_state *state);

int read_channel_admins_test(struct client_state *state);

int read_channel_banned_users_test(struct client_state *state);

int read_channel_all_test(struct client_state *state);

int read_message_test(struct client_state *state);

int update_user_display_name_test(struct client_state *state);

int update_user_display_name_test_reset(struct client_state *state);

int update_user_privilege_level_test(struct client_state *state);

int update_user_privilege_level_test_reset(struct client_state *state);

int update_user_online_status_test(struct client_state *state);

int update_user_online_status_test_reset(struct client_state *state);

int update_user_all_test(struct client_state *state);

int update_user_all_test_reset(struct client_state *state);

int update_channel_name_test(struct client_state *state);

int update_channel_name_test_reset(struct client_state *state);

int update_message_test(struct client_state *state);

int update_auth_test(struct client_state *state);

int update_auth_no_password_test_reset(struct client_state *state);

int destroy_user_test(struct client_state *state);

int destroy_user_no_password_test(struct client_state *state);

int destroy_channel_test(struct client_state *state);

int destroy_message_test(struct client_state *state);

int destroy_auth_test(struct client_state *state);

#endif //CHAT_TEST_SADDLE_TEST_FUNCTIONS_H
