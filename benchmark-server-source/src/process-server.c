#include "../../include/manager.h"
#include "../../include/util.h"
#include "../include/chat.h"
#include "../include/process-server-util.h"
#include "../include/process-server.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // back compatability
#include <sys/types.h>  // back compatability
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables): must be non-const
/**
 * Whether the loop at the heart of the program should be running.
 */
volatile int GOGO_PROCESS = 1;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

/**
 * p_run_poll_loop
 * <p>
 * Run the process server. Wait for activity on one of the processed sockets; if activity
 * is on the listen socket, accept a new connection. If activity is on any other socket,
 * handle that message.
 * </p>
 * @param co the core object
 * @param so the state object
 * @param parent the parent struct
 * @return 0 on success, -1 and set errno on failure
 */
static int p_run_poll_loop(struct core_object *co, struct server_object *so, struct parent *parent);

/**
 * setup_signal_handler
 * <p>
 * Set up a handler for a signal.
 * </p>
 * @param sa sigaction struct to fill
 * @param signal the signal for which to listen
 * @return 0 on success, -1 and set errno on failure
 */
static int setup_signal_handler(struct sigaction *sa, int signal);

/**
 * end_gogo_handler
 * <p>
 * Handler for signal. Set the running loop conditional to 0.
 * </p>
 * @param signal the signal received
 */
static void end_gogo_handler(int signal);

/**
 * p_accept_new_connection
 * <p>
 * Accept a new connection to the server. Set the value of the fd
 * increment the num connections in the state object.
 * </p>
 * @param co the core object
 * @param parent the state object
 * @param pollfds the pollfd array
 * @return the 0 on success, -1 and set errno on failure
 */
static int p_accept_new_connection(struct core_object *co, struct parent *parent, struct pollfd *pollfds);

/**
 * p_get_pollfd_index
 * <p>
 * Find an index of a socket in the file descriptor array where file descriptor == 0.
 * </p>
 * @param pollfds the file descriptor array
 * @return the first index where file descriptor == 0
 */
static size_t p_get_pollfd_index(const struct pollfd *pollfds);

/**
 * p_read_pipe_reenable_fd
 * <p>
 * Wait for the read semaphore to be signaled on the child-to-parent pipe. Invert the fd that is passed in the pipe.
 * </p>
 * @param co the core object
 * @param so the state object
 * @param pollfds the array of pollfds
 * @return 0 on success, -1 and set errno on failure.
 */
static int p_read_pipe_reenable_fd(struct core_object *co, struct server_object *so, struct pollfd *pollfds);

/**
 * p_handle_socket_action
 * <p>
 * Send all file descriptors in pollfds for which POLLIN is set on the domain socket.
 * Remove all file descriptors in pollfds for which POLLHUP is set.
 * </p>
 * @param co the core object
 * @param so the state object
 * @param pollfds the pollfds array
 * @return 0 on success, -1 and set errno on failure
 */
static int p_handle_socket_action(struct core_object *co, struct server_object *so, struct pollfd *pollfds);

/**
 * p_send_to_child
 * <p>
 * Send an active socket over the domain socket to one of the child processes.
 * </p>
 * @param co the core object
 * @param so the state object
 * @param active_pollfd the active socket
 * @return 0 on success, -1 and set errno on failure
 */
static int p_send_to_child(struct core_object *co, struct server_object *so, struct pollfd *active_pollfd);

/**
 * p_remove_connection
 * <p>
 * Close a connection and remove the fd from the list of pollfds.
 * </p>
 * @param co the core object
 * @param parent the state object
 * @param pollfd the pollfd to close and clean
 * @param conn_index the index of the connection in the array of client_addrs
 * @param listen_pollfd the listen pollfd
 */
static void p_remove_connection(struct core_object *co, struct parent *parent,
                                struct pollfd *pollfd, size_t conn_index, struct pollfd *listen_pollfd);

/**
 * c_run_child_process
 * <p>
 * Set up and run a child process that handles action on a socket passed to it by the server.
 * </p>
 * @param co the core object
 * @param so the state object
 * @return 0 on success, -1 and set errno on failure.
 */
static int c_run_child_process(struct core_object *co, struct server_object *so);

/**
 * c_receive_and_handle_messages
 * <p>
 * Look for action on the domain socket, read the sent client socket, then send the client fd known by the parent
 * through the pipe when reading is done.
 * </p>
 * @param co the core_object
 * @param so the state object
 * @param child the child struct
 * @return 0 on success, -1 and set errno on failure
 */
