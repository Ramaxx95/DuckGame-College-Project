#ifndef GAME_PROTOCOL_H
#define GAME_PROTOCOL_H

#include <iostream>
#include <vector>
#include <cstdint>

#include "../common/sockets.h"

class GameProtocol {
private:
    Socket& skt;
    bool was_closed;

public:
    explicit GameProtocol(Socket& skt);
    int sendAction(uint8_t action);

    // Guarda el mensaje recibido en un vector de bytes
    // Devueve -1 en caso de haber perdido conexion con el servidor
    // Devuelve 0 en caso exitoso
    int receiveGameUpdate(std::vector<uint8_t>& msg);
    ~GameProtocol();
};

#endif  // GAME_PROTOCOL_H
