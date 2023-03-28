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
    "|6.\tRead All Online User Test                                              |\n"\
    "|7.\tRead Channel Test                                                      |\n"   \
    "|8.\tRead Channel Users Test                                                |\n"\
    "|9.\tRead Channel Administrators Test                                       |\n"\
    "|10.\tRead Channel Banned Users Test                                         |\n"\
    "|11.\tRead Channel All Test                                                  |\n"\
    "|12.\tRead Message Test                                                      |\n"\
    "|13.\tUpdate User Display Name Test                                          |\n"   \
    "|14.\tUpdate User Display Name Reset                                         |\n"\
    "|15.\tUpdate User Privilege Level Test                                       |\n"\
    "|16.\tUpdate User Privilege Level Reset                                      |\n"\
    "|17.\tUpdate User Online Status Test                                         |\n"\
    "|18.\tUpdate User Online Status Reset                                        |\n"\
    "|19.\tUpdate User All Test                                                   |\n"\
    "|20.\tUpdate User All Reset                                                  |\n"\
    "|21.\tUpdate Channel Test                                                    |\n"\
    "|22.\tUpdate Message Test                                                    |\n"\
    "|23.\tUpdate Auth Test                                                       |\n"\
    "|24.\tUpdate Auth No Password Test                                           |\n"\
    "|25.\tDestroy User Test                                                      |\n"\
    "|26.\tDestroy User No Password Test                                          |\n"\
    "|27.\tDestroy Channel Test                                                   |\n"\
    "|28.\tDestroy Message Test                                                   |\n"\
    "|29.\tDestroy Auth Test                                                      |\n"\
    "+------------------------------------------------------------------------------+\n"\
    "Enter number or type \"q\" to quit:\n"

/**
 * Number of characters that are scanned in the input.
 */
#define INPUT_LINE_SIZE 2

/**
 * Determine whether a character is a linefeed or end of file.
 */
#define IS_LF_OR_EOF(character) ((character) == '\n' || (character) == EOF)

int run_ui(struct client_state *state)
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
    if (!(IS_LF_OR_EOF(*buffer) || IS_LF_OR_EOF(*(buffer + 1))))
    {
        FLUSH_STDIN(capture);
    }
    
    (void) fprintf(stdout, "\n");
    
    // Set to STOP test_number to stop if input is 'q'; otherwise, set test_number.
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers) : num will not change
    state->test_number = (buffer[0] == 'q') ? STOP : (int) strtol(buffer, NULL, 10);
    
    return 0;
}

int display_results(struct client_state *state)
{
    PRINT_STACK_TRACE(state->tracer);
    
    char capture;
    // Display the results
    
    // Wait for key press
    (void) fprintf(stdout, "Press enter/return to continue.\n");
    FLUSH_STDIN(capture);
    
    return 0;
}
