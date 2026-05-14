#ifndef LOBBYINFO_H
#define LOBBYINFO_H

struct LobbyInfo {
    bool is_ready;
    int id_game_session;
public:
    explicit LobbyInfo(bool ready, int id_game) : is_ready(ready), id_game_session(id_game) {};

};
#endif //LOBBYINFO_H
