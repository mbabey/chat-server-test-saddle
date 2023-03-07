#ifndef CHAT_TEST_SADDLE_SADDLE_H
#define CHAT_TEST_SADDLE_SADDLE_H

#include <netinet/in.h>
#include <sys/types.h>

struct state {
    char *lib_name;
    struct sockaddr_in addr;
    in_port_t port_number;
};

int run_saddle(int argc, char **argv);

#endif //CHAT_TEST_SADDLE_SADDLE_H
