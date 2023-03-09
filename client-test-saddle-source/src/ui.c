#include "../include/ui.h"
#include "saddle-client.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * ANSI code to clear the terminal window.
 * <ul>
 * <li>033 is an escape character</li>
 * <li>[1:1H moves the cursor to the top left of the terminal window</li>
 * <li>[2J moves all text currently in the terminal to the scroll-back buffer.</li>
 * </ul>
 */
#define CLEAR_SCREEN printf("\033[1;1H\033[2J")

/**
 * Flush standard input of extra characters to newline or EOF.
 */
#define FLUSH_STDIN(capture) while (((capture) = getchar()) != '\n' && (capture) != EOF)

#define MENU \
    "+------------------------------------------------------------------------------+\n"\
    "|Select a test to run.                                                         |\n"\
    "+------------------------------------------------------------------------------+\n"\
    "|1.\tCreate User Test                                                       |\n"\
    "|2.\tCreate Channel Test                                                    |\n"\
    "|3.\tCreate Message Test                                                    |\n"\
    "|4.\tCreate Auth Test                                                       |\n"\
    "|5.\tRead User Test                                                         |\n"\
    "|6.\tRead Channel Test                                                      |\n"\
    "|7.\tRead Message Test                                                      |\n"\
    "|8.\tUpdate User Test                                                       |\n"\
    "|9.\tUpdate Channel Test                                                    |\n"\
    "|10.\tUpdate Message Test                                                    |\n"\
    "|11.\tUpdate Auth Test                                                       |\n"\
    "|12.\tDestroy User Test                                                      |\n"\
    "|13.\tDestroy Channel Test                                                   |\n"\
    "|14.\tDestroy Message Test                                                   |\n"\
    "|15.\tDestroy Auth Test                                                      |\n"\
    "+------------------------------------------------------------------------------+\n"\
    "Enter number or type \"q\" to quit:\n"

#define INPUT_LINE_SIZE 2

#define IS_LF_OR_EOF(character) ((character) == '\n' || (character) == EOF)

int run_ui(struct state_minor *state, struct client *client)
{
    PRINT_STACK_TRACE(state->tracer);
    
    char    buffer[INPUT_LINE_SIZE + 1];
    char    capture;
    ssize_t chars_read;
    
    CLEAR_SCREEN;
    
    (void) fprintf(stdout, MENU);
    
    // Take user input
    memset(buffer, 0, sizeof(buffer));
    chars_read = read(STDIN_FILENO, buffer, INPUT_LINE_SIZE);
    if (chars_read == -1)
    {
        SET_ERROR(state->err);
        return -1;
    }
    if (!IS_LF_OR_EOF(*buffer) || !IS_LF_OR_EOF(*(buffer + 1))) // TODO: Buffer flushing not working, needs assessment
    {
        FLUSH_STDIN(capture);
    }
    
    (void) fprintf(stdout, "\n");
    
    // Set to STOP test_number to stop if input is 'q'; otherwise, set test_number.
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers) : num will not change
    client->test_number = (buffer[0] == 'q') ? STOP : (int) strtol(buffer, NULL, 10);
    
    return 0;
}

int display_results(struct state_minor *state)
{
    PRINT_STACK_TRACE(state->tracer);
    
    // Display the results
    
    // Wait for key press
    (void) fprintf(stdout, "Press any key to continue.\n");
    getchar();
    
    return 0;
}
