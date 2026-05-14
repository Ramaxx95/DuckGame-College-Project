#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H


#include <condition_variable>
#include <mutex>
#include <vector>

#include "../common/map.h"
#include "common/queue.h"

#include "client.h"
#include "gameloop.h"
#include "physicsengine.h"
#include "playeraction.h"
#include "playercontroller.h"


class GameInstance {
    PlayerController player_controller;
    int cant_players;
    Queue<PlayerAction> q_game_loop;
    GameLoop game;
    bool is_init;
    PhysicsEngine physics;

    // Para sincronizar jugadores
    std::condition_variable is_ready;
    std::mutex mtx;

private:
    void game_init();

public:
    GameInstance(int cant_players, const Map& map);

    // Añade un cliente al juego
    void add_client_to_game(Client* client);

    // Inicia el juego; este método esperará a la cantidad de jugadores que se definió en el
    // constructor.
    void start_game(Client* host);

    bool is_finalized();

    void close_game();

    bool is_started() const;
};


#endif  // GAMEINSTANCE_H
