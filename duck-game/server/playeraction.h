#ifndef PLAYERACTION_H
#define PLAYERACTION_H
#include "gametypes.h"

struct PlayerAction {
    Action action;
    Direction direction;
    int playerId;

    PlayerAction() : action(Action::INVALID_ACTION), direction(Direction::NONE), playerId(0) {};

    PlayerAction(const Action action, const Direction direction, const int playerId) : action(action), direction(direction), playerId(playerId) {}
};

#endif //PLAYERACTION_H
