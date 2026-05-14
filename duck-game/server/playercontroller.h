#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <array>
#include <list>
#include <memory>
#include <vector>

#include "common/gameData.h"

#include "bullet.h"
#include "gamestate.h"
#include "player.h"

class PlayerController {
    std::list<std::unique_ptr<Player>> player_queue;
    void delete_players();

public:
    PlayerController();

    // Añade un player a la lista de players
    void add_player(std::unique_ptr<Player> player);

    void update_all_players() const;

    // Agrega el evento al sender del players
    void send_all_players(const GameStateMessage& event) const;

    int get_cant_players() const;

    void init_players() const;

    int get_winner() const;

    void revive_all_players();

    int set_winner(int player_id);

    std::array<Player_Data, 4> get_players_data() const;

    std::list<std::unique_ptr<Player>>& get_players();

    Player* get_player(int playerID) const;

    std::vector<std::unique_ptr<Bullet>> get_players_actions() const;

    ~PlayerController();
};

#endif  // PLAYERCONTROLLER_H
