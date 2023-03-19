#ifndef PROCESS_SERVER_OBJECTS_H
#define PROCESS_SERVER_OBJECTS_H

#include "../../include/error-handlers.h"

#include <semaphore.h>
#include <poll.h>
#include <netinet/in.h>

/**
 * The number of worker processes to be spawned to handle network requests.
 */
#define NUM_CHILD_PROCESSES 8

/**
* The number of connections that can be queued on the listening socket.
*/
#define CONNECTION_QUEUE 100

/**
* The maximum number of connections that can be accepted by the process server.
*/
#define MAX_CONNECTIONS 5

/**
 * The size of the pollfds array. +2 for listen socket and child-to-parent pipe.
 */
#define POLLFDS_SIZE 2 + MAX_CONNECTIONS

/**
* Read end of child_finished_pipe or read child_finished_semaphore.
*/
#define READ_END 0

/**
* Write end of child_finished_pipe or read child_finished_semaphore.
*/
#define WRITE_END 1

/**
* Pipe write semaphore name.
*/
#define PIPE_WRITE_SEM_NAME "/p_2f6a08" // Random hex to prevent collision of this filename with others.

/**
* Domain socket read semaphore name.
*/
#define DOMAIN_READ_SEM_NAME "/dr_2f6a08"

/**
* Domain socket write semaphore name.
*/
#define DOMAIN_WRITE_SEM_NAME "/dw_2f6a08"

/**
* Log semaphore name.
*/
#define LOG_SEM_NAME "/l_2f6a08"

/**
 * For each loop macro for looping over child processes.
 */
#define FOR_EACH_CHILD_c_IN_CHILD_PIDS for (size_t c = 0; c < NUM_CHILD_PROCESSES; ++c)

/**
 * For each loop macro for looping over socket pollfds.
 */
#define FOR_EACH_SOCKET_POLLFD_p_IN_POLLFDS for (size_t p = 2; p < POLLFDS_SIZE; ++p)

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
    sem_t         *log_sem;
    struct parent *parent;
    struct child  *child;
};

/**
 * User. Contains information about a User.
 */
typedef struct
{
    int  id;
    char *display_name;
} User;

/**
 * Channel. Contains information about a Channel.
 */
typedef struct
{
    int  id;
    char *channel_name;
    char *creator;
    User *users;
    User *administrators;
    User *banned_users;
} Channel;

/**
 * Message. Contains information about a Message.
 */
typedef struct
{
    int  id;
    int  user_id;
    int  channel_id;
    char *message_content;
    int  timestamp;
} Message;

/**
 * Auth. Contains information about a Auth.
 */
typedef struct
{
    int        user_id;
    const char *login_token;
    char       *password;
} Auth;

/**
 * Contains information about the parent state.
 */
struct parent
{
    struct pollfd      pollfds[POLLFDS_SIZE]; // 0th position is the listen socket fd, 1st position is pipe.
    struct sockaddr_in client_addrs[MAX_CONNECTIONS];
    size_t             num_connections;
    
    User    *user_list;
    Channel *channel_list;
    Message *message_list;
    Auth    *auth_list;
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

#endif //PROCESS_SERVER_OBJECTS_H
