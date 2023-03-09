#ifndef CLIENT_TEST_SADDLE_UTIL_H
#define CLIENT_TEST_SADDLE_UTIL_H

#include "state.h"

/**
 * recv_parse_message
 * <p>
 * Read a message into a dispatch struct.
 * </p>
 * @param state the state object
 * @param client the client object
 * @return 0 on success, -1 on set err failure
 */
int recv_parse_message(struct state_minor *state, struct client *client, struct dispatch *dispatch);

/**
 * assemble_message_send
 * <p>
 * Assemble and send message .
 * </p>
 * @param state the state object
 * @param client the client object
 * @return 0 on success, -1 on set err failure
 */
int assemble_message_send(struct state_minor *state, struct client *client, struct dispatch *dispatch);

#endif //CLIENT_TEST_SADDLE_UTIL_H
