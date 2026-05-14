#ifndef SENDER_H
#define SENDER_H

#include "common/thread.h"
#include "common/sockets.h"
#include "common/queue.h"
#include <atomic>
#include "gamestate.h"
#include "gameProtocol.h"

class Sender : public Thread {
    Socket& perr;
    Queue<GameStateMessage>& queue_send;
    std::atomic<bool> running;
    GameProtocol protocol;

public:
    Sender(Socket& sk, Queue<GameStateMessage>& queue_send);

    void run() override;

    bool is_running();
};
#endif //SENDER_H
