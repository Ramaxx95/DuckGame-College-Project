#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/queue.h"
#include "../common/thread.h"

#define MOVE_RIGHT '\x10'
#define MOVE_LEFT '\x11'
#define LOOK_UP '\x13'
#define ACTION_JUMP '\x15'
#define ACTION_LIE_DOWN '\x16'
const static uint8_t NOT_MOVING = 0x19;

#define DROP_WEAPON '\x20'
#define PICK_WEAPON '\x21'
#define PICK_CB_PISTOL '\x22'
#define PICK_MAGNUM '\x23'
#define PICK_DUEL_PISTOL '\x24'
#define PICK_AK47 '\x25'
#define PICK_SHOTGUN '\x26'
#define PICK_PEW_LASER '\x27'
#define PICK_LASER_RIFLE '\x28'
#define PICK_SNIPER '\x29'

#define FIRE_WEAPON '\x30'
#define STOP_FIRE '\x31'

class InputManager: public Thread {
private:
    Queue<uint8_t>& input;
    bool& game_running;

public:
    InputManager(Queue<uint8_t>& input_queue, bool& game_running);
    virtual void run() override;
    ~InputManager();
};

#endif
