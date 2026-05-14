#include "playerweapon.h"

PlayerWeapon::PlayerWeapon(std::unique_ptr<ShootingStrategy> strat,
                           WeaponType weapon_type):
        strategy(std::move(strat)), weapon_type(weapon_type) {}


std::vector<std::unique_ptr<Bullet>> PlayerWeapon::shoot(const Position& origin,
                                                         const Direction& direction) const {
    if (strategy) {
        return strategy->shoot(origin, direction);
    }
    return {};
}

std::vector<std::unique_ptr<Bullet>> PlayerWeapon::on_press_action(const Position& origin,
                                                                   const Direction& direction) {
    return shoot(origin, direction);

}

std::vector<std::unique_ptr<Bullet>> PlayerWeapon::update(const Position& origin,
                                                          const Direction& direction) {
    if (strategy) {
        return strategy->update(origin, direction);
    }

    return {};
}


void PlayerWeapon::on_release_action() {
    if (strategy) {
        strategy->reset();
    }
}


int PlayerWeapon::get_ammo() {
    return strategy->get_ammo();
}

unsigned char PlayerWeapon::get_type() {
    return static_cast<unsigned char>(weapon_type);;
}

bool PlayerWeapon::has_action() {
    if (strategy) {
        return strategy->has_actions_left();
    }
    return false;
}

