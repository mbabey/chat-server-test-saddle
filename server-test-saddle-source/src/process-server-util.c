#include "../../include/manager.h"
#include "../include/process-server-util.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * open_semaphores
 * <p>
 * Open the pipe read, domain read and write, and log semaphores.
 * If an error occurs opening any one of them, close and unlink them all.
 * </p>
 * @param co the core object
 * @param so the state object
 * @return 0 on success, set errno and -1 on failure
 */
static int open_semaphores(struct core_object *co, struct server_object *so);

/**
 * p_setup_parent
 * <p>
 * Set up the parent struct by allocating memory, closing unnecessary files, opening the socket,
 * and filling the first two indices of the pollfds array with the listen socket and the
 * child-to-parent pipe read end.
 * </p>
 * @param co the core object
 * @param so the state object
 * @return 0 on success, -1 on and set errno failure.
 */
static int p_setup_parent(struct core_object *co, struct server_object *so);

/**
 * p_open_process_server_for_listen
 * <p>
 * Create a socket, bind, and begin listening for connections. Fill necessary fields in the
 * core object.
 * </p>
 * @param co the core object
 * @param parent the parent object
 * @param listen_addr the address on which to listen
 * @return 0 on success, -1 and set errno on failure
 */
static int p_open_process_server_for_listen(struct core_object *co, struct parent *parent,
                                            struct sockaddr_in *listen_addr);

/**
 * c_setup_child
 * <p>
 * Set up the child struct by allocating memory and closing unnecessary files.
 * </p>
 * @param co the core object
 * @param so the state object
 * @return 0 on success, -1 and set errno of failure.
 */
static int c_setup_child(struct core_object *co, struct server_object *so);

struct server_object *setup_process_state(struct memory_manager *mm)
{
    struct server_object *so;
    
    so = (struct server_object *) mm_calloc(1, sizeof(struct server_object), mm);
    if (!so) // Depending on whether more is added to this state object, this if clause may go.
    {
        return NULL;
    }
    
    return so;
}

int open_pipe_semaphores_domain_sockets(struct core_object *co, struct server_object *so)
{
    PRINT_STACK_TRACE(co->tracer);
    
    // NOLINTNEXTLINE(android-cloexec-pipe): Intentional pipe leakage into child processes
    if (pipe(so->c_to_p_pipe_fds) == -1) // Open pipe.
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    if (open_semaphores(co, so) == -1)
    {
        return -1;
    }
    
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, so->domain_fds) == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    return 0;
}

