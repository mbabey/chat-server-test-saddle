#ifndef SERVER_TEST_SADDLE_PSEUDO_DB_H
#define SERVER_TEST_SADDLE_PSEUDO_DB_H

#include "objects.h"

int db_create(struct core_object *co, struct server_object *so, int type);

int db_read(struct core_object *co, struct server_object *so, int type);

int db_update(struct core_object *co, struct server_object *so, int type);

int db_destroy(struct core_object *co, struct server_object *so, int type);

#endif //SERVER_TEST_SADDLE_PSEUDO_DB_H