static int c_receive_and_handle_messages(struct core_object *co, struct server_object *so, struct child *child);

/**
 * c_get_file_description_from_domain_socket
 * <p>
 * Wait on the domain socket read semaphore for a file description to be sent on the domain socket. Put the domain
 * socket information into the child struct.
 * </p>
 * @param co the core object
 * @param so the state object
 * @param child the child struct
 * @return 0 on success, -1 and set errno on failure.
 */
static int c_get_file_description_from_domain_socket(struct core_object *co, struct server_object *so,
                                                     struct child *child);

/**
 * c_handle_network_dispatch
 * <p>
 * Handle a request-response of a network dispatch.
 * </p>
 * @param co the core object
 * @param so the server object
 * @param child the child object
 * @return 0 on success, -1 and set err on failure
 */
static int c_handle_network_dispatch(struct core_object *co, struct server_object *so, struct child *child);

/**
 * c_inform_parent_recv_finished
 * <p>
 * Send the original fd number to the parent over the child-to-parent pipe.
 * </p>
 * @param co the core object
 * @param so the state object
 * @param child the child struct
 * @return 0 on success, -1 and set errno on failure.
 */
static int c_inform_parent_recv_finished(struct core_object *co, struct server_object *so, struct child *child);

int setup_process_server(struct core_object *co, struct server_object *so)
{
    PRINT_STACK_TRACE(co->tracer);
    
    // Set up the headers for the log file.
    
    so = setup_process_state(co->mm);
    if (!so)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    co->so = so;
    
    if (open_pipe_semaphores_domain_sockets(co, so) == -1)
    {
        return -1;
    }
    
    GOGO_PROCESS = 1;
    
    if (fork_child_processes(co, so) == -1)
    {
        return -1;
    }
    
    return 0;
}

int run_process_server(struct core_object *co, struct server_object *so)
{
    PRINT_STACK_TRACE(co->tracer);
    
    // In parent, child will be NULL. In child, parent will be NULL. This behaviour can be used to identify if child or parent.
    if (so->parent)
    {
        if (p_run_poll_loop(co, so, so->parent) == -1)
        {
            return -1;
        }
    } else if (so->child)
    {
        if (c_run_child_process(co, so) == -1)
        {
            return -1;
        }
    }
    
    return 0;
}

