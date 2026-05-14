#include "sender.h"

Sender::Sender(Socket& sk, Queue<GameStateMessage>& queue_send)
    : perr(sk), queue_send(queue_send), running(true), protocol(perr) {}


void Sender::run() {
    try {
        while (running) {
            GameStateMessage game_state = queue_send.pop();
            protocol.send_game_state(game_state);
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    running = false;
}

bool Sender::is_running() {
    return running;
}

