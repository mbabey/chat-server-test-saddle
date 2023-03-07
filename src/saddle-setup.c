#include "../include/saddle.h"
#include "../include/util.h"

#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <ctype.h>

#define OPTS_LIST "l:i:p:t"
#define USAGE_MESSAGE                                                                           \
    "./chat-test-saddle -l <server | client> -i <ip address> -p <port number> [-t]\n"                                \
    "-l <server | client>, whether to run server or client tests\n"                             \
    "-i <ip address>, if server selected for -l, will run the server at this ip address.\n"     \
    "\tif client selected for -l, will connect to a server at this ip address.\n"               \
    "-p <port number>, if server selected for -l, will run the server at this port number.\n"   \
    "\tif client selected for -l, will connect to a server at this port number.\n"              \
    "[-t], optionally trace the execution of the program.\n"

/**
 * trace_reporter
 * <p>
 * Tracer function. Prints the file name, function, and line number to stdout.
 * </p>
 * @param file the file
 * @param func the function
 * @param line the line number
 */
static void trace_reporter(const char *file, const char *func, size_t line);

/**
 * parse_args
 * <p>
 * Parse command line arguments and assign state variables accordingly.
 * </p>
 * @param state the state object
 * @param argc the argument count
 * @param argv the argument vector
 * @return 0 on success, -1 and set errno on failure
 */
static int parse_args(struct state *state, int argc, char **argv);

/**
 * parse_lib
 * <p>
 * Parse and validate a library (-l) argument.
 * </p>
 * @param lib_type the -l argument
 * @param tracer tracing function
 * @return 0 on success, -1 on failure
 */
static int parse_lib(const char *lib_type, TRACER_FUNCTION_AS(tracer));

/**
 * parse_ip_and_port
 * <p>
 * Parse and validate the IP (-i) argument and port (-p) argument. If either of the IP or port are invalid,
 * print an error message. If both are valid, fill the sockaddr_in object.
 * </p>
 * @param addr the sockaddr_in object
 * @param port_num_str the port number argument
 * @param ip_addr_str the ip addr argument
 * @param tracer tracing function
 * @return 0 on success, -1 on failure
 */
static int parse_ip_and_port(struct sockaddr_in *addr, const char *port_num_str, const char *ip_addr_str,
                             TRACER_FUNCTION_AS(tracer));

/**
 * validate_port
 * <p>
 * Validate a port number. Store it in an unsigned short if valid.
 * </p>
 * @param port_num the unsigned short
 * @param port_num_str the port number argument
 * @param tracer tracing function
 * @return 0 on success, -1 on failure
 */
static int validate_port(in_port_t *port_num, const char *port_num_str, TRACER_FUNCTION_AS(tracer));

/**
 * validate_ip
 * <p>
 * Validate an IP address. Assign the sockaddr_in object if the address is valid.
 * </p>
 * @param addr the sockaddr_in object
 * @param ip_addr_str the ip addr argument
 * @param tracer tracing function
 * @return 0 on success, -1 on failure
 */
static int validate_ip(struct sockaddr_in *addr, const char *ip_addr_str, TRACER_FUNCTION_AS(tracer));

int setup_saddle(struct state *state, int argc, char **argv)
{
    memset(state, 0, sizeof(*state));
    
    state->mm = init_mem_manager();
    if (!state->mm)
    {
        SET_ERROR(state->err)
        return -1;
    }
    
    if (parse_args(state, argc, argv) == -1)
    {
        return -1;
    }
    
    memset(&state->test_functions, 0, sizeof(state->test_functions));
    if (open_lib(&state->lib, state->lib_name, &state->test_functions, state->tracer) == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    return 0;
}

static void trace_reporter(const char *file, const char *func, size_t line)
{
    (void) fprintf(stdout, "TRACE: %s : %s : @ %zu\n", file, func, line);
}

static int parse_args(struct state *state, int argc, char **argv)
{
    int        c;
    const char *port_num_str;
    const char *ip_addr_str;
    
    while ((c = getopt(argc, argv, OPTS_LIST)) != -1) // NOLINT(concurrency-mt-unsafe) : No threads here
    {
        switch (c)
        {
            case 'l':
            {
                state->lib_name = optarg;
                break;
            }
            case 'i':
            {
                ip_addr_str = optarg;
                break;
            }
            case 'p':
            {
                port_num_str = optarg;
                break;
            }
            case 't':
            {
                state->tracer = trace_reporter;
                break;
            }
            case '?':
            {
                if (isprint(optopt))
                {
                    // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
                    (void) fprintf(stderr, "Unknown option \'-%c\'.\n", optopt);
                } else
                {
                    // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
                    (void) fprintf(stderr, "Unknown option character \'\\x%x\'.\n", optopt);
                }
                break;
            }
            default:
            {
                // should not get here
            }
        }
    }
    
    int addr_err;
    int lib_err;
    
    lib_err = parse_lib(state->lib_name, state->tracer);
    addr_err = parse_ip_and_port(&state->addr, port_num_str, ip_addr_str, state->tracer);
    
    if (addr_err || lib_err)
    {
        // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
        (void) fprintf(stdout, USAGE_MESSAGE);
        return -1;
    }
    
    return 0;
}

static int parse_lib(const char *lib_type, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    if (!lib_type || strcmp("server", lib_type) != 0 || strcmp("client", lib_type) != 0)
    {
        // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
        (void) fprintf(stderr, "%s is not a valid test type\n", lib_type);
        return -1;
    }
    
    return 0;
}

static int parse_ip_and_port(struct sockaddr_in *addr, const char *port_num_str, const char *ip_addr_str,
                             TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    int       port_err;
    int       ip_err;
    in_port_t port_num;
    
    memset(addr, 0, sizeof(struct sockaddr_in));
    
    port_err = validate_port(&port_num, port_num_str, tracer);
    
    ip_err = validate_ip(addr, ip_addr_str, tracer);
    
    if (port_err || ip_err)
    {
        return -1;
    }
    
    addr->sin_port   = htons(port_num);
    addr->sin_family = AF_INET;
    
    return 0;
}

static int validate_port(in_port_t *port_num, const char *port_num_str, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    *port_num = (in_port_t) strtol(port_num_str, NULL, 10);
    
    if (*port_num > UINT16_MAX)
    {
        // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
        (void) fprintf(stderr, "%s is not a valid port number\n", port_num_str);
        return -1;
    }
    
    return 0;
}

static int validate_ip(struct sockaddr_in *addr, const char *ip_addr_str, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    
    switch (inet_pton(AF_INET, ip_addr_str, &addr->sin_addr.s_addr))
    {
        case 1: // Valid
        {
            return 0;
        }
        case 0: // Not a valid IP address
        {
            // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
            (void) fprintf(stderr, "%s is not a valid ip address\n", ip_addr_str);
            return -1;
        }
        default: // Some other error
        {
            return -1;
        }
    }
}
