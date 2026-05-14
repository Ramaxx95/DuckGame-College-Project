#ifndef GAME_PROTOCOL_H
#define GAME_PROTOCOL_H


#include <cstdint>
#include <vector>
#include "common/sockets.h"
#include "gamestate.h"
#include "playeraction.h"
#include <stdexcept>
#include <cstring>

class GameProtocol {
private:
    Socket& socket;
    bool was_closed;

    Action byte_to_action(const unsigned char& actionByte);

    Direction byte_to_direction(const unsigned char& directionByte);

public:
    explicit GameProtocol(Socket& socket);

    // enviar el estado del juego
    void send_game_state(const GameStateMessage& state);


    PlayerAction receive_action();
};


#endif // GAME_PROTOCOL_H
