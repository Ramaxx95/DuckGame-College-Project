#include "bananaPeel.h"

BananaPeel::BananaPeel(int life_time, const Position& pos, float speed,
                        const std::pair<float, float>& dir):
                        Bullet(life_time, pos, speed, dir) {}

void BananaPeel::update() {
    position.x += vector_dir.first * speed * FIXED_TIMESTEP;
    position.y += vector_dir.second * speed * FIXED_TIMESTEP;
    update_collider();
}


BulletType BananaPeel::get_type() {
    return  SPECIAL;
}

bool BananaPeel::on_platform_collision(const CollisionResult& result) {
    switch (result.type) {
        case CollisionResult::TOP:
            speed = 0;
        break;
        default:
            vector_dir.second = -vector_dir.second;
        vector_dir.first = 0;
        break;
    }
    return true;
}

BulletEffect BananaPeel::player_hit() {
    is_live = false;
    return BulletEffect::PUSH_PLAYER;
}

