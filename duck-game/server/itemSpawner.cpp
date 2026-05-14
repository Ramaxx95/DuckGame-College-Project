#include "itemSpawner.h"

#include <iostream>

#include "weaponFactory.h"

ItemSpawner::ItemSpawner(const Position& position, const int timer):
        position(position),
        is_up(true),
        timer(timer),
        timer_to_respawn(0),
        collider(position, SPAWNER_WIDTH, SPAWNER_HEIGHT, SPAWNER_HEIGHT, SPAWNER_X_OFFSET),
        current_weapon(generate_new_weapon_type()) {}

void ItemSpawner::down_timer() {
    if (is_up) {
        return;
    }

    if (timer_to_respawn <= 0) {
        is_up = true;
        current_weapon = generate_new_weapon_type();
    }
    timer_to_respawn--;
}

std::unique_ptr<IActionableItem> ItemSpawner::pick_item() {
    if (!is_up) {
        return nullptr;
    }
    is_up = false;
    timer_to_respawn = timer;
    WeaponType wpn_taken = current_weapon;
    current_weapon = WEAPON_NONE;
    return WeaponFactory::create_weapon(wpn_taken);
}


const Position& ItemSpawner::get_position() const { return position; }


unsigned char ItemSpawner::get_char_item() const {
    return static_cast<unsigned char>(current_weapon);
}

const RectangleCollider& ItemSpawner::get_collider() const { return collider; }

WeaponType ItemSpawner::generate_new_weapon_type() {
    // Obtenemos un nuevo tipo aleatorio
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, SNIPER_RIFLE);
    return static_cast<WeaponType>(dis(gen));
}
