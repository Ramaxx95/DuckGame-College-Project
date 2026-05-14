#include "explosiveBullet.h"


ExplosiveBullet::ExplosiveBullet(int timer, const Position& pos, float speed,
                                 const std::pair<float, float>& dir) :
                                 Bullet(timer, pos, speed, dir),
                                 timer(timer), explsion_radious(5) {}


void ExplosiveBullet::update() {
    // TODO actualizar para que caiga y no siga en linea recta (parabola)
    position.x += vector_dir.first * speed * FIXED_TIMESTEP;
    position.y += vector_dir.second * speed * FIXED_TIMESTEP;
    timer--;
    update_collider();
}


BulletType ExplosiveBullet::get_type() {
    return EXPLOSIVE;
}

bool ExplosiveBullet::on_platform_collision(const CollisionResult& result) {
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


BulletEffect ExplosiveBullet::player_hit() {
    is_live = false;
    return BulletEffect::TAKE_DAMAGE;
}

