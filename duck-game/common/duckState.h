#ifndef DUCK_STATE_H
#define DUCK_STATE_H

#include "move.h"
#include <vector>
#include <cstdint>

class DuckState {
    private:
        int duck_id;
        bool is_alive;
        Move move;

    public:
        DuckState(int duck_id, Move move);
        std::vector<uint8_t> serialize();
};

#endif // DUCK_STATE_H
