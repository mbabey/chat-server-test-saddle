#include "../include/ui.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * ANSI code to clear the terminal window.
 * <ul>
 * <li>033 is an escape character</li>
 * <li>[1:1H moves the cursor to the top left of the terminal window</li>
 * <li>[2J moves all text currently in the terminal to the scroll-back buffer.</li>
 * </ul>
 */
#define CLEAR_SCREEN printf("\033[1;1H\033[2J")

#define MENU "Select a test to run by entering the corresponding number. Type \"q\" to quit.\n"\
    "1./tCreate User Test\n"                                                                   \
    "2./tCreate Channel Test\n"                                                                \
    "3./tCreate Message Test\n"                                                                \
    "4./tCreate Auth Test\n"                                                                   \
    "5./tRead User Test\n"                                                                     \
    "6./tRead Channel Test\n"                                                                  \
    "7./tRead Message Test\n"                                                                  \
    "8./tUpdate User Test\n"                                                                   \
    "9./tUpdate Channel Test\n"                                                                \
    "10./tUpdate Message Test\n"                                                               \
    "11./tUpdate Auth Test\n"                                                                  \
    "12./tDestroy User Test\n"                                                                 \
    "13./tDestroy Channel Test\n"                                                              \
    "14./tDestroy Message Test\n"                                                              \
    "15./tDestroy Auth Test\n"

int run_ui(struct state_minor *state, struct client *client)
{
    char    buffer[3];
    ssize_t chars_read;
    
    CLEAR_SCREEN;
    
    (void) fprintf(stdout, MENU);
    
    // Take user input
    memset(buffer, 0, sizeof(buffer));
    chars_read = read(STDIN_FILENO, buffer, 2);
    if (chars_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    
    // parse an integer out of the input
    client->test_number = (int) strtol(buffer, NULL, 10);
    
    return 0;
}

int display_results(struct state_minor *state)
{
    // Display the results
    
    // Wait for key press
    
    return 0;
}
