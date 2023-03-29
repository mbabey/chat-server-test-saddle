#ifndef SERVER_TEST_SADDLE_DB_H
#define SERVER_TEST_SADDLE_DB_H

#include "objects.h"

#define LOGIN_TOKEN_MAX_SIZE 20            /** Maximum size for login tokens. */
#define DISPLAY_NAME_MAX_SIZE 20           /** Maximum size for display names. */
#define PASSWORD_MIN_SIZE 6                /** Minimum size for passwords. */
#define PASSWORD_MAX_SIZE 30               /** Maximum size for passwords. */

#define VALIDATE_LOGIN_TOKEN(login_token) (strlen(login_token) <= LOGIN_TOKEN_MAX_SIZE)
#define VALIDATE_DISPLAY_NAME(display_name) (strlen(display_name) <= DISPLAY_NAME_MAX_SIZE)
#define VALIDATE_PASSWORD(password) (strlen(password) >= PASSWORD_MIN_SIZE && strlen(password) <= PASSWORD_MAX_SIZE)

/**
 * Count the number of tokens in a NULL-terminated list of pointers. Modifies the pointer passed.
 */
#define COUNT_TOKENS(count, tokens) for (; *tokens; ++tokens, ++count)

/**
 * db_create
 * <p>
 * Insert an object into a database.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param type the type of object to insert
 * @param object the object to insert
 * @return 0 on success, 1 if document already exists, -1 and set err of failure
 */
int db_create(struct core_object *co, struct server_object *so, int type, void *object);

/**
 * safe_dbm_store
 * <p>
 * Safely store an item in a database.
 * </p>
 * @param co the core object
 * @param db_name the database name
 * @param sem the database semaphore
 * @param key the key under which to store
 * @param value the value to store
 * @param store_flags whether to insert or overwrite
 * @return 0 on successful insert, 1 if insert and record already exists, -1 and set err on failure
 */
int safe_dbm_store(struct core_object *co, const char *db_name, sem_t *sem, datum *key, datum *value, int store_flags);

/**
 * db_read
 * <p>
 * Read an object of type from the database into object_dst based on the contents of object_query.
 * <ul>
 * <li>Single User: query by display name.</li>
 * <li>Online Users: query by NULL.</li>
 * <li>Channel: query by channel name.</li>
 * <li>Messages: query by number of messages and channel id (order matters).</li>
 * <li>Auth: query by login token.</li>
 * </ul>
 * </p>
 * @param co the core object
 * @param so the server object
 * @param type the type of object to query
 * @param object_dst the destination for the query
 * @param object_query the query
 * @return 0 on success, -1 and set err on failure
 */
int db_read(struct core_object *co, struct server_object *so, int type, void *object_dst, void *object_query);

int db_update(struct core_object *co, struct server_object *so, int type, void *object_src);

int db_destroy(struct core_object *co, struct server_object *so, int type, void *object);

/**
 * safe_dbm_delete
 * <p>
 * Safely delete an item from a database.
 * </p>
 * @param co the core object
 * @param db_name the database name
 * @param sem the database semaphore
 * @param key the key to delete
 * @return 0 on success, -1 and set err on failure.
 */
int safe_dbm_delete(struct core_object *co, const char *db_name, sem_t *sem, datum *key);

#endif //SERVER_TEST_SADDLE_DB_H
