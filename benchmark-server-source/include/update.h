#ifndef SERVER_TEST_SADDLE_UPDATE_H
#define SERVER_TEST_SADDLE_UPDATE_H

#include "../../include/global-objects.h"
#include "objects.h"

/**
 * handle_update
 * <p>
 * Switch on the Object of an UPDATE Type Dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
int handle_update(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

int handle_update_user(struct core_object *co, struct dispatch *dispatch, char **body_tokens);
int handle_update_channel(struct core_object *co, struct dispatch *dispatch, char **body_tokens);
int handle_update_message(struct core_object *co, struct dispatch *dispatch, char **body_tokens);
int handle_update_auth(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

#endif //SERVER_TEST_SADDLE_UPDATE_H
