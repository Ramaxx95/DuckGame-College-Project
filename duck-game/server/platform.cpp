#include "platform.h"
Platform::Platform(Position pos, float length, float width):
        position(pos), collider(position, length, width, width, collider_x_offset) {}

const RectangleCollider& Platform::get_collider() const { return collider; }

const Position& Platform::get_position() const { return position; }
