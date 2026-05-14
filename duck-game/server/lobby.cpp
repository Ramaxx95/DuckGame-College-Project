#include "lobby.h"

Lobby::Lobby(const int max_lobby_size, const int host_id)
    : max_size(max_lobby_size), host_id(host_id), closed(false) {
    clients.reserve(max_size);
}

bool Lobby::add_to_lobby(Client* client) {
    clients.push_back(client);
    client->set_player_number(get_length());
    return true;
}


int Lobby::get_length() const {
    return static_cast<int>(clients.size());
}

bool Lobby::is_full() const {
    return get_length() >= max_size;
}

const std::vector<Client*>& Lobby::get_lobby_and_close() {
    closed = true;
    return clients;
}

int Lobby::get_host_id() const {
    return host_id;
}

bool Lobby::is_closed() const {
    return closed;
}

bool Lobby::is_ready() const {
    int ready_count = 0;
    for (auto client : clients) {
        if (client->is_ready()) {
            ready_count++;
        }
    }
    return ready_count == get_length();
}


void Lobby::wait_for_lobby_ready() {
    std::unique_lock<std::mutex> lock(mtx);
    is_lobby_ready.wait(lock, [this] {
        if ((get_length() != 1 && is_ready()) || closed) {
            return true;
        }
        return false;
    });
}

void Lobby::notify_lobby_ready() {
    std::unique_lock<std::mutex> lock(mtx);
    is_lobby_ready.notify_all();
}


void Lobby::close() {
    closed = true;
    // Destrabamos los posibles threads que quedaron bloqueados
    is_lobby_ready.notify_all();
}