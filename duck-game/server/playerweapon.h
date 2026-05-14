#ifndef PLAYERWEAPON_H
#define PLAYERWEAPON_H

#include <memory>
#include <utility>
#include <vector>

#include "gametypes.h"
#include "IActionableItem.h"
#include "shootingstrategy.h"


class PlayerWeapon : public IActionableItem {
    std::unique_ptr<ShootingStrategy> strategy;
    WeaponType weapon_type;

    std::vector<std::unique_ptr<Bullet>> shoot(const Position& origin,
                                           const Direction& direction) const;

public:
    PlayerWeapon(std::unique_ptr<ShootingStrategy> strat, WeaponType weapon_type);


    std::vector<std::unique_ptr<Bullet>> on_press_action(const Position& origin,
                                                         const Direction& direction) override;

    std::vector<std::unique_ptr<Bullet>> update(const Position& origin,
                                                const Direction& direction) override;

    void on_release_action() override;

    unsigned char get_type() override;

    int get_ammo() override;

    bool has_action() override;
};


#endif  // PLAYERWEAPON_H
