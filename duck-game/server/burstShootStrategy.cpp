#include "burstShootStrategy.h"

#include <algorithm>
#include <cmath>
#include <iostream>

BurstShootStrategy::BurstShootStrategy(const ShootingContext& ctx, int burst_timer):
        ShootingStrategy(ctx), bullet_counter(-1), bullets_left(0), timer_btw_burst(burst_timer) {}


std::vector<std::unique_ptr<Bullet>> BurstShootStrategy::shoot(const Position& origin,
                                                               const Direction& direction) {
    std::vector<std::unique_ptr<Bullet>> bullets;
    if (context.ammo <= 0 || bullets_left > 0 || timer > 0 || !can_shoot) {
        return bullets;
    }

    auto bullet_dir = make_bullet_dir(direction);

    auto bullet_lifetime = get_bullet_life_time();

    auto position = get_position_w_offset(direction, origin);

    bullets.emplace_back(make_bullet(bullet_lifetime, position, context.bullet_speed, bullet_dir));

    bullets_left = context.bullets_per_shot - 1;
    can_shoot = false;
    has_actions = true;
    set_timer();
    context.ammo--;

    return bullets;
}

std::pair<float, float> BurstShootStrategy::make_bullet_dir(const Direction& dir) {
    float final_angle = 0.0f;

    // min_dispersion de grados a radianes
    auto dispersion_angle = static_cast<float>(dispersion * M_PI / 180.0f * bullet_counter);

    const auto max_dispersion = static_cast<float>(context.max_dispersion * M_PI / 180.0f);
    dispersion_angle = std::clamp(dispersion_angle, -max_dispersion, max_dispersion);

    // angulo base segun la dirección
    switch (dir) {
        case Direction::LEFT:
            final_angle = M_PI + dispersion_angle;  // 180° en radianes
            break;
        case Direction::RIGHT:
            final_angle = 0.0f - dispersion_angle;
            break;
        case Direction::UP:
            final_angle = -M_PI_2 + dispersion_angle;  // 90° en radianes
            break;
        case Direction::DOWN:
            final_angle = -M_PI_2 - dispersion_angle;  // -90° en radianes
            break;
        default:
            // Dirección no válida
            return {0.0f, 0.0f};
    }


    // obtenemos la componente "x" y la componente "y" del angulo
    float x = std::cos(final_angle);
    float y = std::sin(final_angle);

    update_bullet_counter();
    return {x, y};
}

void BurstShootStrategy::update_bullet_counter() {
    if (bullet_counter == 1) {
        bullet_counter = -1;
    }
    bullet_counter++;
}

void BurstShootStrategy::reset() { can_shoot = true; }


std::vector<std::unique_ptr<Bullet>> BurstShootStrategy::update(const Position& origin,
                                                                const Direction& direction) {
    std::vector<std::unique_ptr<Bullet>> bullets;

    if (bullets_left <= 0) {
        if (timer > 0) {
            decrease_timer();
            return bullets;
        }
        has_actions = false;
        return bullets;
    }

    if (timer > 0) {
        decrease_timer();
        return bullets;
    }


    bullets_left--;

    if (bullets_left <= 0) {
        set_timer(timer_btw_burst);
    } else {
        set_timer();
    }
    context.ammo--;
    auto bullet_dir = make_bullet_dir(direction);

    auto bullet_lifetime = get_bullet_life_time();

    auto position = get_position_w_offset(direction, origin);

    bullets.emplace_back(make_bullet(bullet_lifetime, position, context.bullet_speed, bullet_dir));

    return bullets;
}
