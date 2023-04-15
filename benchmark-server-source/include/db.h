#ifndef SERVER_TEST_SADDLE_DB_H
#define SERVER_TEST_SADDLE_DB_H

#include "objects.h"

#define LOGIN_TOKEN_MAX_SIZE 20            /** Maximum size for login tokens. */
#define NAME_MAX_SIZE 20           /** Maximum size for display names. */
#define PASSWORD_MIN_SIZE 6                /** Minimum size for passwords. */
#define PASSWORD_MAX_SIZE 30               /** Maximum size for passwords. */

#define VALIDATE_NAME(name) (strlen(name) <= NAME_MAX_SIZE)
#define VALIDATE_PASSWORD(password) (strlen(password) >= PASSWORD_MIN_SIZE && strlen(password) <= PASSWORD_MAX_SIZE)
#define VALIDATE_TIMESTAMP(timestamp) (timestamp > 0)

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
 * @return 1 on success and record found, 0 on success and record not found, -1 and set err on failure
 */
int db_read(struct core_object *co, struct server_object *so, int type, void *object_dst, void *object_query);

int db_update(struct core_object *co, struct server_object *so, int type, void *object_src);

int db_destroy(struct core_object *co, struct server_object *so, int type, void *object);

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
 * safe_dbm_fetch
 * <p>
 * Safely fetch an item from a database.
 * </p>
 * @param co the core object
 * @param db_name the name of the db from which to fetch
 * @param sem the db semaphore
 * @param key the key of the item to fetch
 * @param serial_buffer the buffer into which to copy the fetched item
 * @return 0 if successful and copy occurs, 1 if item not found, -1 and set err on failure
 */
int safe_dbm_fetch(struct core_object *co, const char *db_name, sem_t *sem, datum *key, uint8_t **serial_buffer);

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

/**
 * find_by_name
 * <p>
 * Find an entry in the database by a string name. The name must be the second parameter of the object following an int.
 * </p>
 * @param co the core object
 * @param db the database in which to search
 * @param db_sem the semaphore for the database
 * @param serial_object the object to store the result, or NULL if no resuilt is needed
 * @param name the
 * @return 0 on success and record not located, 1 on success and record located, -1 and set err on failure
 */
int find_by_name(struct core_object *co, const char *db_name, sem_t *db_sem,
                 uint8_t **serial_object, const char *name);

/**
 * find_addr_id_pair_by_id
 * <p>
 * Search the database for an Address-User ID pair by User ID.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param addr_id_pair the address id pair
 * @param id the id
 * @return 0 on success, -1 and set on failure
 */
int find_addr_id_pair_by_id(struct core_object *co, struct server_object *so, AddrIdPair *addr_id_pair, int id);

/**
 * copy_dptr_to_buffer
 * <p>
 * Allocate memory for and copy the contents of a datum dptr into a buffer. If the dptr is NULL, return 1.
 * </p>
 * @param co the core object
 * @param buffer the buffer into which to allocate
 * @param value the datum from which to copy
 * @return 0 if successful and copy occurs, 1 if datum dptr is NULL, -1 and set err on failure.
 */
int copy_dptr_to_buffer(struct core_object *co, uint8_t **buffer, datum *value);

/**
 * determine_request_sender
 * <p>
 * Given a socket address on which a dispatch was sent, find the User that sent the dispatch.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param request_sender the User object into which to store the request sender
 * @return 0 on success, -1 and set err on failure
 */
int determine_request_sender(struct core_object *co, struct server_object *so, User *request_sender);

/**
 * print_db_error
 * <p>
 * Print an error message based on the error code of passed.
 * </p>
 * @param err_code the error code
 */
void print_db_error(DBM *db);

#endif //SERVER_TEST_SADDLE_DB_H
