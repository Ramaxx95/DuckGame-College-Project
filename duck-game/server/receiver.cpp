#include "receiver.h"

Receiver::Receiver(Socket& skt, Queue<PlayerAction>& queue_receiver, int playerid):
        perr(skt),
        queue_receiver(queue_receiver),
        running(true),
        protocol(perr),
        player_id(playerid) {}

void Receiver::run() { 
    try {
        while (running) {
            PlayerAction action = protocol.receive_action();
            action.playerId = player_id;
            queue_receiver.push(action);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    running = false;
}

bool Receiver::is_running() const { return running; }
