#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include <cstdint>

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Move {
    private:
        int x;
        int y;
        bool is_jumping;
        bool is_shooting;
        bool is_running;
        Direction direction;

    public:
        std::vector<uint8_t> serialize();
};

#endif // MOVE_H
