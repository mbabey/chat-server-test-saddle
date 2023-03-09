#include "../include/test-functions.h"
#include "../include/send-recv.h"

#include <stdio.h>
#include <stdlib.h>

int create_user_test(struct state_minor *state, struct client *client)
{
    printf("create_user_test\n");
    
    struct dispatch dispatch;
//    const char      *login_token;
//    const char      *display_name;
//    const char      *password;
    char            *dispatch_body_temp;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type    = (unsigned int) CREATE;
    dispatch.object  = (unsigned int) USER;
    
    // TODO: Make var args function to assemble body out of strings.
//    login_token = "bigdog69\n";
//    display_name = "thedog\n";
//    password = "password1234@!\n";
    dispatch_body_temp = strdup("bigdog69\x03thedog\x03password1234@!\x03");
    
    dispatch.body      = dispatch_body_temp;
    dispatch.body_size = strlen(dispatch.body);
    
    if (assemble_message_send(state, client, &dispatch) == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    free(dispatch_body_temp);
    
    if (recv_parse_message(state, client, &dispatch) == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    if (dispatch.version != (unsigned int) 1)
    {
        printf("Test failed: bad version number\n"); // TODO: make this attach to a struct for printing later.
    }
    if (dispatch.type != (unsigned int) CREATE)
    {
        printf("Test failed: bad type\n");
    }
    if (dispatch.object != (unsigned int) USER)
    {
        printf("Test failed: bad object\n");
    }
    
    // TODO: body parsing function.
    
    return 0;
}

int create_channel_test(struct state_minor *state, struct client *client)
{
    printf("create_channel_test not yet implemented.\n");
    
    struct dispatch dispatch;
//    const char      *display_name;
//    const char      *channel_name;
//    const char      *publicity;
    char            *dispatch_body_temp;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type    = (unsigned int) CREATE;
    dispatch.object  = (unsigned int) CHANNEL;
    
    dispatch_body_temp = strdup("the doghouse\x03thedog\x031\x03");
    
    dispatch.body      = dispatch_body_temp;
    dispatch.body_size = strlen(dispatch.body);
    
    if (assemble_message_send(state, client, &dispatch) == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    free(dispatch_body_temp);
    
    if (recv_parse_message(state, client, &dispatch) == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    if (dispatch.version != (unsigned int) 1)
    {
        printf("Test failed: bad version number\n"); // TODO: make this attach to a struct for printing later.
    }
    if (dispatch.type != (unsigned int) CREATE)
    {
        printf("Test failed: bad type\n");
    }
    if (dispatch.object != (unsigned int) USER)
    {
        printf("Test failed: bad object\n");
    }
    
    // TODO: body parsing function.
    
    return 0;
}

int create_message_test(struct state_minor *state, struct client *client)
{
    struct dispatch dispatch;
//    const char      *display_name;
//    const char      *channel_name;
//    const char      *message_content;
//    time_t          timestamp;
    char *dispatch_body_temp;
    
    dispatch.version = (unsigned int) 1;
    dispatch.type    = (unsigned int) CREATE;
    dispatch.object  = (unsigned int) MESSAGE;
    
    dispatch_body_temp = strdup("thedog\x03the doghouse\x03yo what's up its me the dog\x031678347396\x03");
    
    if (assemble_message_send(state, client, &dispatch) == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    free(dispatch_body_temp);
    
    if (recv_parse_message(state, client, &dispatch) == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    if (dispatch.version != (unsigned int) 1)
    {
        printf("Test failed: bad version number\n"); // TODO: make this attach to a struct for printing later.
    }
    if (dispatch.type != (unsigned int) CREATE)
    {
        printf("Test failed: bad type\n");
    }
    if (dispatch.object != (unsigned int) USER)
    {
        printf("Test failed: bad object\n");
    }
    
    // TODO: body parsing function.
    
    return 0;
}

int create_auth_test(void)
{
    printf("create_auth_test not yet implemented.\n");
    
    return 0;
}

int read_user_test(void)
{
    printf("read_user_test not yet implemented.\n");
    
    return 0;
}

int read_channel_test(void)
{
    printf("read_channel_test not yet implemented.\n");
    
    return 0;
}

int read_message_test(void)
{
    printf("read_message_test not yet implemented.\n");
    
    return 0;
}

int update_user_test(void)
{
    printf("update_user_test not yet implemented.\n");
    
    return 0;
}

int update_channel_test(void)
{
    printf("update_channel_test not yet implemented.\n");
    
    return 0;
}

int update_message_test(void)
{
    printf("update_message_test not yet implemented.\n");
    
    return 0;
}

int update_auth_test(void)
{
    printf("update_auth_test not yet implemented.\n");
    
    return 0;
}

int destroy_user_test(void)
{
    printf("destroy_user_test not yet implemented.\n");
    
    return 0;
}

int destroy_channel_test(void)
{
    printf("destroy_channel_test not yet implemented.\n");
    
    return 0;
}

int destroy_message_test(void)
{
    printf("destroy_message_test not yet implemented.\n");
    
    return 0;
}

int destroy_auth_test(void)
{
    printf("destroy_auth_test not yet implemented.\n");
    
    return 0;
}
