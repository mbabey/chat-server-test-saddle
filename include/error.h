#ifndef CHAT_TEST_SADDLE_ERROR_H
#define CHAT_TEST_SADDLE_ERROR_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define SET_ERROR(err) \
    err.file = __FILE__; \
    err.func = __func__; \
    err.line = __LINE__; \
    err.error_number = errno;

#define GET_ERROR(err) \
    (void) fprintf(stderr, "Error: %d - \"%s\" in %s:%s @ %lu", err.error_number, strerror(err.error_number), err.file, err.func, err.line);

typedef struct {
    const char *file;
    const char *func;
    size_t line;
    int error_number;
} Error;

#endif //CHAT_TEST_SADDLE_ERROR_H
