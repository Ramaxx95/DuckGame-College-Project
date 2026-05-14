#ifndef ITEMSPAWNER_H
#define ITEMSPAWNER_H
#include <memory>
#include <random>

#include "gametypes.h"
#include "playerweapon.h"

#define SPAWNER_WIDTH 16
#define SPAWNER_HEIGHT 16
#define SPAWNER_X_OFFSET 0

class ItemSpawner {
protected:
    Position position;
    bool is_up;
    int timer;
    int timer_to_respawn;
    RectangleCollider collider;
    WeaponType current_weapon;

    WeaponType generate_new_weapon_type();

public:
    ItemSpawner(const Position& position, int timer);

    std::unique_ptr<IActionableItem> virtual pick_item();

    // Resta 1 el tiempo del spawner
    void virtual down_timer();

    // Retorna la posicion del spawner
    const Position& get_position() const;

    // Retorna la representacion en unsigned char del item que maneja
    unsigned char get_char_item() const;

    const RectangleCollider& get_collider() const;

    virtual ~ItemSpawner() = default;
};


#endif  // ITEMSPAWNER_H
