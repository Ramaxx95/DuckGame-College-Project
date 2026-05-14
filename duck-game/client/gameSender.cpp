#include "gameSender.h"

GameSender::GameSender(Queue<uint8_t>& p_input, Socket& skt, bool& game_running):
        p_input(p_input), protocol(GameProtocol(skt)), game_running(game_running) {}

void GameSender::run() {

    while (this->game_running) {
        try {
            uint8_t action = this->p_input.pop();
            this->protocol.sendAction(action);
        } catch (const ClosedQueue& e) {
            break;
        }
    }
}

GameSender::~GameSender() {}
