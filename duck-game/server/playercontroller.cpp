#include "playercontroller.h"

#include <utility>

PlayerController::PlayerController() = default;

void PlayerController::add_player(std::unique_ptr<Player> player) {
    player_queue.push_back(std::move(player));
}


void PlayerController::send_all_players(const GameStateMessage& event) const {
    for (auto& player: player_queue) {
        player->send(event);
    }
}

int PlayerController::get_cant_players() const { return static_cast<int>(player_queue.size()); }

void PlayerController::init_players() const {
    for (auto& player: player_queue) {
        player->init();
    }
}

void PlayerController::delete_players() { player_queue.clear(); }

void PlayerController::update_all_players() const {
    for (const auto& player: player_queue) {
        player->update();
    }
}

std::array<Player_Data, 4> PlayerController::get_players_data() const {
    std::array<Player_Data, 4> players_data{};

    for (const auto& player: player_queue) {
        players_data[player->get_id() - 1] = player->get_data();
    }
    return players_data;
}

Player* PlayerController::get_player(int playerID) const {
    for (auto& player: player_queue) {
        if (player->get_id() == playerID) {
            return player.get();
        }
    }
    return nullptr;
}

int PlayerController::get_winner() const {
    long unsigned int amount_of_players_dead = 0;
    for (const auto& player: player_queue) {
        if (player->get_dead_value()) {
            amount_of_players_dead++;
        }
    }

    if (amount_of_players_dead == player_queue.size() - 1) {
        for (const auto& player: player_queue) {
            if (!player->get_dead_value()) {
                return player->get_id();
            }
        }
    }
    return -1;
}

void PlayerController::revive_all_players() {
    for (auto& player: player_queue) {
        player->set_dead(false);
    }
}

int PlayerController::set_winner(int player_id) {
    int match_wins = 0;
    for (auto& player: player_queue) {
        if (player->get_id() == player_id) {
            match_wins = player->increase_match_wins();
        }
    }
    return match_wins;
}

std::vector<std::unique_ptr<Bullet>> PlayerController::get_players_actions() const {
    std::vector<std::unique_ptr<Bullet>> players_actions;

    for (auto& player: player_queue) {
        if (player->has_weapon_actions()) {
            auto bullets = player->update_weapon();
            players_actions.insert(players_actions.end(), std::make_move_iterator(bullets.begin()),
                                   std::make_move_iterator(bullets.end()));
        }
    }

    return players_actions;
}

std::list<std::unique_ptr<Player>>& PlayerController::get_players() { return player_queue; }

PlayerController::~PlayerController() { delete_players(); }
