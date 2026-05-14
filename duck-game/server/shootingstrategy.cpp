#include "shootingstrategy.h"

#include <random>

#include "bouncingBullet.h"
#include "explosiveBullet.h"
#include "normalbullet.h"

ShootingStrategy::ShootingStrategy(const ShootingContext& ctx):
        context(ctx), dispersion(context.min_dispersion), timer(0),
        can_shoot(true), has_actions(false) {}


void ShootingStrategy::reset_dispersion() { dispersion = context.min_dispersion; }

void ShootingStrategy::set_timer() { timer = context.frame_per_bullets; }

void ShootingStrategy::set_timer(int new_timer) {
    timer = new_timer;
}



void ShootingStrategy::decrease_timer() {
    if (timer > 0) {
        timer--;
    }
}

std::unique_ptr<Bullet> ShootingStrategy::make_bullet(int life_time, const Position& pos,
                                                      float speed,
                                                      const std::pair<float, float>& dir) const {
    switch (context.bullet_type) {
        case NORMAL:
            return std::make_unique<NormalBullet>(life_time, pos, speed, dir);
        case ELASTIC:
            return std::make_unique<BouncingBullet>(life_time, pos, speed, dir);
        case EXPLOSIVE:
            return std::make_unique<ExplosiveBullet>(life_time, pos, speed, dir);
        default:
            return {};
    }

}



Position ShootingStrategy::get_position_w_offset(const Direction& direction,
                                                 const Position& origin) const {
    float pos_off_y = 0;
    float pos_off_x = 0;
    switch (direction) {
        case Direction::UP:
            // TODO hacer el/los calculos correctos
            pos_off_y = origin.y;
            pos_off_x = origin.x + context.bullet_offset_x / 2;
            break;
        case Direction::LEFT:
            pos_off_y = origin.y + context.bullet_offset_y;
            pos_off_x = origin.x;
            break;
        case Direction::RIGHT:
            pos_off_y = origin.y + context.bullet_offset_y;
            pos_off_x = origin.x + context.bullet_offset_x;
            break;
        default:
            break;
    }
    return Position{pos_off_x, pos_off_y};
}

void ShootingStrategy::update_dispersion() { dispersion += context.dispersion_increment; }


int ShootingStrategy::get_bullet_life_time() const {
    if (context.min_range >= context.max_range) {
        return context.min_range;
    }

    // generador de números aleatorios
    static std::random_device rd;
    static std::mt19937 gen(rd());


    std::uniform_int_distribution<> dis(context.min_range, context.max_range);
    return dis(gen);
}

int ShootingStrategy::get_ammo() const { return this->context.ammo; }

bool ShootingStrategy::has_actions_left() const {
    return has_actions;
}
