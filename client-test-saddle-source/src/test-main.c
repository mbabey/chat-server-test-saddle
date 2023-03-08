#include <stdio.h>
#include <stdlib.h>

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
    if (lib_main(NULL) == -1)
    {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}