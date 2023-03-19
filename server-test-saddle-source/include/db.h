#ifndef SERVER_TEST_SADDLE_DB_H
#define SERVER_TEST_SADDLE_DB_H

#include "objects.h"

/**
 * db_create
 * <p>
 * Add an object of a type to the database list.
 * </p>
 * @param co
 * @param so
 * @param type
 * @return
 */
int db_create(struct core_object *co, struct server_object *so, int type, void *object);

int db_read(struct core_object *co, struct server_object *so, int type, void *object);

int db_update(struct core_object *co, struct server_object *so, int type, void *object);

int db_destroy(struct core_object *co, struct server_object *so, int type, void *object);

#endif //SERVER_TEST_SADDLE_DB_H
