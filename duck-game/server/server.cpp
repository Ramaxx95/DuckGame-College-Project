#include "server.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "acceptor.h"
#include "gamemanager.h"
#include "reader.h"

const static std::string EXIT_CHAR = "q";
const static std::string PATH_TO_MAPS = "./maps";

Server::Server(char* port): port(port) {}

void Server::Init() {

    // inicialize config
    Reader reader(PATH_TO_MAPS);
    std::vector<Map> maps = reader.readMaps();

    for (Map map: maps) {
        map.print();
    }

    int max_active_gemes = 10;
    int max_lenght_lobby = 4;

    GameManager game_manager(max_active_gemes, max_lenght_lobby, maps);
    Acceptor acceptor(port, game_manager);

    acceptor.start();

    std::string in;
    while (std::cin >> in) {
        if (in == EXIT_CHAR) {
            break;
        }
    }

    game_manager.close();
    acceptor.stop();
    acceptor.join();
}
