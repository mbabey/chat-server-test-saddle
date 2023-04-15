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
 * @param channel the Channel to serialize
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
 * @param message the Message to serialize
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
 * @param auth the Auth to serialize
 * @return size of Auth in bytes on success, 0 and set err on failure
 */
unsigned long serialize_auth(struct core_object *co, uint8_t **serial_auth, const Auth *auth);

/**
 * serialize_addr_id_pair
 * <p>
 * Given a display name and a socket address, allocate memory for and create a byte
 * stream in the format [name, ip-addr, port-num].
 * </p>
 * @param co the core object
 * @param addr_id_dst the destination buffer
 * @param user_id the display name
 * @param addr the socket addr
 * @return the size of the buffer on success, -1 and set err on failure.
 */
unsigned long serialize_addr_id_pair(struct core_object *co, uint8_t **addr_id_dst, struct sockaddr_in *addr,
                                     int *user_id);

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

/**
 * deserialize_addr_id_pair
 * <p>
 * Deserialize a buffer into an AddrIdPair.
 * </p>
 * @param co the core object
 * @param addr_id_pair_get the address-id pair object in which to store the deserialized buffer
 * @param serial_addr_id the buffer to deserialize
 */
void deserialize_addr_id_pair(struct core_object *co, AddrIdPair **addr_id_pair_get, uint8_t *serial_addr_id);

/**
 * free_user
 * <p>
 * Free a user's fields then the user. Must be allocated in the memory manager.
 * </p>
 * @param co the core object
 * @param user the user to deallocate
 */
void free_user(struct core_object *co, User *user);
/**
 * free_auth
 * <p>
 * Free a auth's fields then the auth. Must be allocated in the memory manager.
 * </p>
 * @param co the core object
 * @param auth the user to deallocate
 */
void free_auth(struct core_object *co, Auth *auth);


#endif //TEST_SERVER_OBJECT_UTIL_H
