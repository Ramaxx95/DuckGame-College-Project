#include "gameProtocol.h"

#include <arpa/inet.h>

GameProtocol::GameProtocol(Socket& skt): skt(skt), was_closed(false) {}

int GameProtocol::sendAction(uint8_t action) {
    this->skt.sendall(&action, sizeof(uint8_t), &this->was_closed);
    if (this->was_closed) {
        throw std::runtime_error("El jugador fue desconectado del servidor!");
    }
    return 0;
}

int GameProtocol::receiveGameUpdate(std::vector<uint8_t>& msg) {

    /*
     *
     * Esta data es dinamica asi que dentro del mensaje enviado se tiene que
     * especificar el tamaño enviado.
     * Para guardar esta data lo mejor seria crear un struct PlayerData, y para
     * el mapa un struct MapData.
     * El mensaje evniado puede verse asi:
     *
     * +-------------+-------------+-----+-------------+-------------+---------+
     * | cnt players | PlayerData1 | ... | PlayerData4 | sizeMapData | MapData |
     * +-------------+-------------+-----+-------------+-------------+---------+
     *
     */

    uint32_t msg_size = 0;
    this->skt.recvall(&msg_size, sizeof(msg_size), &this->was_closed);
    if (this->was_closed) {
        return -1;
    }

    msg_size = ntohl(msg_size);
    if (msg_size > 0) {
        msg.resize(msg_size);
    }

    this->skt.recvall(msg.data(), msg_size, &this->was_closed);
    if (this->was_closed) {
        return -1;
    }

    return 0;
}

GameProtocol::~GameProtocol() {}
