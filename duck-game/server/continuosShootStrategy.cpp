#include "continuosShootStrategy.h"

#include <algorithm>
#include <cmath>
#include <iostream>

ContinuosShootStrategy::ContinuosShootStrategy(const ShootingContext& ctx, bool invert_dispersion):
        ShootingStrategy(ctx), bullet_counter(-1), invert_dispersion(invert_dispersion) {}


std::vector<std::unique_ptr<Bullet>> ContinuosShootStrategy::shoot(const Position& origin,
                                                                   const Direction& direction) {
    std::vector<std::unique_ptr<Bullet>> bullets;
    if (context.ammo <= 0 || timer > 0) {
        return bullets;
    }

    set_timer();
    has_actions = true;
    auto bullet_dir = make_bullet_dir(direction);

    auto bullet_lifetime = get_bullet_life_time();

    auto position = get_position_w_offset(direction, origin);

    bullets.emplace_back(make_bullet(bullet_lifetime, position, context.bullet_speed, bullet_dir));

    context.ammo--;
    return bullets;
}

std::pair<float, float> ContinuosShootStrategy::make_bullet_dir(const Direction& dir) {
    float final_angle = 0.0f;

    // min_dispersion de grados a radianes
    auto dispersion_angle = static_cast<float>(dispersion * M_PI / 180.0f * bullet_counter);

    const auto max_dispersion = static_cast<float>(context.max_dispersion * M_PI / 180.0f);
    dispersion_angle = std::clamp(dispersion_angle, -max_dispersion, max_dispersion);

    // angulo segun la dirección
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

    if (invert_dispersion) {
        bullet_counter *= -1;
    }

    return {x, y};
}


void ContinuosShootStrategy::reset() {
    timer = 0;
    has_actions = false;
    reset_dispersion();
}


std::vector<std::unique_ptr<Bullet>> ContinuosShootStrategy::update(const Position& origin,
                                                                    const Direction& direction) {
    decrease_timer();
    update_dispersion();
    return shoot(origin, direction);
}
