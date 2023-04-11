#ifndef SERVER_TEST_SADDLE_DESTROY_H
#define SERVER_TEST_SADDLE_DESTROY_H

#include "../../include/global-objects.h"
#include "objects.h"

/**
 * handle_destroy
 * <p>
 * Switch on the Object of a DESTROY Type Dispatch.
 * </p>
 * @param co the core object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
int handle_destroy(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

int handle_destroy_user(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

int handle_destroy_channel(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

int handle_destroy_message(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

int handle_destroy_auth(struct core_object *co, struct dispatch *dispatch, char **body_tokens);

#endif //SERVER_TEST_SADDLE_DESTROY_H
