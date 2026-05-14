#ifndef PLATFORM_H
#define PLATFORM_H
#include "gametypes.h"
#include "rectanglecollider.h"

const int collider_x_offset = 0;

class Platform {
    Position position;
    RectangleCollider collider;

public:
    explicit Platform(Position pos, float length, float width);

    const RectangleCollider& get_collider() const;

    const Position& get_position() const;
};


#endif  // PLATFORM_H
