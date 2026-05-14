#include "singleshotstrat.h"

#include <algorithm>

#include "normalbullet.h"

SingleShotStrat::SingleShotStrat(const ShootingContext& ctx): ShootingStrategy(ctx) {}


std::vector<std::unique_ptr<Bullet>> SingleShotStrat::shoot(const Position& origin,
                                                            const Direction& direction) {
    std::vector<std::unique_ptr<Bullet>> bullets;
    if (context.ammo <= 0 || timer > 0 || !can_shoot) {
        return bullets;
    }
    auto bullet_dir = make_bullet_dir(direction);

    auto bullet_lifetime = get_bullet_life_time();

    auto position = get_position_w_offset(direction, origin);

    bullets.emplace_back(make_bullet(bullet_lifetime, position, context.bullet_speed, bullet_dir));

    has_actions = true;
    can_shoot = false;
    set_timer();
    context.ammo--;
    return bullets;
}


std::pair<float, float> SingleShotStrat::make_bullet_dir(const Direction& dir) {
    float final_angle = 0.0f;

    // min_dispersion de grados a radianes
    auto dispersion_angle = static_cast<float>(dispersion * M_PI / 180.0f);

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
            final_angle = M_PI_2 - dispersion_angle;  // -90° en radianes
            break;
        default:
            // Dirección no válida
            return {0.0f, 0.0f};
    }

    // obtenemos la componente "x" y la componente "y" del angulo
    float x = std::cos(final_angle);
    float y = std::sin(final_angle);

    return {x, y};
}

void SingleShotStrat::reset() {
    reset_dispersion();
    can_shoot = true;
}

std::vector<std::unique_ptr<Bullet>> SingleShotStrat::update(const Position& /*origin*/,
                                                             const Direction& /*origin*/) {
    decrease_timer();
    if (timer <= 0) {
        has_actions = false;
    }
    return {};
}
