#include "bouncingBullet.h"


BouncingBullet::BouncingBullet(int life_time, const Position& pos, float speed,
                                const std::pair<float, float>& dir):
                                Bullet(life_time, pos, speed, dir) {}



void BouncingBullet::update() {
    if ((int)sqrt(pow(position.x - origin.x, 2)) > 16 * life_time ||
        (int)sqrt(pow(position.y - origin.y, 2)) > 16 * life_time) {
        is_live = false;
        } else {
            position.x += vector_dir.first * speed * FIXED_TIMESTEP;
            position.y += vector_dir.second * speed * FIXED_TIMESTEP;
        }
    update_collider();
}

bool BouncingBullet::on_platform_collision(const CollisionResult& result) {
    switch (result.type) {
        case CollisionResult::TOP:
        case CollisionResult::BOTTOM:
            vector_dir.second = -vector_dir.second;
            break;
        case CollisionResult::LEFT:
        case CollisionResult::RIGHT:
            vector_dir.first = -vector_dir.first;
            break;
        default:
            return false;
    }
    return true;
}

BulletType BouncingBullet::get_type() {
    return ELASTIC;
}

BulletEffect BouncingBullet::player_hit() {
    return TAKE_DAMAGE;
}

