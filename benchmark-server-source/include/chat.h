#ifndef SERVER_TEST_SADDLE_CHAT_H
#define SERVER_TEST_SADDLE_CHAT_H

#include "../../include/global-objects.h"
#include "objects.h"

/**
 * perform_dispatch_operation
 * <p>
 * Perform the dispatch operation indicated by the dispatch passed to this function.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
int perform_dispatch_operation(struct core_object *co, struct server_object *so, struct dispatch *dispatch,
                               char **body_tokens);


/**
 * broadcast_message_to_channel
 * <p>
 * Broadcast a message to all users in a channel.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param dispatch the dispatch
 * @param body_tokens the body tokens
 * @return 0 on success, -1 and set err on failure.
 */
int broadcast_message_to_channel(struct core_object *co, struct server_object *so, struct dispatch *dispatch,
                                 char **body_tokens);

#endif //SERVER_TEST_SADDLE_CHAT_H
