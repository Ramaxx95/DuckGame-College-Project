#ifndef GAME_SENDER_H
#define GAME_SENDER_H

#include "../common/queue.h"
#include "../common/sockets.h"
#include "../common/thread.h"

#include "gameProtocol.h"

class GameSender: public Thread {
private:
    Queue<uint8_t>& p_input;
    GameProtocol protocol;
    bool& game_running;

public:
    GameSender(Queue<uint8_t>& p_input, Socket& skt, bool& game_running);
    virtual void run() override;
    ~GameSender();
};

#endif
