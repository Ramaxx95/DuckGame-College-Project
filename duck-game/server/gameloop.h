#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <atomic>
#include <memory>
#include <utility>
#include <vector>

#include "../common/map.h"
#include "common/queue.h"
#include "common/thread.h"

#include "gamemap.h"
#include "gamestate.h"
#include "playeraction.h"
#include "playercontroller.h"

class GameLoop: public Thread {
    Queue<PlayerAction>& q_game_loop;
    std::atomic<bool> running;
    PlayerController& players;
    Map map;
    GameMap game_map;
    bool show_middle_animation;
    bool show_end_game;
    int iterations;

    void proccess_action(const PlayerAction& action);

public:
    explicit GameLoop(Queue<PlayerAction>& q_game_loop, PlayerController& players,
                      const Map& map_r);

    void run() override;

    void handle_event(const PlayerAction& action);

    bool is_running();

    void end_game();

    GameStateMessage generate_game_state() const;

    bool handle_winners_logic();
};


#endif  // GAMELOOP_H
