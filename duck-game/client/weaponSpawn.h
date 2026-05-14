#ifndef WEAPON_SPAWN_H
#define WEAPON_SPAWN_H

#include <memory>
#include <utility>

#include "prop.h"

class WeaponSpawn: public Prop {
private:
    char wpn_id;

public:
    WeaponSpawn(std::unique_ptr<Sprite> wpn_sprite, char weapon_id);
    virtual void render() override;
    ~WeaponSpawn();
};


#endif  // WEAPON_SPAWN_H
