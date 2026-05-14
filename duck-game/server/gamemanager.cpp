#include "gamemanager.h"
namespace {
    const int HOST_NUMBER = 1;
}
GameManager::GameManager(const int max_active_games, 
    const int max_length_lobby, std::vector<Map> rmap): 
    max_active_games(max_active_games), max_length_lobby(max_length_lobby), maps(rmap),
    selected_map_id(0) {}

int GameManager::create_lobby(Client* host) {
    std::lock_guard<std::mutex> lock(mtx);

    if (active_lobbies.size() >= max_active_games) {
        return -1;
    }

    const int lobby_id = next_lobby_id++;

    auto result = active_lobbies.try_emplace(lobby_id, max_length_lobby, host->get_id());

    if (!result.first->second.add_to_lobby(host)) {
        active_lobbies.erase(lobby_id);
        return -1;
    }

    host->set_player_number(HOST_NUMBER);

    return lobby_id;
}

int GameManager::add_player_to_lobby(Client* const client) {
    std::lock_guard<std::mutex> lock(mtx);

    for (auto& [lobby_id, lobby] : active_lobbies) {
        if (!lobby.is_full() && !lobby.is_closed()) {
            lobby.add_to_lobby(client);
            return lobby_id;
        }
    }
    return -1;
}

void GameManager::wait_ready_lobby(int lobby_id) {
    if (auto it = active_lobbies.find(lobby_id); it != active_lobbies.end()) {
        auto& lobby = it->second;
        lobby.wait_for_lobby_ready();
    }
}

void GameManager::notify_ready_lobby(int lobby_id) {
    if (auto it = active_lobbies.find(lobby_id); it != active_lobbies.end()) {
        auto& lobby = it->second;
        lobby.notify_lobby_ready();
    }
}


int GameManager::init_game(Map map, Client* host, int lobby_id) {
    const auto lobby_it = active_lobbies.find(lobby_id);

    if (lobby_it == active_lobbies.end()) {
        return -1;
    }

    Lobby& lobby = lobby_it->second;

    if (lobby.get_host_id() != host->get_id() || !lobby.is_ready()) {
        return -1;
    }

    std::vector<Client*> clients_to_notify;
    GameInstance* game_instance = nullptr;

    {
        std::lock_guard<std::mutex> lock(mtx);

        // Crea un juego
        active_games.try_emplace(lobby_id, lobby.get_length(), map);

        lobby.notify_lobby_ready();

        // Obtiene todos los clientes del lobby y lo cierra
        clients_to_notify = lobby.get_lobby_and_close();


        //TODO borrar el lobby aca directamente no deberia de modificar el funcionamiento

        game_instance = &active_games.at(lobby_id);
    }


    if (game_instance) {
        for (const auto& client : clients_to_notify) {
            if (client) {
                if (client->get_id() == host->get_id()) {
                    client->notify_init_game(map, lobby.get_length());
                } else {
                    client->notify_lobby_ready();
                }
            }
        }
        game_instance->start_game(host);
        delete_lobby(lobby_id);
    }
    return 0;
}

bool GameManager::join_game(Client* client, int lobby_id) {
    std::lock_guard<std::mutex> lock(mtx);
    auto game_it = active_games.find(lobby_id);
    if (game_it != active_games.end() && !game_it->second.is_started()) {
        game_it->second.add_client_to_game(client);
        return true;
    }
    return false;
}

void GameManager::delete_lobby(int lobby_id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (auto it = active_lobbies.find(lobby_id); it != active_lobbies.end()) {
        active_lobbies.erase(it);
    }
}

void GameManager::delete_closed_games() {
    std::lock_guard<std::mutex> lock(mtx);

    auto it = active_games.begin();
    while (it != active_games.end()) {
        if (it->second.is_finalized()) {
            it->second.close_game();
            it = active_games.erase(it);
        } else {
            ++it;
        }
    }
}


std::vector<Map> GameManager::getmaps(){
    return maps;
}

Map GameManager::get_selected_map() {
    return maps[selected_map_id];
}

void GameManager::set_selected_map_id(int& map_id) {
    selected_map_id = map_id;
}

void GameManager::close() {
    auto it = active_lobbies.begin();
    while (it != active_lobbies.end()) {
        if (!it->second.is_closed()) {
            it->second.close();
        } else {
            ++it;
        }
    }
    auto it_g = active_games.begin();
    while (it_g != active_games.end()) {
        if (it_g->second.is_finalized()) {
            it_g->second.close_game();
        } else {
            ++it_g;
        }
    }
    // No es necesario ya que el destructor los borra directamente cuando se va de scope
    // Pero es mejor aclarar y limpiar los diccionarios aca
    active_lobbies.clear();
    active_games.clear();
}

int GameManager::get_lobby_lenght(int lobby_id) {
    if (auto it = active_lobbies.find(lobby_id); it != active_lobbies.end()) {
        auto& lobby = it->second;
        return  lobby.get_length();
    }
    return -1;
}
