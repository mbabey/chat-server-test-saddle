#ifndef PROCESS_SERVER_OBJECTS_H
#define PROCESS_SERVER_OBJECTS_H

#include "../../include/error-handlers.h"

#include <semaphore.h>
#include <poll.h>
#include <ndbm.h>
#include <netinet/in.h>

// TODO delet this when done
#define PRINT_BYTES(array, size) \
    int ocunt = 0; \
    for (uint8_t *cp = serial_auth; ocunt < size; ++cp) { printf("%d: %hhx\n", ocunt++, *cp); }

#define NUM_CHILD_PROCESSES 8              /** The number of worker processes to be spawned to handle network requests. */
#define CONNECTION_QUEUE 100               /** The number of connections that can be queued on the listening socket. */
#define MAX_CONNECTIONS 5                  /** The maximum number of connections that can be accepted by the process server. */
#define POLLFDS_SIZE 2 + MAX_CONNECTIONS   /** The size of the pollfds array. +2 for listen socket and child-to-parent pipe. */
#define READ_END 0                         /** Read end of child_finished_pipe or read child_finished_semaphore. */
#define WRITE_END 1                        /** Write end of child_finished_pipe or read child_finished_semaphore. */

#define PIPE_WRITE_SEM_NAME "/pw_3fda69"   /** Pipe write semaphore name. */
#define DOMAIN_READ_SEM_NAME "/dr_3fda69"  /** Domain socket read semaphore name. */
#define DOMAIN_WRITE_SEM_NAME "/dw_3fda69" /** Domain socket write semaphore name. */
#define USER_SEM_NAME "/u_3fda69"          /** User db semaphore name. */
#define CHANNEL_SEM_NAME "/ch_3fda69"      /** Channel db semaphore name. */
#define MESSAGE_SEM_NAME "/m_3fda69"       /** Message db semaphore name. */
#define AUTH_SEM_NAME "/au_3fda69"         /** Auth db semaphore name. */
#define NAME_ADDR_SEM_NAME "/na_3fda69"         /** Auth db semaphore name. */

#define USER_DB_NAME "dbu_3fda69"          /** User db name. */
#define CHANNEL_DB_NAME "dbch_3fda69"      /** Channel db name. */
#define MESSAGE_DB_NAME "dbm_3fda69"       /** Message db name. */
#define AUTH_DB_NAME "dbau_3fda69"         /** Auth db name. */
#define NAME_ADDR_DB_NAME "dbna_3fda69"    /** Display name-Socket address database. */

#define DB_FLAGS O_RDWR | O_CREAT          /** Flags for opening db. */
#define DB_FILE_MODE S_IRUSR | S_IWUSR     /** File mode for opening db. */

#define FOR_EACH_CHILD_c_IN_CHILD_PIDS for (size_t c = 0; c < NUM_CHILD_PROCESSES; ++c) /** For each loop macro for looping over child processes. */
#define FOR_EACH_SOCKET_POLLFD_p_IN_POLLFDS for (size_t p = 2; p < POLLFDS_SIZE; ++p)   /** For each loop macro for looping over socket pollfds. */

/**
 * Contains information about the program state.
 */
struct core_object
{
    struct sockaddr_in listen_addr;
    struct error_saver err;
    
    TRACER_FUNCTION_AS(tracer);
    
    struct memory_manager *mm;
    
    struct server_object *so;
};

/**
 * Contains information about the server state.
 */
struct server_object
{
    pid_t         child_pids[NUM_CHILD_PROCESSES];
    int           domain_fds[2];
    int           c_to_p_pipe_fds[2];
    sem_t         *domain_sems[2];
    sem_t         *c_to_p_pipe_sem_write;
    sem_t         *user_db_sem;
    sem_t         *channel_db_sem;
    sem_t         *message_db_sem;
    sem_t         *auth_db_sem;
    sem_t         *name_addr_db_sem;
    struct parent *parent;
    struct child  *child;
};

/**
 * Contains information about the parent state.
 */
struct parent
{
    struct pollfd      pollfds[POLLFDS_SIZE]; // 0th position is the listen socket fd, 1st position is pipe.
    struct sockaddr_in client_addrs[MAX_CONNECTIONS];
    size_t             num_connections;
};

/**
 * Contains information about the child state.
 */
struct child
{
    int                client_fd_parent;
    int                client_fd_local;
    struct sockaddr_in client_addr;
};

/**
 * User. Contains information about a User.
 */
typedef struct
{
    int  id;
    char *display_name;
    int  privilege_level;
    int  online_status;
} User;

/**
 * Channel. Contains information about a Channel.
 */
typedef struct
{
    int        id;
    char       *channel_name;
    char       *creator;
    size_t     users_count;      // This is the number of users.
    const char **users;          // This is a null terminated list of names.
    size_t     users_size_bytes; // This is the number of bytes.
    
    size_t     administrators_count;
    const char **administrators;
    size_t     administrators_size_bytes;
    
    size_t     banned_users_count;
    const char **banned_users;
    size_t     banned_users_size_bytes;
} Channel;

/**
 * Message. Contains information about a Message.
 */
typedef struct
{
    int    id;
    int    user_id;
    int    channel_id;
    char   *message_content;
    time_t timestamp;
} Message;

/**
 * Auth. Contains information about an Auth.
 */
typedef struct
{
    int  user_id;
    char *login_token;
    char *password;
} Auth;

/**
 * NameAddrPair. Contains display name and associated socket address.
 */
typedef struct
{
    char      *display_name;
    in_addr_t socket_ip;
    in_port_t socket_port;
} NameAddrPair;

#endif //PROCESS_SERVER_OBJECTS_H
