#ifndef PROCESS_SERVER_PROCESS_SERVER_UTIL_H
#define PROCESS_SERVER_PROCESS_SERVER_UTIL_H

#include "objects.h"

/**
 * setup_process_state
 * <p>
 * Set up the server object for the process server. Add it to the memory manager.
 * </p>
 * @param mm the memory manager to which the server object will be added
 * @return the server object, or NULL and set errno on failure
 */
struct server_object *setup_process_state(struct memory_manager *mm);

/**
 * open_pipe_semaphores_domain_sockets
 * <p>
 * Open the domain socket and set up the semaphores for controlling access to the child-parent pipe,
 * the domain socket, and the log file.
 * </p>
 * @param co the core object
 * @param so the server object
 * @return 0 on success, -1 and set errno on failure
 */
int open_pipe_semaphores_domain_sockets(struct core_object *co, struct server_object *so);

/**
 * open_databases
 * <p>
 * Open the User, Channel, Message, and Auth databases.
 * </p>
 * @param co the core object
 * @param so the server object
 * @return 0 on success, -1 and set err on failure
 */
int open_databases(struct core_object *co, struct server_object *so);

/**
 * fork_child_processes
 * <p>
 * Fork the main process into the specified number of child processes. Save the child pids. Setup the parent in the
 * parent process and the children in the child processes.
 * </p>
 * @param co the core object
 * @param so the server object
 * @return 0 on success, -1 and set errno on failure.
 */
int fork_child_processes(struct core_object *co, struct server_object *so);

/**
 * p_destroy_parent_state
 * <p>
 * Perform actions necessary to close the parent process: signal all child processes to end,
 * close pipe read end, close UNIX socket connection, close active connections, close semaphores,
 * free allocated memory.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param parent the parent struct
 */
void p_destroy_parent_state(struct core_object *co, struct server_object *so, struct parent *parent);

/**
 * c_destroy_child_state
 * <p>
 * Perform actions necessary to close the child process: close pipe write end, close
 * UNIX socket connection, free allocated memory.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param child the child struct
 */
void c_destroy_child_state(struct core_object *co, struct server_object *so, struct child *child);

/**
 * close_databases
 * <p>
 * Close the User, Channel, Message, and Auth databases.
 * </p>
 * @param co the core object
 * @param so the server object
 */
void close_databases(struct core_object *co, struct server_object *so);

/**
 * close_fd_report_undefined_error
 * <p>
 * Close a file descriptor and report an error which would make the file descriptor undefined.
 * </p>
 * @param fd the fd to close
 * @param err_msg the error message to print
 */
void close_fd_report_undefined_error(int fd, const char *err_msg);

#endif //PROCESS_SERVER_PROCESS_SERVER_UTIL_H
