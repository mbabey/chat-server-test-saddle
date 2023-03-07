#include "../include/saddle.h"
#include "../include/util.h"

#include <string.h>
#include <unistd.h>

#define OPTS_LIST "l:i:p:t"
#define USAGE_MESSAGE                                                                           \
    "./chat-test-saddle -l <server | client> -i <ip address> -p <port number> [-t]\n"                                \
    "-l <server | client>, whether to run server or client tests\n"                             \
    "-i <ip address>, if server selected for -l, will run the server at this ip address.\n"     \
    "\tif client selected for -l, will connect to a server at this ip address.\n"               \
    "-p <port number>, if server selected for -l, will run the server at this port number.\n"   \
    "\tif client selected for -l, will connect to a server at this port number.\n"              \
    "[-t], optionally trace the execution of the program."

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

int setup_saddle(struct state *state, int argc, char **argv)
{
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
    char c;
    
    while ((c = getopt(argc, argv, OPTS_LIST)) != -1)
    {
        switch (c)
        {
            case 'l':
            {
                // if "server", load server tests
                // if "client", load client tests
                break;
            }
            case 'i':
            {
                // parse IP address
                break;
            }
            case 'p':
            {
                // parse port number
                break;
            }
            case '?':
            {
                
                break;
            }
            default:
            {
                // should not get here
            }
        }
    }
    
    if (check_state(state) == -1)
    
    return 0;
}

static
