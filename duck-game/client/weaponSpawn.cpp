#include "weaponSpawn.h"

WeaponSpawn::WeaponSpawn(std::unique_ptr<Sprite> wpn_sprite, char weapon_id):
        Prop(std::move(wpn_sprite)), wpn_id(weapon_id) {}

void WeaponSpawn::render() {

    if (this->wpn_id != 0x00) {
        this->prop_sprite->selectFrame((int)this->wpn_id - 1);
        this->prop_sprite->render();
    }
}

WeaponSpawn::~WeaponSpawn() {}
