#ifndef CLIENT_TEST_SADDLE_UTIL_H
#define CLIENT_TEST_SADDLE_UTIL_H

#include "global-objects.h"

/**
 * recv_parse_message
 * <p>
 * Read a message into a dispatch struct.
 * </p>
 * @param state the state object
 * @param socket_fd the socket on which to receive a message
 * @param dispatch the dispatch to receive into
 * @param body_tokens pointer to array in which to store strings of body
 * @return 0 on success, -1 on set err failure
 */
int recv_parse_message(struct state *state, int socket_fd, struct dispatch *dispatch, char ***body_tokens);

/**
 * count_tokens
 * <p>
 * Count the number of ETX characters in the body.
 * </p>
 * @param body_size the number of bytes in the body
 * @param body the body
 * @param tracer tracer function
 * @return the number of ETX tokens
 */
int count_tokens(uint16_t body_size, const char *body, void (*tracer)(const char *, const char *, size_t));

/**
 * assemble_message_send
 * <p>
 * Assemble and send message .
 * </p>
 * @param state the state object
 * @param socket the socket on which to send a message
 * @param dispatch the dispatch to send
 * @return 0 on success, -1 on set err failure
 */
int assemble_message_send(struct state *state, int socket_fd, struct dispatch *dispatch);

/**
 * free_body_tokens
 * <p>
 * Free body tokens in a null-terminated list of body tokens.
 * </p>
 * @param body_tokens the list of body tokens
 * @param tracer tracer function
 */
void free_body_tokens(struct state *state, char **body_tokens);

/**
 * print_dispatch
 * <p>
 * Print a dispatch's contents to stdout.
 * </p>
 * @param state the state object
 * @param dispatch the dispatch to print
 */
void print_dispatch(struct state *state, struct dispatch *dispatch, const char *req_res_str);

#endif //CLIENT_TEST_SADDLE_UTIL_H
