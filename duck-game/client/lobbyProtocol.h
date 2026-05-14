#ifndef CLIENT_LOBBY_PROTOCOL_H
#define CLIENT_LOBBY_PROTOCOL_H

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "../common/map.h"
#include "../common/sockets.h"

template <typename T>
void print_bytes(const T& data, const std::string& type, const std::string& function) {
    // Obtener un puntero a los bytes del objeto
    const uint8_t* byteData = reinterpret_cast<const uint8_t*>(&data);
    size_t size = sizeof(data);

    std::cout << "[DEBUG] Bytes " << type << " " << function << ": ";
    for (size_t i = 0; i < size; ++i) {
        // Imprimir cada byte en formato hexadecimal
        std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex
                  << static_cast<int>(byteData[i]) << " ";
    }
    std::cout << std::dec << std::endl;  // Volver a formato decimal
}

class LobbyProtocol {
private:
    Socket& skt;
    bool was_closed;

    void receive_player_id(int& id);
    Map receive_map();

public:
    explicit LobbyProtocol(Socket& peer);
    void request_join();
    void send_ready();
    void init_game(int map_id);
    void send_create();
    void reveive_join_success(int& duck_number);
    Map receive_create_success(int& duck_number);
    std::vector<int> receive_map_ids();
    void receive_all_clients_ready();
    Map receive_init_game(int& size);
};

#endif  // CLIENT_LOBBY_PROTOCOL_H
