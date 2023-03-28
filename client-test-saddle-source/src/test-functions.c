#include "../../include/util.h"
#include "../include/test-functions.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * test_dispatch
 * <p>
 * Print an assembled Dispatch Request. Send the Dispatch Request and wait for the Response. Print the Dispatch
 * Response.
 * </p>
 * @param state the state object
 * @param dispatch the dispatch
 * @return 0 on success, -1 and set err on failure
 */
static int test_dispatch(struct client_state *state, struct dispatch *dispatch);

int create_user_test(struct client_state *state)
{
    printf("create user test\n");
    
    struct dispatch dispatch;
    
//    const char      *login_token;
//    const char      *display_name;
//    const char      *password;
    
    dispatch.version   = (unsigned int) 1;
    dispatch.type      = (unsigned int) CREATE;
    dispatch.object    = (unsigned int) USER;
    dispatch.body      = strdup("bigdog69\x03thedog\x03password1234@!\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

static int test_dispatch(struct client_state *state, struct dispatch *dispatch)
{
    int  status;
    char **body_tokens;
    
    print_dispatch((struct state *) state, dispatch, "Request");
    
    status = assemble_message_send((struct state *) state, state->socket_fd, dispatch);
    free((*dispatch).body);
    if (status == -1)
    {
        return -1;
    }
    
    status = recv_parse_message((struct state *) state, state->socket_fd, dispatch, &body_tokens);
    if (status == -1)
    {
        return -1;
    }
    if (status == 0)
    {
        print_dispatch((struct state *) state, dispatch, "Response");
    }
    
    return 0;
}

int create_channel_test(struct client_state *state)
{
    printf("create channel test.\n");
    
    struct dispatch dispatch;
//    const char      *channel_name;
//    const char      *display_name;
//    const char      *publicity;
    
    dispatch.version   = (unsigned int) 1;
    dispatch.type      = (unsigned int) CREATE;
    dispatch.object    = (unsigned int) CHANNEL;
    dispatch.body      = strdup("the doghouse\x03thedog\x03""1""\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int create_message_test(struct client_state *state)
{
    printf("create message test.\n");
    
    struct dispatch dispatch;
//    const char      *display_name;
//    const char      *channel_name;
//    const char      *message_content;
//    time_t          timestamp;
    
    dispatch.version   = (unsigned int) 1;
    dispatch.type      = (unsigned int) CREATE;
    dispatch.object    = (unsigned int) MESSAGE;
    dispatch.body      = strdup("thedog\x03the doghouse\x03yo what's up its me the dog\x03""1678347396""\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int create_auth_test(struct client_state *state)
{
    printf("create_auth_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) CREATE;
    dispatch.object = (unsigned int) AUTH;
    dispatch.body = strdup("thedog\x03password1234@!\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int read_user_test(struct client_state *state)
{
    printf("read_user_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) READ;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("thedog\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int read_all_user_test(struct client_state *state)
{
    printf("read_user_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) READ;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int read_channel_test(struct client_state *state)
{
    printf("read_channel_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) READ;
    dispatch.object = (unsigned int) CHANNEL;
    dispatch.body = strdup("the doghouse\x03""0\x03""0\x03""0\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int read_channel_get_users_test(struct client_state *state)
{
    printf("read_channel_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) READ;
    dispatch.object = (unsigned int) CHANNEL;
    dispatch.body = strdup("the doghouse\x03""1\x03""0\x03""0\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int read_channel_get_admins_test(struct client_state *state)
{
    printf("read_channel_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) READ;
    dispatch.object = (unsigned int) CHANNEL;
    dispatch.body = strdup("the doghouse\x03""0\x03""1\x03""0\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int read_channel_get_banned_users_test(struct client_state *state)
{
    printf("read_channel_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) READ;
    dispatch.object = (unsigned int) CHANNEL;
    dispatch.body = strdup("the doghouse\x03""0\x03""0\x03""1\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int read_channel_get_all_test(struct client_state *state)
{
    printf("read_channel_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) READ;
    dispatch.object = (unsigned int) CHANNEL;
    dispatch.body = strdup("the doghouse\x03""1\x03""1\x03""1\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int read_message_test(struct client_state *state)
{
    printf("read_message_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) READ;
    dispatch.object = (unsigned int) MESSAGE;
    dispatch.body = strdup();
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_user_test(struct client_state *state)
{
    printf("update_user_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup();
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_channel_test(struct client_state *state)
{
    printf("update_channel_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) CHANNEL;
    dispatch.body = strdup();
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_message_test(struct client_state *state)
{
    printf("update_message_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) MESSAGE;
    dispatch.body = strdup();
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_auth_test(struct client_state *state)
{
    printf("update_auth_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) AUTH;
    dispatch.body = strdup();
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int destroy_user_test(struct client_state *state)
{
    printf("destroy_user_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) DESTROY;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup();
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int destroy_channel_test(struct client_state *state)
{
    printf("destroy_channel_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) DESTROY;
    dispatch.object = (unsigned int) CHANNEL;
    dispatch.body = strdup();
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int destroy_message_test(struct client_state *state)
{
    printf("destroy_message_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) DESTROY;
    dispatch.object = (unsigned int) MESSAGE;
    dispatch.body = strdup();
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int destroy_auth_test(struct client_state *state)
{
    printf("destroy_auth_test not yet implemented.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) DESTROY;
    dispatch.object = (unsigned int) AUTH;
    dispatch.body = strdup();
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}
