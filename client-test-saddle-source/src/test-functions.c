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
    printf("\nCreating user \"thedog\"\n");
    
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
    printf("\nCreating public channel \"the doghouse\".\n");
    
    struct dispatch dispatch;
//    const char      *channel_name;
//    const char      *display_name;
//    const char      *publicity;
    
    dispatch.version   = (unsigned int) 1;
    dispatch.type      = (unsigned int) CREATE;
    dispatch.object    = (unsigned int) CHANNEL;
    dispatch.body      = strdup("the doghouse\x03thedog\x03""0""\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int create_message_test(struct client_state *state)
{
    printf("\nCreating message in channel \"the doghouse\".\n");
    
    struct dispatch dispatch;
//    const char      *display_name;
//    const char      *channel_name;
//    const char      *message_content;
//    time_t          timestamp;
    
    dispatch.version   = (unsigned int) 1;
    dispatch.type      = (unsigned int) CREATE;
    dispatch.object    = (unsigned int) MESSAGE;
    dispatch.body      = strdup("thedog\x03the doghouse\x03yo what's up its me the dog\x03""0000000064228f8a""\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int create_auth_test(struct client_state *state)
{
    printf("\nLogging in user with login token \nbigdog69\n.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) CREATE;
    dispatch.object = (unsigned int) AUTH;
    dispatch.body = strdup("bigdog69\x03password1234@!\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int read_user_test(struct client_state *state)
{
    printf("\nReading user \"thedog\".\n");
    
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

int read_user_all_test(struct client_state *state)
{
    printf("\nReading all online users.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) READ;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("");
    dispatch.body_size = 0;
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int read_channel_test(struct client_state *state)
{
    printf("\nReading channel \"the doghouse\".\n");
    
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

int read_channel_users_test(struct client_state *state)
{
    printf("\nReading users in channel \"the doghouse\".\n");
    
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

int read_channel_admins_test(struct client_state *state)
{
    printf("\nReading administrators in channel \"the doghouse\".\n");
    
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

int read_channel_banned_users_test(struct client_state *state)
{
    printf("\nReading banned users in channel \"the doghouse\".\n");
    
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

int read_channel_all_test(struct client_state *state)
{
    printf("\nReading all values in channel \"the doghouse\".\n");
    
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
    printf("\nReading 10 messages from the channel \"the doghouse\".\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) READ;
    dispatch.object = (unsigned int) MESSAGE;
    dispatch.body = strdup("the doghouse\x03""10\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_user_display_name_test(struct client_state *state)
{
    printf("\nUpdating User display name to \"thecat\".\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("thedog\x03""1\x03thecat\x03""0\x03""0\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_user_display_name_test_reset(struct client_state *state)
{
    printf("\nUpdating User display name to \"thedog\".\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("thecat\x03""1\x03thedog\x03""0\x03""0\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_user_privilege_level_test(struct client_state *state)
{
    printf("\nUpdating User privilege level to 1.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("thedog\x03""0\x03""1\x03""1\x03""0\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_user_privilege_level_test_reset(struct client_state *state)
{
    printf("\nUpdating User privilege level to 0.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("thedog\x03""0\x03""1\x03""0\x03""0\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_user_online_status_test(struct client_state *state)
{
    printf("\nUpdating User online status to 1.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("thedog\x03""0\x03""0\x03""1\x03""1\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_user_online_status_test_reset(struct client_state *state)
{
    printf("\nUpdating User online status to 1.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("thedog\x03""0\x03""0\x03""1\x03""1\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_user_all_test(struct client_state *state)
{
    printf("\nUpdating User name to \"thecat\", privilege level to 1, and online status to 0.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("thedog\x03""1\x03thecat\x03""1\x03""1\x03""1\x03""0\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_user_all_test_reset(struct client_state *state)
{
    printf("\nUpdating User name to \"thedog\", privilege level to 0, and online status to 1.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("thecat\x03""1\x03thedog\x03""1\x03""0\x03""1\x03""1\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_channel_test(struct client_state *state)
{
    printf("\nupdate_channel_test not yet implemented.\n");
    
//    struct dispatch dispatch;
//
//    dispatch.version = (unsigned int) 1;
//    dispatch.type = (unsigned int) UPDATE;
//    dispatch.object = (unsigned int) CHANNEL;
//    dispatch.body = strdup("");
//    dispatch.body_size = strlen(dispatch.body);
//
//    if (test_dispatch(state, &dispatch) == -1)
//    {
//        return -1;
//    }
    
    return 0;
}

int update_message_test(struct client_state *state)
{
    printf("\nUpdating message by user \"thedog\" in channel \"the doghouse\" with timestamp \"0000000064228f8a\".\n");

    struct dispatch dispatch;

    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) MESSAGE;
    dispatch.body = strdup("thedog\x03the doghouse\x03""0000000064228f8a\x03wtf haha this is a message\x03");
    dispatch.body_size = strlen(dispatch.body);

    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_auth_test(struct client_state *state)
{
    printf("\nChanging password to \"!@4321drowssap\".\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) AUTH;
    dispatch.body = strdup("thedog\x03password1234@!\x03!@4321drowssap\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int update_auth_no_password_test_reset(struct client_state *state)
{
    printf("\nChanging password to \"password1234@!\" without providing old password.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) UPDATE;
    dispatch.object = (unsigned int) AUTH;
    dispatch.body = strdup("thedog\x03password1234@!\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int destroy_user_test(struct client_state *state)
{
    printf("\nDestroying user \"thedog\".\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) DESTROY;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("thedog\x03bigdog69\x03password1234@!\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int destroy_user_no_password_test(struct client_state *state)
{
    printf("\nDestroying user \"thedog\" with no password.\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) DESTROY;
    dispatch.object = (unsigned int) USER;
    dispatch.body = strdup("thedog\x03bigdog69\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int destroy_channel_test(struct client_state *state)
{
    printf("\nDestroying channel \"the doghouse\".\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) DESTROY;
    dispatch.object = (unsigned int) CHANNEL;
    dispatch.body = strdup("the doghouse\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}

int destroy_message_test(struct client_state *state)
{
    printf("\ndestroy_message_test not yet implemented.\n");
    
//    struct dispatch dispatch;
//
//    dispatch.version = (unsigned int) 1;
//    dispatch.type = (unsigned int) DESTROY;
//    dispatch.object = (unsigned int) MESSAGE;
//    dispatch.body = strdup();
//    dispatch.body_size = strlen(dispatch.body);
//
//    if (test_dispatch(state, &dispatch) == -1)
//    {
//        return -1;
//    }
    
    return 0;
}

int destroy_auth_test(struct client_state *state)
{
    printf("\nRequesting logout of \"thedog\".\n");
    
    struct dispatch dispatch;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type = (unsigned int) DESTROY;
    dispatch.object = (unsigned int) AUTH;
    dispatch.body = strdup("thedog\x03");
    dispatch.body_size = strlen(dispatch.body);
    
    if (test_dispatch(state, &dispatch) == -1)
    {
        return -1;
    }
    
    return 0;
}
