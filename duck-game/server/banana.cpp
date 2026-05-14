#include "banana.h"

#include <iostream>

#include "bananaPeel.h"

Banana::Banana(const ThrowConfig& config, const int peel_duration) : ThroweableItem(config),
                                                                     peeled(false),
                                                                     peel_duration(peel_duration) {}



std::vector<std::unique_ptr<Bullet>> Banana::throw_item(const Position& pos, const Direction& dir) {
    std::vector<std::unique_ptr<Bullet>> banana_peel;
    if (!peeled) {
        return banana_peel;
    }
    auto direction = make_dir(dir);
    auto position = make_position(dir, pos);
    banana_peel.emplace_back(std::make_unique<BananaPeel>(peel_duration,position,
                                                            config.speed, direction));

    return banana_peel;
}

std::vector<std::unique_ptr<Bullet>> Banana::on_press_action(const Position& origin,
                                                             const Direction& direction) {
    return throw_item(origin, direction);
}
void Banana::on_release_action() {
    peeled = true;
}


std::vector<std::unique_ptr<Bullet>> Banana::update(const Position& /*origin*/,
                                                    const Direction& /*origin*/) {
    has_actions = false;
    return {};
}

int Banana::get_ammo() {
    return 1;
}

unsigned char Banana::get_type() {
    return WeaponType::BANANA;
}
