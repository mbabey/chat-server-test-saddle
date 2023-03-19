#ifndef CLIENT_TEST_SADDLE_UTIL_H
#define CLIENT_TEST_SADDLE_UTIL_H

#include "global-objects.h"


/**
 * recv_parse_message
 * <p>
 * Read a message into a dispatch struct.
 * </p>
 * @param state the state object
 * @param client the client object
 * @param dispatch the dispatch to receive into
 * @return 0 on success, -1 on set err failure
 */
int recv_parse_message(struct state *state, int socket_fd, struct dispatch *dispatch);

/**
 * assemble_message_send
 * <p>
 * Assemble and send message .
 * </p>
 * @param state the state object
 * @param client the client object
 * @param dispatch the dispatch to send
 * @return 0 on success, -1 on set err failure
 */
int assemble_message_send(struct state *state, int socket_fd, struct dispatch *dispatch);

/**
 * parse_body
 * <p>
 * Parse the body of a dispatch. Count the number ETX in the body and allocate
 * an array of strings in body_tokens of that count plus one. Then, tokenize
 * the body on ETX and store each token in the string array.
 * </p>
 * @param state the state object
 * @param client the client object
 * @param body_tokens the string array to store the body tokens
 * @param body_size the body size
 * @param body the body
 * @return 0 on success, -1 and set err on failure
 */
int parse_body(struct state *state, char ***body_tokens, uint16_t body_size, char *body);

#endif //CLIENT_TEST_SADDLE_UTIL_H
