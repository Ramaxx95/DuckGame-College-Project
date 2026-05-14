#include "gameinstance.h"

#include <memory>

GameInstance::GameInstance(int cant_players, const Map& map):
        cant_players(cant_players),
        q_game_loop(1),
        game(q_game_loop, player_controller, map),
        is_init(false) {}

void GameInstance::start_game(Client* host) {
    std::unique_lock<std::mutex> lock(mtx);

    player_controller.add_player(std::make_unique<Player>(host->get_duck_id(), physics,
                                                          host->release_socket(), q_game_loop));

    is_ready.wait(lock, [this] { return player_controller.get_cant_players() == cant_players; });

    game_init();
}

void GameInstance::add_client_to_game(Client* client) {
    std::unique_lock<std::mutex> lock(mtx);
    player_controller.add_player(std::make_unique<Player>(client->get_duck_id(), physics,
                                                          client->release_socket(), q_game_loop));
    is_ready.notify_all();
}

void GameInstance::game_init() {
    is_init = true;
    player_controller.init_players();
    game.start();
}

bool GameInstance::is_started() const { return is_init; }

bool GameInstance::is_finalized() { return game.is_running(); }

void GameInstance::close_game() {
    game.end_game();
    std::cout << "Closing game..." << std::endl;
    game.join();
}
