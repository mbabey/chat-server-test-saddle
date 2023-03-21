#ifndef SERVER_TEST_SADDLE_CREATE_H
#define SERVER_TEST_SADDLE_CREATE_H

#include "../../include/global-objects.h"
#include "objects.h"

/**
 * handle_create
 * <p>
 * Switch on the Object of a CREATE Type Dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
int handle_create(struct core_object *co, struct server_object *so, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_create_user
 * <p>
 * Create a User account.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure.
 */
int handle_create_user(struct core_object *co, struct server_object *so, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_create_channel
 * <p>
 * Create a Channel.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure.
 */
int handle_create_channel(struct core_object *co, struct server_object *so, struct dispatch *dispatch,
        char **body_tokens);

/**
 * handle_create_message
 * <p>
 * Create a Message in a Channel.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure.
 */
int handle_create_message(struct core_object *co, struct server_object *so, struct dispatch *dispatch,
        char **body_tokens);

/**
 * handle_create_auth
 * <p>
 * Log a User in.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure.
 */
int handle_create_auth(struct core_object *co, struct server_object *so, struct dispatch *dispatch, char **body_tokens);

#endif //SERVER_TEST_SADDLE_CREATE_H
