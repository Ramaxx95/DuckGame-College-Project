#ifndef RECEIVER_H
#define RECEIVER_H

#include "common/queue.h"
#include "common/sockets.h"
#include "common/thread.h"
#include <atomic>
#include "playeraction.h"
#include "gameProtocol.h"

class Receiver : public Thread {
    Socket& perr;
    Queue<PlayerAction>& queue_receiver;
    std::atomic<bool> running;
    GameProtocol protocol;
    int player_id;

    void push_into_queue(PlayerAction event);

public:
    Receiver(Socket& skt, Queue<PlayerAction>& queue_receiver, int playerid);

    void run() override;

    bool is_running() const;
};




#endif //RECEIVER_H
