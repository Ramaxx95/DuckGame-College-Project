#include "normalbullet.h"

#include <iostream>

NormalBullet::NormalBullet(const int life_time, const Position position, const float speed,
                           std::pair<float, float> dir):
        Bullet(life_time, position, speed, dir) {}


BulletType NormalBullet::get_type() { return NORMAL; }

void NormalBullet::update() {
    if ((int)sqrt(pow(position.x - origin.x, 2)) > 16 * life_time ||
        (int)sqrt(pow(position.y - origin.y, 2)) > 16 * life_time) {
        is_live = false;
    } else {
        position.x += vector_dir.first * speed * FIXED_TIMESTEP;
        position.y += vector_dir.second * speed * FIXED_TIMESTEP;
    }
    update_collider();
}

bool NormalBullet::on_platform_collision(const CollisionResult& /*result*/) {
    is_live = false;
    return true;
}

BulletEffect NormalBullet::player_hit() {
    is_live = false;
    return BulletEffect::TAKE_DAMAGE;
}

