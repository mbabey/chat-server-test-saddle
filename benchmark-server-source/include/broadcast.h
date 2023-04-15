#ifndef TEST_SERVER_BROADCAST_H
#define TEST_SERVER_BROADCAST_H

struct forward_message_args
{
    struct core_object   *co;
    struct server_object *so;
    struct dispatch      *dispatch;
    const char           *display_name;
};

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

#endif //TEST_SERVER_BROADCAST_H
