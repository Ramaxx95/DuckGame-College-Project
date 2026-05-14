#ifndef PLAYERINFO_H
#define PLAYERINFO_H
#include <string>
#include <utility>

class ClientInfo {
    public:
    int id;
    int lobbyid;
    int player_number;

    ClientInfo(): id(-1), lobbyid(-1), player_number(-1) {}
};

#endif //PLAYERINFO_H
