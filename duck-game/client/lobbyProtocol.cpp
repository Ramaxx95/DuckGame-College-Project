#include "lobbyProtocol.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <arpa/inet.h>

const static uint8_t H01 = 0x01;  // Receive initial event
const static uint8_t H02 = 0x02;  // Receive init game
const static uint8_t H03 = 0x03;  // Send init game
const static uint8_t H04 = 0x04;  // Receive initial event
const static uint8_t H05 = 0x05;
const static uint8_t H06 = 0x06;

LobbyProtocol::LobbyProtocol(Socket& peer): skt(peer), was_closed(false) {}

Map LobbyProtocol::receive_create_success(int& duck_number) {
    uint8_t msg = 0;
    skt.recvall(&msg, sizeof(msg), &was_closed);

    print_bytes(msg, "recibidos", "receive_create_success");

    if (was_closed) {
        throw std::runtime_error("Error al recibir la solicitud de unirse a la partida");
    }

    duck_number = static_cast<int>(msg);
    return receive_map();
}

void LobbyProtocol::send_create() {
    uint8_t msg = H01;
    skt.sendall(&msg, 1, &was_closed);

    print_bytes(msg, "enviados", "send_create");

    if (was_closed) {
        throw std::runtime_error("Error al enviar la solicitud de creacion de la partida");
    }
}

void LobbyProtocol::request_join() {
    uint8_t msg = H02;
    skt.sendall(&msg, 1, &was_closed);

    print_bytes(msg, "enviados", "request_join");

    if (was_closed) {
        throw std::runtime_error("Error en la recepcion de la solicitud de unirse a la partida");
    }
}

void LobbyProtocol::send_ready() {
    uint8_t msg = H05;
    skt.sendall(&msg, sizeof(msg), &was_closed);

    print_bytes(msg, "enviados", "send_ready");

    if (was_closed) {
        throw std::runtime_error("Error en el envio de la confirmacion de inicio de la partida");
    }
}

void LobbyProtocol::init_game(int map_id) {
    uint16_t data = (static_cast<uint16_t>(H04) << 8) | static_cast<uint8_t>(map_id);
    skt.sendall(&data, sizeof(data), &was_closed);

    print_bytes(data, "enviados", "init_game");

    if (was_closed) {
        throw std::runtime_error("Error en el envio de la confirmacion de inicio de la partida");
    }
}

Map LobbyProtocol::receive_map() {
    uint16_t len = 0;
    skt.recvall(&len, sizeof(len), &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error al recibir el mapa");
    }

    len = ntohs(len);
    std::vector<uint8_t> map_data(len);
    skt.recvall(map_data.data(), len, &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error al recibir el mapa");
    }

    return Map::deserialize(map_data);
}

void LobbyProtocol::reveive_join_success(int& duck_number) {
    uint8_t msg = 0;
    skt.recvall(&msg, sizeof(msg), &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error al recibir la solicitud de unirse a la partida");
    }

    duck_number = static_cast<int>(msg);
}

std::vector<int> LobbyProtocol::receive_map_ids() {
    uint16_t len = 0;
    skt.recvall(&len, sizeof(len), &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error al recibir los ids de los mapas");
    }

    len = ntohs(len);
    std::vector<uint8_t> bytes(len);
    skt.recvall(bytes.data(), len * sizeof(uint8_t), &was_closed);
    if (was_closed) {
        throw std::runtime_error("Error al recibir los ids de los mapas");
    }

    std::vector<int> map_ids;
    for (int i = 0; i < len; i++) {
        map_ids.push_back(static_cast<int>(bytes[i]));
    }

    return map_ids;
}

void LobbyProtocol::receive_all_clients_ready() {
    uint8_t msg;
    skt.recvall(&msg, sizeof(msg), &was_closed);

    if (was_closed || msg != H06) {
        throw std::runtime_error("Error al recibir la confirmacion de inicio de la partida");
    }
}

Map LobbyProtocol::receive_init_game(int& size) {
    uint8_t header = 0;
    skt.recvall(&header, sizeof(uint8_t), &was_closed);

    if (was_closed || header != H03) {
        throw std::runtime_error(
                "Error en la recepcion de la confirmacion de inicio de la partida");
    }

    uint16_t len = 0;
    skt.recvall(&len, sizeof(len), &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error al recibir los ids de los mapas");
    }

    len = ntohs(len);
    size = static_cast<int>(len);
    return receive_map();
}
