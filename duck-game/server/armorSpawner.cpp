#include "armorSpawner.h"

ArmorSpawner::ArmorSpawner(const Position& position, int timer):
        ItemSpawner(position, timer), current_armor(generate_new_armor()) {}

ArmorType ArmorSpawner::pick_armor() {
    if (!is_up) {
        return ARMOR_NO;
    }
    is_up = false;
    timer_to_respawn = timer;
    ArmorType armor_taken = current_armor;
    current_armor = ARMOR_NO;
    return armor_taken;
}

void ArmorSpawner::down_timer() {
    if (is_up) {
        return;
    }

    if (timer_to_respawn <= 0) {
        is_up = true;
        current_armor = generate_new_armor();
    }
    timer_to_respawn--;
}

ArmorType ArmorSpawner::get_current_armor() const { return current_armor; }

ArmorType ArmorSpawner::generate_new_armor() {
    // Obtenemos un nuevo tipo aleatorio
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, ARMOR_BODY);
    return static_cast<ArmorType>(dis(gen));
}
