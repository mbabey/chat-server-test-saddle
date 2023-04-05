#include "../../include/error-handlers.h"
#include "../../include/manager.h"
#include "../include/object-util.h"

#include <string.h>

unsigned long serialize_user(struct core_object *co, uint8_t **serial_user, const User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    unsigned long serial_user_size;
    
    serial_user_size = sizeof(user->id)
                       + strlen(user->display_name) + 1
                       + sizeof(user->privilege_level)
                       + sizeof(user->online_status);
    
    *serial_user = mm_malloc(serial_user_size, co->mm);
    if (!*serial_user)
    {
        SET_ERROR(co->err);
        return 0;
    }
    
    size_t byte_offset;
    
    memcpy(*serial_user, &user->id, sizeof(user->id));
    byte_offset = sizeof(user->id);
    memcpy((*serial_user + byte_offset), user->display_name, strlen(user->display_name) + 1);
    byte_offset += strlen(user->display_name) + 1;
    memcpy((*serial_user + byte_offset), &user->privilege_level, sizeof(user->privilege_level));
    byte_offset += sizeof(user->privilege_level);
    memcpy((*serial_user + byte_offset), &user->online_status, sizeof(user->online_status));
    
    return serial_user_size;
}

unsigned long serialize_channel(struct core_object *co, uint8_t **serial_channel, const Channel *channel)
{
    PRINT_STACK_TRACE(co->tracer);
    
    unsigned long serial_channel_size;
    
    serial_channel_size = sizeof(channel->id)
                          + strlen(channel->channel_name) + 1
                          + strlen(channel->creator) + 1
                          + channel->users_size_bytes
                          + channel->administrators_size_bytes
                          + channel->banned_users_size_bytes;
    
    *serial_channel = mm_malloc(serial_channel_size, co->mm);
    if (!*serial_channel)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    size_t     byte_offset;
    const char **list;
    
    memcpy(*serial_channel, &channel->id, sizeof(channel->id));
    byte_offset = sizeof(channel->id);
    memcpy((*serial_channel + byte_offset), channel->channel_name, strlen(channel->channel_name) + 1);
    byte_offset += strlen(channel->channel_name) + 1;
    memcpy((*serial_channel + byte_offset), channel->creator, strlen(channel->creator) + 1);
    byte_offset += strlen(channel->creator) + 1;
    
    list = channel->users;
    for (; *list != NULL; ++list)
    {
        memcpy((*serial_channel + byte_offset), *list, strlen(*list) + 1);
        byte_offset += strlen(*list) + 1;
    }
    list = channel->administrators;
    for (; *list != NULL; ++list)
    {
        memcpy((*serial_channel + byte_offset), *list, strlen(*list) + 1);
        byte_offset += strlen(*list) + 1;
    }
    list = channel->banned_users;
    for (; *list != NULL; ++list)
    {
        memcpy((*serial_channel + byte_offset), *list, strlen(*list) + 1);
        byte_offset += strlen(*list) + 1;
    }
    
    return serial_channel_size;
}

unsigned long serialize_message(struct core_object *co, uint8_t **serial_message, const Message *message)
{
    PRINT_STACK_TRACE(co->tracer);
    
    unsigned long serial_message_size;
    
    serial_message_size = sizeof(message->id)
                          + sizeof(message->user_id)
                          + sizeof(message->channel_id)
                          + strlen(message->message_content) + 1
                          + sizeof(message->timestamp);
    
    *serial_message = mm_malloc(serial_message_size, co->mm);
    if (!*serial_message)
    {
        SET_ERROR(co->err);
        return 0;
    }
    
    size_t byte_offset;
    
    memcpy(*serial_message, &message->id, sizeof(message->id));
    byte_offset = sizeof(message->id);
    memcpy((*serial_message + byte_offset), &message->user_id, sizeof(message->user_id));
    byte_offset += sizeof(message->user_id);
    memcpy((*serial_message + byte_offset), &message->channel_id, sizeof(message->channel_id));
    byte_offset += sizeof(message->channel_id);
    memcpy((*serial_message + byte_offset), message->message_content, strlen(message->message_content) + 1);
    byte_offset += strlen(message->message_content) + 1;
    memcpy((*serial_message + byte_offset), &message->timestamp, sizeof(message->timestamp));
    
    return serial_message_size;
}

