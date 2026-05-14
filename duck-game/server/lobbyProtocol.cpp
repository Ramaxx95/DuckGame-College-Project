#include "lobbyProtocol.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <arpa/inet.h>

const static uint8_t H01 = 0x01;  // Receive initial event
const static uint8_t H02 = 0x02;  // Receive init game
const static uint8_t H03 = 0x03;
const static uint8_t H04 = 0x04;  // Send init game
const static uint8_t H05 = 0x05;  // Receive player ready
const static uint8_t H06 = 0x06;  // Receive all clients ready

ServerLobbyProtocol::ServerLobbyProtocol(Socket& peer): skt(peer), was_closed(false) {}

void ServerLobbyProtocol::send_map(Map map) {
    std::vector<uint8_t> bytes;
    std::vector<uint8_t> map_data = map.serialize();

    uint16_t len_bytes = htons(static_cast<uint16_t>(map_data.size()));
    bytes.push_back(static_cast<uint8_t>(len_bytes));
    bytes.push_back(static_cast<uint8_t>(len_bytes >> 8));
    bytes.insert(bytes.end(), map_data.begin(), map_data.end());

    skt.sendall(bytes.data(), bytes.size() * sizeof(uint8_t), &was_closed);
    if (was_closed) {
        throw std::runtime_error("Error en el envio de mapa");
    }
}

void ServerLobbyProtocol::send_create(Map map, int duck_number) {
    uint8_t create_data = static_cast<uint8_t>(duck_number);
    skt.sendall(&create_data, sizeof(create_data), &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error en el envio de la solicitud de creacion de la partida");
    }

    send_map(map);
}

// Si hay algun error, mando -1 en el duck_number
void ServerLobbyProtocol::success_join(int duck_number) {
    uint8_t join_data = static_cast<uint8_t>(duck_number);
    skt.sendall(&join_data, sizeof(join_data), &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error en el envio de la solicitud de unirse a la partida");
    }
}

int ServerLobbyProtocol::receive_ready() {
    uint8_t ready_data;
    skt.recvall(&ready_data, sizeof(uint8_t), &was_closed);

    if (was_closed || ready_data != H05) {
        throw std::runtime_error("Error en la recepcion la recepcion de ready de un jugador");
    }

    return ready_data;
}

int ServerLobbyProtocol::receive_init_game() {
    uint16_t data = 0;
    skt.recvall(&data, sizeof(uint16_t), &was_closed);

    uint8_t header = (data >> 8) & 0xFF;
    uint8_t map_id = data & 0xFF;

    if (was_closed || header != H04) {
        throw std::runtime_error(
                "Error en la recepcion de la confirmacion de inicio de la partida");
    }

    return static_cast<int>(map_id);
}

void ServerLobbyProtocol::send_init_game(Map map, int size) {
    std::vector<uint8_t> bytes = {H03};

    uint16_t len_bytes = htons(static_cast<uint16_t>(size));
    bytes.push_back(static_cast<uint8_t>(len_bytes));
    bytes.push_back(static_cast<uint8_t>(len_bytes >> 8));

    skt.sendall(bytes.data(), bytes.size() * sizeof(uint8_t), &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error en el envio de la confirmacion de inicio de la partida");
    }

    send_map(map);
}

LobbyEvent ServerLobbyProtocol::receive_initial_event() {
    uint8_t msg;
    skt.recvall(&msg, 1, &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error al recibir el evento inicial");
    }

    if (msg == H01) {
        return LobbyEvent(true);
    }
    return LobbyEvent(false);
}

void ServerLobbyProtocol::send_maps(std::vector<Map> maps) {
    uint16_t len_bytes = htons(static_cast<uint16_t>(maps.size()));
    std::vector<uint8_t> bytes;

    bytes.push_back(static_cast<uint8_t>(len_bytes));
    bytes.push_back(static_cast<uint8_t>(len_bytes >> 8));

    for (auto& map: maps) {
        bytes.push_back(static_cast<uint8_t>(map.get_id()));
    }

    skt.sendall(bytes.data(), bytes.size() * sizeof(uint8_t), &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error en el envio de los mapas");
    }
}

void ServerLobbyProtocol::send_all_clients_ready() {
    uint8_t msg = H06;
    skt.sendall(&msg, sizeof(uint8_t), &was_closed);

    if (was_closed) {
        throw std::runtime_error(
                "Error en el envio de la confirmacion de que todos los jugadores estan listos");
    }
}

void ServerLobbyProtocol::close_connection() {
    try {
        skt.shutdown(2);
    } catch (std::exception& e) {
        std::cout << "Socket del cliente ya fue cerrado" << std::endl;
    }
    skt.close();
}