static int p_run_poll_loop(struct core_object *co, struct server_object *so, struct parent *parent)
{
    PRINT_STACK_TRACE(co->tracer);
    struct sigaction sigint;
    int              poll_status;
    struct pollfd    *pollfds;
    nfds_t           nfds;
    
    if (setup_signal_handler(&sigint, SIGINT) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    if (setup_signal_handler(&sigint, SIGTERM) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    pollfds = parent->pollfds;
    nfds    = POLLFDS_SIZE;
    
    while (GOGO_PROCESS)
    {
        poll_status = poll(pollfds, nfds, -1);
        if (poll_status == -1)
        {
            SET_ERROR(co->err);
            return (errno == EINTR) ? 0 : -1;
        }
        
        if ((*pollfds).revents == POLLIN) // Action on the listen socket.
        {
            if (p_accept_new_connection(co, so->parent, pollfds) == -1)
            {
                return -1;
            }
        } else if ((*(pollfds + 1)).revents == POLLIN) // Action on child-to-parent pipe.
        {
            if (p_read_pipe_reenable_fd(co, so, pollfds) == -1)
            {
                return -1;
            }
        } else // Action on a client socket.
        {
            if (p_handle_socket_action(co, so, pollfds) == -1)
            {
                return -1;
            }
        }
    }
    
    return 0;
}

static int setup_signal_handler(struct sigaction *sa, int signal)
{
    sigemptyset(&sa->sa_mask);
    sa->sa_flags   = 0;
    sa->sa_handler = end_gogo_handler;
    if (sigaction(signal, sa, 0) == -1)
    {
        return -1;
    }
    return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void end_gogo_handler(int signal)
{
    GOGO_PROCESS = 0;
}

#pragma GCC diagnostic pop

static int p_accept_new_connection(struct core_object *co, struct parent *parent, struct pollfd *pollfds)
{
    PRINT_STACK_TRACE(co->tracer);
    int       new_cfd;
    size_t    pollfd_index;
    socklen_t sockaddr_size;
    
    pollfd_index  = p_get_pollfd_index(parent->pollfds);
    sockaddr_size = sizeof(struct sockaddr_in);
    
    // pollfds->fd is listen socket.
    new_cfd = accept(pollfds->fd, (struct sockaddr *) &parent->client_addrs[pollfd_index - 2], &sockaddr_size);
    if (new_cfd == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    // Only save in array if valid.
    pollfds[pollfd_index].fd     = new_cfd; // Plus one because listen_fd.
    pollfds[pollfd_index].events = POLLIN;
    ++parent->num_connections;
    
    // Don't need to short-circuit here; will only be in this function if listen socket events == POLLIN.
    if (parent->num_connections >= MAX_CONNECTIONS)
    {
        pollfds->events = 0; // Turn off POLLIN on the listening socket when max connections reached.
    }
    
    // NOLINTNEXTLINE(concurrency-mt-unsafe): No threads here
    (void) fprintf(stdout, "Client connected from %s:%d\n", inet_ntoa(parent->client_addrs[pollfd_index - 2].sin_addr),
                   ntohs(parent->client_addrs[pollfd_index - 2].sin_port));
    
    return 0;
}

static size_t p_get_pollfd_index(const struct pollfd *pollfds)
{
    size_t conn_index = 2;
    
    FOR_EACH_SOCKET_POLLFD_p_IN_POLLFDS
    {
        if (pollfds[p].fd == 0)
        {
            conn_index = p;
            break;
        }
    }
    
    return conn_index;
}

static int p_read_pipe_reenable_fd(struct core_object *co, struct server_object *so, struct pollfd *pollfds)
{
    PRINT_STACK_TRACE(co->tracer);
    int     fd;
    ssize_t bytes_read;
    
    bytes_read = read(so->c_to_p_pipe_fds[READ_END], &fd, sizeof(int));
    
    sem_post(so->c_to_p_pipe_sem_write);
    
    if (bytes_read == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    FOR_EACH_SOCKET_POLLFD_p_IN_POLLFDS
    {
        // Case: the fd has disconnected
        if (pollfds[p].fd == fd) // pollfd.fd here is negative.
        {
            pollfds[p].fd = pollfds[p].fd * -1;  // Invert pollfd.fd so it will be read from in poll loop.
            pollfds[p].revents = POLLHUP;
        }
        // Case: reenable the fd
        if (pollfds[p].fd == fd * -1) // pollfd.fd here is negative.
        {
            pollfds[p].fd = pollfds[p].fd * -1; // Invert pollfd.fd so it will be read from in poll loop.
            break;
        }
    }
    
    return 0;
}

static int p_handle_socket_action(struct core_object *co, struct server_object *so, struct pollfd *pollfds)
{
    PRINT_STACK_TRACE(co->tracer);
    struct pollfd *pollfd;
    
    FOR_EACH_SOCKET_POLLFD_p_IN_POLLFDS
    {
        pollfd = pollfds + p;
        if (pollfd->revents == POLLIN)
        {
            if (p_send_to_child(co, so, pollfd) == -1)
            {
                return -1;
            }
            pollfd->fd *= -1; // Disable the pollfd until it is signaled by the child to be re-enabled.
            
            // NOLINTNEXTLINE(hicpp-signed-bitwise): never negative
        } else if ((pollfd->revents & POLLHUP) || (pollfd->revents & POLLERR) || (pollfd->revents & POLLNVAL)) // Client has closed other end of socket.
            // On macOS, POLLHUP will be set; on Linux, POLLERR will be set.
        {
            (p_remove_connection(co, so->parent, pollfd, p - 2, pollfds));
        }
        pollfd->revents = 0; // Reset revents to be sure.
    }
    
    return 0;
}

static int p_send_to_child(struct core_object *co, struct server_object *so, struct pollfd *active_pollfd)
{
    PRINT_STACK_TRACE(co->tracer);
    ssize_t        bytes_sent;
    struct msghdr  msghdr;
    struct iovec   iovec;
    struct cmsghdr *cmsghdr;
    char           control_buffer[CMSG_SPACE(sizeof(int))]; // Create space for one cmsghdr storing an integer.
    
    memset(&msghdr, 0, sizeof(struct msghdr));
    memset(&iovec, 0, sizeof(struct iovec));
    memset(&control_buffer, 0, sizeof(control_buffer));
    
    iovec.iov_base = &active_pollfd->fd; // The original file descriptor number to send.
    iovec.iov_len  = sizeof(int);
    
    msghdr.msg_iov        = &iovec; // Put the IO vector in the msghdr to send.
    msghdr.msg_iovlen     = 1;
    msghdr.msg_control    = control_buffer; // Put the control buffer (containing the cmsghdr) into the msghdr to send.
    msghdr.msg_controllen = sizeof(control_buffer);
    
    cmsghdr = CMSG_FIRSTHDR(&msghdr);
    if (!cmsghdr)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    cmsghdr->cmsg_level = SOL_SOCKET;
    cmsghdr->cmsg_type  = SCM_RIGHTS; // Indicates it is a file description being sent.
    cmsghdr->cmsg_len   = CMSG_LEN(sizeof(int));
    // NOLINTNEXTLINE(clang-diagnostic-cast-align): Intentional cast.
    *((int *) CMSG_DATA(cmsghdr)) = active_pollfd->fd; // The file description to send.
    
    if (sem_wait(so->domain_sems[WRITE_END]) == -1)
    {
        SET_ERROR(co->err);
        return (errno == EINTR) ? 0 : -1;
    }
    bytes_sent = sendmsg(so->domain_fds[WRITE_END], &msghdr, 0); // Send the msghdr.
    if (bytes_sent == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    sem_post(so->domain_sems[READ_END]);
    
    return 0;
}

static void p_remove_connection(struct core_object *co, struct parent *parent,
                                struct pollfd *pollfd, size_t conn_index, struct pollfd *listen_pollfd)
{
    PRINT_STACK_TRACE(co->tracer);
    
    close_fd_report_undefined_error(pollfd->fd, "state of client socket is undefined.");
    
    // NOLINTNEXTLINE(concurrency-mt-unsafe): No threads here
    (void) fprintf(stdout, "Client from %s:%d disconnected\n", inet_ntoa(parent->client_addrs[conn_index].sin_addr),
                   ntohs(parent->client_addrs[conn_index].sin_port));
    
    // Zero the pollfd struct, the client_addr in the parent object, and decrement the connection count.
    memset(pollfd, 0, sizeof(struct pollfd));
    memset(&parent->client_addrs[conn_index], 0, sizeof(struct sockaddr_in));
    --parent->num_connections;
    
    // Short-circuit to prevent reassignment.
    if (listen_pollfd->events != POLLIN && parent->num_connections < MAX_CONNECTIONS)
    {
        listen_pollfd->events = POLLIN; // Turn on POLLIN on the listening socket when less than max connections.
    }
}

static int c_run_child_process(struct core_object *co, struct server_object *so)
{
    PRINT_STACK_TRACE(co->tracer);
    pid_t            pid;
    struct sigaction sigint;
    
    pid = getpid();
    
    (void) fprintf(stdout, "Child process with pid %d started.\n", pid);
    
    if (setup_signal_handler(&sigint, SIGINT) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    if (setup_signal_handler(&sigint, SIGTERM) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    if (c_receive_and_handle_messages(co, so, so->child) == -1)
    {
        return -1;
    }
    
    (void) fprintf(stdout, "Child process with pid %d winding down.\n", pid);
    
    return 0;
}

static int c_receive_and_handle_messages(struct core_object *co, struct server_object *so, struct child *child)
{
    PRINT_STACK_TRACE(co->tracer);
    
    int status;
    
    // Child processes will loop here.
    while (GOGO_PROCESS)
    {
        // Clean the child struct.
        memset(child, 0, sizeof(struct child));
        
        if (c_get_file_description_from_domain_socket(co, so, child) == -1)
        {
            return -1;
        }
        
        if (errno == EINTR)
        {
            break;
        }
    
        status = c_handle_network_dispatch(co, so, child);
        if (status == -1)
        {
            return -1;
        }
        if (status == 1)
        {
            child->client_fd_parent *= -1; // Indicate to the parent that the client has disconnected
        }
        
        if (c_inform_parent_recv_finished(co, so, child) == -1)
        {
            return -1;
        }
        
        close_fd_report_undefined_error(child->client_fd_local, "state of child receive socket undefined.");
    }
    
    return 0;
}

static int c_get_file_description_from_domain_socket(struct core_object *co, struct server_object *so,
                                                     struct child *child)
{
    PRINT_STACK_TRACE(co->tracer);
    
    ssize_t        bytes_recv;
    struct msghdr  msghdr;
    struct iovec   iovec;
    struct cmsghdr *cmsghdr;
    char           control_buffer[CMSG_SPACE(sizeof(int))]; // Create space for one cmsghdr storing an integer.
    socklen_t      socklen;
    
    memset(&msghdr, 0, sizeof(struct msghdr));
    memset(&iovec, 0, sizeof(struct iovec));
    memset(&control_buffer, 0, sizeof(control_buffer));
    
    iovec.iov_base = &child->client_fd_parent; // The original file descriptor.
    iovec.iov_len  = sizeof(int);
    
    msghdr.msg_iov        = &iovec; // Put the IO vector in the msghdr to receive.
    msghdr.msg_iovlen     = 1;
    msghdr.msg_control    = control_buffer; // Put the control buffer into the msghdr to receive.
    msghdr.msg_controllen = sizeof(control_buffer);
    
    if (sem_wait(so->domain_sems[READ_END]) == -1) // Wait for the domain socket read semaphore.
    {
        SET_ERROR(co->err);
        return (errno == EINTR) ? 0 : -1;
    }
    
    bytes_recv = recvmsg(so->domain_fds[READ_END], &msghdr, 0);
    
    sem_post(so->domain_sems[WRITE_END]); // Signal the domain socket write semaphore.
    
    if (bytes_recv == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    // Store the information from the message in the child object.
    cmsghdr = CMSG_FIRSTHDR(&msghdr);
    child->client_fd_local = *((int *) CMSG_DATA(cmsghdr)); // The file description.
    socklen = sizeof(child->client_addr);
    if (getpeername(child->client_fd_local, (struct sockaddr *) &child->client_addr, &socklen) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    // NOLINTNEXTLINE(concurrency-mt-unsafe): No threads here
    (void) fprintf(stdout, "Child %d handling message from %s:%d\n", getpid(), inet_ntoa(child->client_addr.sin_addr),
                   ntohs(child->client_addr.sin_port));
    
    return 0;
}

static int c_handle_network_dispatch(struct core_object *co, struct server_object *so, struct child *child)
{
    PRINT_STACK_TRACE(co->tracer);
    
    struct dispatch dispatch;
    char            **body_tokens;
    int             status;
    
    // receive the dispatch
    memset(&dispatch, 0, sizeof(dispatch));
    
    status = recv_parse_message((struct state *) co, child->client_fd_local,
                                &dispatch, &body_tokens);
    if (status == -1)
    {
        return -1;
    }
    if (status == 1)
    {
        return 1;
    }
    
    if (dispatch.body_size == 0)
    {
        body_tokens = NULL;
    }
    print_dispatch((struct state *) co, &dispatch, "Request");
    
    mm_free(co->mm, dispatch.body); // Free the body after tokenizing.
    
    if (perform_dispatch_operation(co, so, &dispatch, body_tokens) == -1)
    {
        dispatch.body      = mm_strdup("500\x03", co->mm);
        dispatch.body_size = strlen(dispatch.body);
        GET_ERROR(co->err); // NOLINT(mt-concurrency-unsafe) : No threads here.
    }
    
    free_body_tokens((struct state *) co, body_tokens); // Free the body tokens after performing the operation.
    
    print_dispatch((struct state *) co, &dispatch, "Response");
    
    status = assemble_message_send((struct state *) co, child->client_fd_local, &dispatch);
    mm_free(co->mm, dispatch.body);
    if (status == -1)
    {
        return -1;
    }
    
    return 0;
}

static int c_inform_parent_recv_finished(struct core_object *co, struct server_object *so, struct child *child)
{
    PRINT_STACK_TRACE(co->tracer);
    ssize_t bytes_written;
    
    if (sem_wait(so->c_to_p_pipe_sem_write) == -1) // Wait for the pipe write semaphore.
    {
        SET_ERROR(co->err);
        return (errno == EINTR) ? 0 : -1;
    }
    
    // FD will be negative if the parent should close it.
    bytes_written = write(so->c_to_p_pipe_fds[WRITE_END], &child->client_fd_parent, sizeof(int));
    
    if (bytes_written == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    return 0;
}

void destroy_process_state(struct core_object *co, struct server_object *so)
{
    PRINT_STACK_TRACE(co->tracer);
    
    if (so->parent)
    {
        p_destroy_parent_state(co, so, so->parent);
    } else if (so->child)
    {
        c_destroy_child_state(co, so, so->child);
        mm_free_all(co->mm);
        exit(EXIT_SUCCESS);
    }
}
