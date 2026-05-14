#include "clientmanager.h"

#include <utility>

ClientManager::ClientManager(Socket&& perr, GameManager& game_manager, int id_client):
        game_manager(game_manager), running(true), client(std::move(perr), id_client) {}


void ClientManager::run() {
    try {
        // Logica para interactuar con el cliente en el lobby
        LobbyEvent action = client.recive();
        if (action.isCreationEvent()) {
            handle_creator_lobby_client();

        } else {
            handle_join_lobby_client();
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

bool ClientManager::is_running() { return running; }

// Logica para un cliente que creo un lobby
void ClientManager::handle_creator_lobby_client() {
    try {
        int lobby_id = game_manager.create_lobby(&client);
        client.send_map(game_manager.getmaps()[0]);

        client.send_map_ids(game_manager.getmaps());

        client.wait_ready();

        // Esperando todos ready
        game_manager.wait_ready_lobby(lobby_id);

        client.notify_lobby_ready();

        // Espera a que inicie el juego
        int map_id = client.wait_for_init_game();

        game_manager.set_selected_map_id(map_id);

        Map map = game_manager.getmaps()[map_id];

        if (game_manager.init_game(map, &client, lobby_id) < 0) {
            std::cout << "[ERROR - clientmanager.cpp - handle_creator_lobby_client] "
                      << "Error al iniciar el juego" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    running = false;
}

// Logica para un cliente que entro a un lobby
void ClientManager::handle_join_lobby_client() {
    try {
        int lobby_id = game_manager.add_player_to_lobby(&client);
        client.notify_join_res();
        if (lobby_id < 0) {
            // Y ahora que?
        } else {
            // Espero a que estea listo
            client.wait_ready();

            // Notifica al lobby que este cliente esta listo
            game_manager.notify_ready_lobby(lobby_id);

            game_manager.wait_ready_lobby(lobby_id);

            // Espero a que el cliente inicie el juego y obtengo el mapa que quiere
            client.wait_for_init_game();

            // Envio al cliente el mapa que eligio
            client.notify_init_game(game_manager.get_selected_map(),
                                    game_manager.get_lobby_lenght(lobby_id));
        }

        while (!game_manager.join_game(&client, lobby_id)) {
            std::cout << "[ERROR - clientmanager.cpp - handle_join_lobby_client] "
                      << "Error al unirse al juego" << std::endl;
        }
        running = false;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void ClientManager::stop() {
    if (running) {
        client.end_connection();
    }
}
