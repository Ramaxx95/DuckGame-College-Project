#ifndef SQUARE_COLLIDER_H
#define SQUARE_COLLIDER_H
#include "gametypes.h"


class RectangleCollider {
    Position host_position;
    float length;
    float height;
    float host_height;
    float x_offset;

public:
    explicit RectangleCollider(Position position, float length, float height, float host_height,
                               float x_offset);

    Position get_position() const;

    void update_position(Position position);

    float get_length() const;

    float get_height() const;

    // Cambia la altura del cuadro de colision
    // "height": nuevo valor de altura
    void changeSize(float height);
};


#endif  // SQUARE_COLLIDER_H
