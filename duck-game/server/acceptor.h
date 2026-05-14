#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "gamemanager.h"
#include "clientmanager.h"
#include "common/sockets.h"
#include <vector>
#include "common/thread.h"
#include <atomic>

class Acceptor : public Thread {
private:
    Socket sk;
    GameManager& lobby_manager;
    std::vector<std::unique_ptr<ClientManager>> managers;
    std::atomic<bool> running;
    int id_counter;

public:
    Acceptor(const char* hostname, GameManager& lobby_manager);

    void stop();

    void run() override;

    ~Acceptor() override;

private:
    // Elimina threads muertos del contenedor de managers.
    void clean_finished_managers();
};



#endif //ACCEPTOR_H
