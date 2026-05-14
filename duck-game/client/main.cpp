#include <exception>
#include <iostream>
#include <QApplication>

#include "game.h"

#define VALID_ARGC 3
#define SERVER_IP 1
#define PORT 2
#define OK 0
#define ERR 1

int main(int argc, char* argv[]) {
    try {
        if (argc != VALID_ARGC) {
            std::cerr << "Invalid amount of arguments: " << argc << " expected: " << VALID_ARGC
                      << "\n";
            return ERR;
        }

        const char* server_ip = argv[SERVER_IP];
        const char* port = argv[PORT];

        QApplication app(argc, argv);

        Game duck_game = Game(server_ip, port, app);
        duck_game.start();

        return OK;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return ERR;
    } catch (...) {
        std::cerr << "Something went wrong and an unknown exception was caught.\n";
        return ERR;
    }
}
