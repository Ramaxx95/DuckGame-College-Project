#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <utility>
#include <vector>
#include <unordered_map>
#include "gameinstance.h"
#include "lobby.h"
#include "lobbyinfo.h"
#include "client.h"
#include "common/map.h"

class GameManager {
    std::unordered_map<int, Lobby> active_lobbies;
    std::unordered_map<int, GameInstance> active_games;
    int next_lobby_id = 0;
    const std::size_t max_active_games;
    const std::size_t max_length_lobby;
    std::mutex mtx;
    std::vector<Map> maps;
    int selected_map_id;

    void delete_lobby(int lobby_id);

    void delete_closed_games();

public:
    explicit GameManager(int max_active_games, int max_length_lobby, std::vector<Map> rmap);

    // Añade el cliente al primer lobby que encuentra; si lo encuentra,
    // retorna el ID del lobby y le asina un numero al cliente, caso contrario -1.
    int add_player_to_lobby(Client* client);

    // Crea un lobby y añade cliente como host; retorna el ID del lobby.
    // caso contrario -1
    int create_lobby(Client* host);

    // Crea un juego y notifica a todos los integrantes del lobby.
    int init_game(Map map, Client* host, int lobby_id);

    // Entra al juego con el ID correspondiente.
    bool join_game(Client* client, int lobby_id);

    // Espera a que todos los clientes del lobby indicado estean ready
    void wait_ready_lobby(int lobby_id);

    void notify_ready_lobby(int lobby_id);

    std::vector<Map> getmaps();

    Map get_selected_map();

    void set_selected_map_id(int& map_id);

    // cierra todos los lobbies y las instancias de los juegos
    void close();

    int get_lobby_lenght(int lobby_id);
};


#endif //GAMEMANAGER_H
