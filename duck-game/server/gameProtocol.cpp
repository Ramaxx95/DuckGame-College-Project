#include "gameProtocol.h"

#include <iostream>

#include <netinet/in.h>

#include "lobbyProtocol.h"


namespace ActionCodes {
const unsigned char JUMP = 0x15;
const unsigned char PRES_SHOOT = 0x30;
const unsigned char STOP_SHOOT = 0x31;
const unsigned char MOVE_LEFT = 0x11;
const unsigned char MOVE_RIGHT = 0x10;
const unsigned char LOOK_UP = 0x13;
const unsigned char LOOK_DOWN = 0x16;
const unsigned char PICKUP = 0x21;
}  // namespace ActionCodes

GameProtocol::GameProtocol(Socket& socket): socket(socket), was_closed(false) {}


void GameProtocol::send_game_state(const GameStateMessage& state) {

    // serializar el estado del juego
    std::vector<uint8_t> send_buffer = GameState::serialize_game_state(state);

    // Obtener el tamaño del mensaje serializado
    uint32_t serialized_size = send_buffer.size();
    uint32_t bytes_len = htonl(serialized_size);

    // Enviar el tamaño del mensaje primero
    socket.sendall(&bytes_len, sizeof(bytes_len), &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error al enviar el tamaño del mensaje");
    }

    // Enviar el mensaje completo
    socket.sendall(send_buffer.data(), serialized_size, &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error al enviar el mensaje");
    }
}


PlayerAction GameProtocol::receive_action() {
    uint8_t action_byte;
    socket.recvall(&action_byte, 1, &was_closed);

    if (was_closed) {
        throw std::runtime_error("Error al recibir la acción del jugador");
    }

    PlayerAction player_action(byte_to_action(action_byte), byte_to_direction(action_byte), 0);

    return player_action;
}


Action GameProtocol::byte_to_action(const unsigned char& actionByte) {
    switch (actionByte) {
        case ActionCodes::JUMP:
            return Action::JUMP;
        case ActionCodes::PRES_SHOOT:
            return Action::PRESS_FIRE;
        case ActionCodes::STOP_SHOOT:
            return Action::RELEASE_FIRE;
        case ActionCodes::MOVE_LEFT:
        case ActionCodes::MOVE_RIGHT:
            return Action::MOVE;
        case ActionCodes::LOOK_UP:
        case ActionCodes::LOOK_DOWN:
            return Action::LOOK;
        case ActionCodes::PICKUP:
            return Action::PICKUP;
        default:
            return Action::INVALID_ACTION;
    }
}


Direction GameProtocol::byte_to_direction(const unsigned char& directionByte) {
    switch (directionByte) {
        case ActionCodes::MOVE_LEFT:
            return Direction::LEFT;

        case ActionCodes::MOVE_RIGHT:
            return Direction::RIGHT;

        case ActionCodes::LOOK_UP:
            return Direction::UP;

        case ActionCodes::LOOK_DOWN:
            return Direction::DOWN;

        default:
            return Direction::NONE;
    }
}
