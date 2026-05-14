#ifndef GAME_RECEIVER_H
#define GAME_RECEIVER_H

#include <vector>

#include "../common/gameData.h"
#include "../common/queue.h"
#include "../common/sockets.h"
#include "../common/thread.h"

#include "gameProtocol.h"

#define SUCCESS 0

class GameReceiver: public Thread {
private:
    Queue<Player_Data>& p_data;
    Queue<MapData>& m_data;
    Queue<GameStateAnimations>& game_state;
    GameProtocol protocol;
    bool& game_running;

    // Guarda la data contenida en 'msg' dentro de 'data'
    // Devuelve SUCCESS en caso exitoso
    // Devuelve -1 en caso fallido
    int deserializeData(std::vector<Player_Data>& data, MapData& map_data,
                        GameStateAnimations& game_data, std::vector<uint8_t>* msg);

public:
    GameReceiver(Queue<Player_Data>& player_data, Queue<MapData>& m_data,
                 Queue<GameStateAnimations>& game_state, Socket& skt, bool& game_running);
    virtual void run() override;
    ~GameReceiver();
};

#endif  // GAME_RECEIVER_H
