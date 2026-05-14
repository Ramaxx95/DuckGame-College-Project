#include "rectanglecollider.h"

RectangleCollider::RectangleCollider(Position position, float length, float height,
                                     float host_height, float x_offset):
        host_position(position),
        length(length),
        height(height),
        host_height(host_height),
        x_offset(x_offset) {}

Position RectangleCollider::get_position() const { return host_position; }

float RectangleCollider::get_length() const { return length; }

float RectangleCollider::get_height() const { return height; }

void RectangleCollider::update_position(Position position) {
    host_position = {position.x + x_offset, position.y + host_height - height};
}

void RectangleCollider::changeSize(float height) { this->height = height; }
