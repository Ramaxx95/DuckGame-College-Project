#ifndef COLLISIONEVENTMANAGER_H
#define COLLISIONEVENTMANAGER_H

#include "bullet.h"
#include "collisionmanager.h"
#include "platform.h"
#include "player.h"
#include "rectanglecollider.h"

class CollisionEventManager {
    struct PushForce {
        float x = 200.0f;
        float y = 200.0f;
    };

    PushForce push_force_info;

public:
    CollisionEventManager();

    // Devuelve "true" en caso de que haya una colision con un piso
    // En cualquier otro caso, devuelve "false"
    bool on_platform_collision(Player& player, const Platform& platform,
                               const CollisionResult& collision_result);

    void out_of_map_check(Player& player, float max_x, float max_y);

    void on_platform_collision(Bullet& bullet, const CollisionResult& collision_result);

    void on_player_collision(Bullet& bullet, Player& player);
};


#endif  // COLLISIONEVENTMANAGER_H
