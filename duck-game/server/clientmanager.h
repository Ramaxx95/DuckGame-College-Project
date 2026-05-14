#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <atomic>
#include "common/sockets.h"
#include "common/thread.h"
#include "gamemanager.h"
#include "client.h"
#include "lobbyEvent.h"

class ClientManager : public Thread {
private:
    GameManager& game_manager;
    std::atomic<bool> running;
    Client client;


    void handle_creator_lobby_client();

    void handle_join_lobby_client();
public:
    ClientManager(Socket&& perr, GameManager& game_manager, int id_client);

    void run() override;
    
    void stop();

    // Indica si el thread murio.
    bool is_running();
};



#endif //CLIENT_MANAGER_H
