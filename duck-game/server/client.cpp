#include "client.h"
#include <utility>

Client::Client(Socket sk, int id) : perr(std::move(sk)), socket_moved(false),
                                    protocol(perr), id(id), player_number(-1),
                                    ready(false) {}



LobbyEvent Client::recive() {
    return protocol.receive_initial_event();
}

int Client::wait_for_init_game() {
    return protocol.receive_init_game();
}

void Client::notify_init_game(Map map, int size) {
    protocol.send_init_game(map, size);
}

void Client::notify_join_res() {
    protocol.success_join(player_number);
}

void Client::send_map(Map map) {
    protocol.send_create(map, player_number);
}

void Client::send_map_ids(std::vector<Map> maps) {
    protocol.send_maps(maps);
}

void Client::wait_ready() {
    protocol.receive_ready();
    ready = true;
}

void Client::set_player_number(const int number) {
    player_number = number;
}

int Client::get_id() const {
    return id;
}

int Client::get_duck_id() const {
    return player_number;
}


bool Client::is_ready() const {
    return ready;
}

Socket&& Client::release_socket() {
    return std::move(perr);
}


void Client::notify_lobby_ready() {
    protocol.send_all_clients_ready();
}


void Client::end_connection() {
    protocol.close_connection();
}