unsigned long serialize_auth(struct core_object *co, uint8_t **serial_auth, const Auth *auth)
{
    PRINT_STACK_TRACE(co->tracer);
    
    unsigned long serial_auth_size;
    
    serial_auth_size = sizeof(auth->user_id)
                       + strlen(auth->login_token) + 1
                       + strlen(auth->password) + 1;
    
    *serial_auth = mm_malloc(sizeof(auth->user_id)
                             * strlen(auth->login_token) + 1
                             + strlen(auth->password) + 1, co->mm);
    if (!*serial_auth)
    {
        SET_ERROR(co->err);
        return 0;
    }
    
    size_t byte_offset;
    
    memcpy(*serial_auth, &auth->user_id, sizeof(auth->user_id));
    byte_offset = sizeof(auth->user_id);
    memcpy((*serial_auth + byte_offset), auth->login_token, strlen(auth->login_token) + 1);
    byte_offset += strlen(auth->login_token) + 1;
    memcpy((*serial_auth + byte_offset), auth->password, strlen(auth->password) + 1); // TODO: hash this lol
    
    return serial_auth_size;
}

unsigned long serialize_addr_id_pair(struct core_object *co, uint8_t **name_addr_dst,
                                     struct sockaddr_in *addr, int *user_id)
{
    PRINT_STACK_TRACE(co->tracer);
    
    size_t addr_id_size;
    
    (*name_addr_dst) = mm_malloc(SOCKET_ADDR_SIZE + sizeof(int), co->mm);
    if (!(*name_addr_dst))
    {
        SET_ERROR(co->err);
        return 0;
    }
    
    memcpy((*name_addr_dst), &(*addr).sin_addr.s_addr, sizeof(in_addr_t));
    addr_id_size = sizeof(in_addr_t);
    memcpy((*name_addr_dst) + addr_id_size, &(*addr).sin_port, sizeof(in_port_t));
    addr_id_size += sizeof(in_port_t);
    memcpy((*name_addr_dst) + addr_id_size, user_id, sizeof(int));
    addr_id_size += sizeof(int);
    
    return addr_id_size;
}

void deserialize_user(struct core_object *co, User **user_get, uint8_t *serial_user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    size_t byte_offset;
    
    memcpy(&(*user_get)->id, serial_user, sizeof((*user_get)->id));
    byte_offset = sizeof((*user_get)->id);
    (*user_get)->display_name = mm_strdup((char *) (serial_user + byte_offset), co->mm);
    byte_offset += strlen((*user_get)->display_name);
    memcpy(&(*user_get)->privilege_level, (serial_user + byte_offset), sizeof((*user_get)->privilege_level));
    byte_offset += sizeof((*user_get)->privilege_level);
    memcpy(&(*user_get)->online_status, (serial_user + byte_offset), sizeof((*user_get)->online_status));
}

void deserialize_auth(struct core_object *co, Auth **auth_get, uint8_t *serial_auth)
{
    PRINT_STACK_TRACE(co->tracer);
    
    size_t byte_offset;
    
    memcpy(&(*auth_get)->user_id, serial_auth, sizeof((*auth_get)->user_id));
    byte_offset = sizeof((*auth_get)->user_id);
    (*auth_get)->login_token = mm_strdup((char *) (serial_auth + byte_offset), co->mm);
    byte_offset += strlen((*auth_get)->login_token) + 1;
    (*auth_get)->password = mm_strdup((char *) (serial_auth + byte_offset), co->mm);
}

void free_user(struct core_object *co, User *user)
{
    PRINT_STACK_TRACE(co->tracer);
    
    mm_free(co->mm, user->display_name);
    mm_free(co->mm, user);
}

void free_auth(struct core_object *co, Auth *auth)
{
    PRINT_STACK_TRACE(co->tracer);
    
    mm_free(co->mm, auth->login_token);
    mm_free(co->mm, auth->password);
    mm_free(co->mm, auth);
}
