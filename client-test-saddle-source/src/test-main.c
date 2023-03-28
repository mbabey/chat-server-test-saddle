#include "../../include/saddle-function.h"
#include "../../include/error-handlers.h"
#include "../include/client-objects.h"

#include <arpa/inet.h>
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define OPTS_LIST "i:p:t"
#define USAGE_MESSAGE                                                          \
    "\nusage: ./client-test-saddle -i <ip address> -p <port number> [-t]\n"      \
    "\t-i <ip address>, connect to a server at this ip address.\n"               \
    "\t-p <port number>, connect to a server at this port number.\n"             \
    "\t[-t], optionally trace the execution of the program.\n\n"

/**
 * parse_args
 * <p>
 * Parse command line arguments and assign core object variables accordingly.
 * </p>
 * @param state the state object
 * @param argc the argument count
 * @param argv the argument vector
 * @return 0 on success, -1 and set errno on failure
 */
static int parse_args(struct client_state *state, int argc, char **argv);

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

/**
 * client-test-saddle:main
 * <p>
 * Test main function for debugging.
 * </p>
 * @param argc number of arguments
 * @param argv argument vector
 * @return 0 good, 1 bad.
 */
int main(int argc, char **argv)
{
    struct client_state state;
    
    state.tracer = NULL;
    
    if (parse_args(&state, argc, argv) == -1)
    {
        return EXIT_FAILURE;
    }
    
    state.mm = init_mem_manager();
//
//    inet_pton(AF_INET, argv[1], &state.addr.sin_addr.s_addr);
//    port_number = strtol(argv[2], NULL, 10);
//    state.addr.sin_port   = htons(port_number);
//    state.addr.sin_family = AF_INET;
//

    if (lib_main(&state) == -1)
    {
        GET_ERROR(state.err);
        return EXIT_FAILURE;
    }
    
    free_mem_manager(state.mm);
    
    return EXIT_SUCCESS;
}

static int parse_args(struct client_state *state, int argc, char **argv)
{
    int        c;
    const char *port_num_str;
    const char *ip_addr_str;
    int        addr_err;
    
    port_num_str = NULL;
    ip_addr_str  = NULL;
    
    while ((c = getopt(argc, argv, OPTS_LIST)) != -1) // NOLINT(concurrency-mt-unsafe) : No threads here
    {
        switch (c)
        {
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
                // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
                (void) fprintf(stdout, USAGE_MESSAGE);
                break;
            }
            default:
            {
                // should not get here
            }
        }
    }
    
    addr_err = parse_ip_and_port(&state->addr, port_num_str, ip_addr_str, state->tracer);
    if (addr_err)
    {
        // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
        (void) fprintf(stdout, USAGE_MESSAGE);
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

static void trace_reporter(const char *file, const char *func, size_t line)
{
    (void) fprintf(stdout, "TRACE: %s : %s : @ %zu\n", file, func, line);
}

static int validate_port(in_port_t *port_num, const char *port_num_str, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    if (!port_num_str)
    {
        return -1;
    }
    
    long parsed_port_num;
    
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers): abra kadabra number will not change
    parsed_port_num = strtol(port_num_str, NULL, 10);
    
    if (parsed_port_num > UINT16_MAX)
    {
        // NOLINTNEXTLINE(concurrency-mt-unsafe) : No threads here
        (void) fprintf(stderr, "%s is not a valid port number\n", port_num_str);
        return -1;
    }
    
    *port_num = (in_port_t) parsed_port_num;
    
    return 0;
}

static int validate_ip(struct sockaddr_in *addr, const char *ip_addr_str, TRACER_FUNCTION_AS(tracer))
{
    PRINT_STACK_TRACE(tracer);
    if (!ip_addr_str)
    {
        return -1;
    }
    
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

