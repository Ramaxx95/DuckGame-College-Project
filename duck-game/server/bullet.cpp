#include "bullet.h"

#include <bits/ranges_base.h>

Bullet::Bullet(int life_time, const Position& pos, float speed, const std::pair<float, float>& dir):
        life_time(life_time),
        position(pos),
        origin(pos),
        speed(speed),
        vector_dir(dir),
        collider(pos, 1, 2, 0, 0),
        is_live(true) {}

void Bullet::update_collider() { collider.update_position(position); }


BulletData Bullet::get_data() const {
    const int x = static_cast<int>(position.x);
    const int y = static_cast<int>(position.y);

    // calculamos el angulo
    const float angle_radians = std::atan2(vector_dir.second, vector_dir.first);

    // convertir a grados
    float angle_degrees = angle_radians * static_cast<float>(180.0f / M_PI);

    // rango de [0°, 360°]
    if (angle_degrees < 0) {
        angle_degrees += 360.0f;
    }
    return BulletData{x, y, static_cast<int>(angle_degrees)};
}

RectangleCollider Bullet::get_collider() const { return collider; }

bool Bullet::get_life_status() const { return is_live; }

void Bullet::set_bullet_dead() { is_live = false; }
