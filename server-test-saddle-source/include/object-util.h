#ifndef TEST_SERVER_OBJECT_UTIL_H
#define TEST_SERVER_OBJECT_UTIL_H

#include "../include/objects.h"

/**
 * serialize_user
 * <p>
 * Serialize a User struct.
 * </p>
 * @param co the core object
 * @param serial_user the buffer into which to serialize the User
 * @param user the User to serialize
 * @return size of User in bytes on success, 0 and set err on failure
 */
unsigned long serialize_user(struct core_object *co, uint8_t **serial_user, const User *user);

/**
 * serialize_channel
 * <p>
 * Serialize a Channel struct.
 * </p>
 * @param co the core object
 * @param serial_channel the buffer into which to serialize the Channel
 * @param user the Channel to serialize
 * @return size of Channel in bytes on success, 0 and set err on failure
 */
unsigned long serialize_channel(struct core_object *co, uint8_t **serial_channel, const Channel *channel);

/**
 * serialize_message
 * <p>
 * Serialize a Message struct.
 * </p>
 * @param co the core object
 * @param serial_message the buffer into which to serialize the Message
 * @param user the Message to serialize
 * @return size of Message in bytes on success, 0 and set err on failure
 */
unsigned long serialize_message(struct core_object *co, uint8_t **serial_message, const Message *message);

/**
 * serialize_auth
 * <p>
 * Serialize a Auth struct.
 * </p>
 * @param co the core object
 * @param serial_auth the buffer into which to serialize the Auth
 * @param user the Auth to serialize
 * @return size of Auth in bytes on success, 0 and set err on failure
 */
unsigned long serialize_auth(struct core_object *co, uint8_t **serial_auth, const Auth *auth);

/**
 * deserialize_user
 * <p>
 * Deserialize a User into a User struct.
 * </p>
 * @param co the core object
 * @param user_get the User struct
 * @param serial_user the bytes to deserialize
 */
void deserialize_user(struct core_object *co, User **user_get, uint8_t *serial_user);

/**
 * deserialize_auth
 * <p>
 * Store a byte string version of an Auth into an Auth struct
 * </p>
 * @param co the core object
 * @param auth_get the auth in which to store the bytes
 * @param serial_auth the bytes to convert
 */
void deserialize_auth(struct core_object *co, Auth **auth_get, uint8_t *serial_auth);

#endif //TEST_SERVER_OBJECT_UTIL_H