static int open_semaphores(struct core_object *co, struct server_object *so)
{
    PRINT_STACK_TRACE(co->tracer);
    
    sem_t *pipe_write_sem;
    sem_t *domain_read_sem;
    sem_t *domain_write_sem;
    sem_t *user_db_sem;
    sem_t *channel_db_sem;
    sem_t *message_db_sem;
    sem_t *auth_db_sem;
    
    // Value 0 will block; value 1 will allow first process to enter, then behave as if value was 0.
    pipe_write_sem   = sem_open(PIPE_WRITE_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
    domain_read_sem  = sem_open(DOMAIN_READ_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    domain_write_sem = sem_open(DOMAIN_WRITE_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
    user_db_sem      = sem_open(USER_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
    channel_db_sem   = sem_open(CHANNEL_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
    message_db_sem   = sem_open(MESSAGE_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
    auth_db_sem      = sem_open(AUTH_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (pipe_write_sem == SEM_FAILED || domain_read_sem == SEM_FAILED || domain_write_sem == SEM_FAILED
        || user_db_sem == SEM_FAILED || channel_db_sem == SEM_FAILED || message_db_sem == SEM_FAILED ||
        auth_db_sem == SEM_FAILED)
    {
        SET_ERROR(co->err);
        // Closing an unopened semaphore will return -1 and set errno = EINVAL, which can be ignored.
        sem_close(pipe_write_sem);
        sem_close(domain_read_sem);
        sem_close(domain_write_sem);
        sem_close(user_db_sem);
        sem_close(channel_db_sem);
        sem_close(message_db_sem);
        sem_close(auth_db_sem);
        // Unlinking an unopened semaphore will return -1 and set errno = ENOENT, which can be ignored.
        sem_unlink(PIPE_WRITE_SEM_NAME);
        sem_unlink(DOMAIN_READ_SEM_NAME);
        sem_unlink(DOMAIN_WRITE_SEM_NAME);
        sem_unlink(USER_SEM_NAME);
        sem_unlink(CHANNEL_SEM_NAME);
        sem_unlink(MESSAGE_SEM_NAME);
        sem_unlink(AUTH_SEM_NAME);
        return -1;
    }
    
    so->c_to_p_pipe_sem_write = pipe_write_sem;
    so->domain_sems[READ_END]  = domain_read_sem;
    so->domain_sems[WRITE_END] = domain_write_sem;
    so->user_db_sem    = user_db_sem;
    so->channel_db_sem = channel_db_sem;
    so->message_db_sem = message_db_sem;
    so->auth_db_sem    = auth_db_sem;
    
    return 0;
}

int open_databases(struct core_object *co, struct server_object *so)
{
    PRINT_STACK_TRACE(co->tracer);
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    so->user_db    = dbm_open(USER_DB_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    so->channel_db = dbm_open(CHANNEL_DB_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    so->message_db = dbm_open(MESSAGE_DB_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    so->auth_db    = dbm_open(AUTH_DB_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    // NOLINTEND
    if (so->user_db == (DBM*) 0 || so->channel_db == (DBM*) 0 || so->message_db == (DBM*) 0 || so->auth_db == (DBM*) 0)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    return 0;
}

int fork_child_processes(struct core_object *co, struct server_object *so)
{
    PRINT_STACK_TRACE(co->tracer);
    
    pid_t pid;
    
    memset(so->child_pids, 0, sizeof(so->child_pids));
    FOR_EACH_CHILD_c_IN_CHILD_PIDS
    {
        pid = fork();
        if (pid == -1)
        {
            SET_ERROR(co->err);
            return -1; // will go to ERROR state.
        }
        so->child_pids[c] = pid;
        if (pid == 0)
        {
            if (c_setup_child(co, so) == -1)
            {
                return -1;
            }
            
            break; // Do not fork bomb.
        }
    }
    if (pid > 0)
    {
        if (p_setup_parent(co, so) == -1)
        {
            return -1;
        }
    }
    
    return 0;
}

static int c_setup_child(struct core_object *co, struct server_object *so)
{
    so->parent = NULL; // Here for clarity; will already be null.
    so->child  = (struct child *) mm_calloc(1, sizeof(struct child), co->mm);
    if (!so->child)
    {
        SET_ERROR(co->err);
        return -1; // Will go to ERROR state in child process.
    }
    
    close_fd_report_undefined_error(so->c_to_p_pipe_fds[READ_END], "state of parent pipe write is undefined.");
    close_fd_report_undefined_error(so->domain_fds[WRITE_END], "state of parent domain socket is undefined.");
    
    so->c_to_p_pipe_fds[READ_END] = 0;
    so->domain_fds[WRITE_END]     = 0;
    
    return 0;
}

static int p_setup_parent(struct core_object *co, struct server_object *so)
{
    so->parent = (struct parent *) mm_calloc(1, sizeof(struct parent), co->mm);
    if (!so->parent)
    {
        SET_ERROR(co->err);
        return -1;
    }
    so->child = NULL; // Here for clarity; will already be null.
    
    close_fd_report_undefined_error(so->c_to_p_pipe_fds[WRITE_END], "state of parent pipe write is undefined.");
    close_fd_report_undefined_error(so->domain_fds[READ_END], "state of parent domain socket is undefined.");
    
    so->c_to_p_pipe_fds[WRITE_END] = 0;
    so->domain_fds[READ_END]       = 0;
    
    if (p_open_process_server_for_listen(co, so->parent, &co->listen_addr) == -1)
    {
        return -1;
    }
    
    so->parent->pollfds[1].fd     = so->c_to_p_pipe_fds[READ_END];
    so->parent->pollfds[1].events = POLLIN;
    
    return 0;
}

static int p_open_process_server_for_listen(struct core_object *co, struct parent *parent,
                                            struct sockaddr_in *listen_addr)
{
    PRINT_STACK_TRACE(co->tracer);
    int fd;
    
    fd = socket(PF_INET, SOCK_STREAM, 0); // NOLINT(android-cloexec-socket): SOCK_CLOEXEC dne
    if (fd == -1)
    {
        SET_ERROR(co->err);
        return -1;
    }
    
    if (bind(fd, (struct sockaddr *) listen_addr, sizeof(struct sockaddr_in)) == -1)
    {
        SET_ERROR(co->err);
        (void) close(fd);
        return -1;
    }
    
    if (listen(fd, CONNECTION_QUEUE) == -1)
    {
        SET_ERROR(co->err);
        (void) close(fd);
        return -1;
    }
    
    // NOLINTNEXTLINE(concurrency-mt-unsafe): No threads here
    (void) fprintf(stdout, "Server running on %s:%d\n", inet_ntoa(listen_addr->sin_addr),
                   ntohs(listen_addr->sin_port));
    
    parent->pollfds[0].fd     = fd;
    parent->pollfds[0].events = POLLIN;
    
    return 0;
}

void p_destroy_parent_state(struct core_object *co, struct server_object *so, struct parent *parent)
{
    PRINT_STACK_TRACE(co->tracer);
    int status;
    
    FOR_EACH_CHILD_c_IN_CHILD_PIDS // Send signals to child processes real quick.
    {
        kill(so->child_pids[c], SIGINT);
    }
    FOR_EACH_CHILD_c_IN_CHILD_PIDS // Wait for child processes to wrap up.
    {
        waitpid(so->child_pids[c], &status, 0);
    }
    
    close_fd_report_undefined_error(so->c_to_p_pipe_fds[READ_END], "state of pipe read is undefined.");
    close_fd_report_undefined_error(so->domain_fds[WRITE_END], "state of parent domain socket is undefined.");
    
    for (size_t sfd_num = 0; sfd_num < POLLFDS_SIZE; ++sfd_num)
    {
        close_fd_report_undefined_error((parent->pollfds + sfd_num)->fd, "state of connection socket is undefined.");
    }
    
    mm_free(co->mm, parent);
    
    sem_close(so->c_to_p_pipe_sem_write);
    sem_close(so->domain_sems[READ_END]);
    sem_close(so->domain_sems[WRITE_END]);
    sem_close(so->user_db_sem);
    sem_close(so->channel_db_sem);
    sem_close(so->message_db_sem);
    sem_close(so->auth_db_sem);
    sem_unlink(PIPE_WRITE_SEM_NAME);
    sem_unlink(DOMAIN_READ_SEM_NAME);
    sem_unlink(DOMAIN_WRITE_SEM_NAME);
    sem_unlink(USER_SEM_NAME);
    sem_unlink(CHANNEL_SEM_NAME);
    sem_unlink(MESSAGE_SEM_NAME);
    sem_unlink(AUTH_SEM_NAME);
    
}

void c_destroy_child_state(struct core_object *co, struct server_object *so, struct child *child)
{
    PRINT_STACK_TRACE(co->tracer);
    close_fd_report_undefined_error(so->c_to_p_pipe_fds[WRITE_END], "state of pipe write is undefined.");
    close_fd_report_undefined_error(so->domain_fds[READ_END], "state of child domain socket is undefined.");
    
    mm_free(co->mm, child);
}

void close_databases(struct core_object *co, struct server_object *so)
{
    PRINT_STACK_TRACE(co->tracer);
    
    // NOLINTBEGIN(concurrency-mt-unsafe) : No threads here
    dbm_close(so->user_db);
    dbm_close(so->channel_db);
    dbm_close(so->message_db);
    dbm_close(so->auth_db);
    // NOLINTEND
}

void close_fd_report_undefined_error(int fd, const char *err_msg)
{
    if (close(fd) == -1)
    {
        switch (errno)
        {
            case EBADF: // Not a problem.
            {
                errno = 0;
                break;
            }
            default:
            {
                // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
                (void) fprintf(stderr, "Error: %s; %s\n", strerror(errno), err_msg);
            }
        }
    }
}
