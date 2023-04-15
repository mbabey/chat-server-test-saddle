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
 * @param so the server object
 * @param dispatch the dispatch
 * @param body_tokens the tokenized dispatch body
 * @return 0 on success, -1 and set err on failure
 */
int handle_destroy(struct core_object *co, struct server_object *so, struct dispatch *dispatch, char **body_tokens);

/**
 * handle_destroy_auth
 * <p>
 * Log a user out.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param dispatch the dispatch
 * @param body_tokens the body of the dispatch
 * @return 0 on success, -1 and set err on failure.
 */
int handle_destroy_auth(struct core_object *co, struct server_object *so, struct dispatch *dispatch,
        char **body_tokens);

#endif //SERVER_TEST_SADDLE_DESTROY_H
