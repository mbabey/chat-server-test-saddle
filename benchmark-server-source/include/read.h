#ifndef SERVER_TEST_SADDLE_READ_H
#define SERVER_TEST_SADDLE_READ_H

#include "../../include/global-objects.h"
#include "objects.h"

/**
 * handle_read
 * <p>
 * Switch on the Object of a READ Type Dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
int handle_read(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_read_user
 * <p>
 * Get User information and put it in the dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
int handle_read_user(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_read_channel
 * <p>
 * Get Channel information and put it in the dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
int handle_read_channel(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_read_message
 * <p>
 * Get Message information and put it in the dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
int handle_read_message(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

#endif //SERVER_TEST_SADDLE_READ_H
