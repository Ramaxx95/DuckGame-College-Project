#include "granade.h"

Granade::Granade(int timer, const ThrowConfig& config):
    ThroweableItem(config), is_active(false), timer(timer) {}



std::vector<std::unique_ptr<Bullet>> Granade::throw_item(const Position& pos, const Direction& dir) {
    std::vector<std::unique_ptr<Bullet>> explosive_bullet;
    if (!is_active) {
        has_actions = true;
        return explosive_bullet;
    }
    has_actions = false;
    auto direction = make_dir(dir);
    auto position = make_position(dir, pos);
    explosive_bullet.emplace_back(std::make_unique<ExplosiveBullet>(timer, position,
                                                                     config.speed,
                                                                     direction));
    return explosive_bullet;
}



std::vector<std::unique_ptr<Bullet>> Granade::update_state(const Position& pos, const Direction& dir) {
    std::vector<std::unique_ptr<Bullet>> explosive_bullet;
    if (!is_active || !has_actions) {
        return explosive_bullet;
    }
    if (timer <= 0) {
        explosive_bullet.emplace_back(std::make_unique<ExplosiveBullet>(0, pos,0,
                                                                          make_dir(dir)));
    }
    timer--;
    return explosive_bullet;
}

std::vector<std::unique_ptr<Bullet>> Granade::on_press_action(const Position& origin,
                                                              const Direction& direction) {
    return throw_item(origin, direction);
}

void Granade::on_release_action() {
    is_active = true;
}

std::vector<std::unique_ptr<Bullet>> Granade::update(const Position& origin,
                                                     const Direction& direction) {
    return update_state(origin, direction);
}

unsigned char Granade::get_type() {
    return WeaponType::GRANADE;
}


int Granade::get_ammo() {
    return 1;
}

