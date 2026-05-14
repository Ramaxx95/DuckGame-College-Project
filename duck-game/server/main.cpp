#include "common/liberror.h"
#include "server.h"

#include <iostream>

static const int VALID_ARGC = 2;
static const int OK = 0;
static const int ERR = 1;

int main(int argc, char *argv[]) { try {
    if (argc != VALID_ARGC) {
        std::cerr << "Invalid amount of arguments: " << argc <<  "expected: " << VALID_ARGC << "\n";
        return ERR;
    }

    char* port = argv[1];
    Server server(port);

    server.Init();

    return OK;
} catch (const std::exception& err) {
    std::cerr
        << "Something went wrong and an exception was caught: "
        << err.what()
        << "\n";
    return ERR;
} catch (...) {
    std::cerr << "Something went wrong and an unknown exception was caught.\n";
    return ERR;
} }
