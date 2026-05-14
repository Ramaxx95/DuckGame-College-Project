#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "rectanglecollider.h"
#include "gametypes.h"

struct CollisionResult {
    bool collided;
    enum CollisionType { NONE, TOP, BOTTOM, LEFT, RIGHT } type;
};

class CollisionManager {

    public:
    CollisionManager();

    CollisionResult get_collision_info(const RectangleCollider& player_collider,
                                        const RectangleCollider& platform_collider);

    // Devuelve true si el objeto1 colisiono con el objeto2
    bool verify_collision(const RectangleCollider& object1, const RectangleCollider& object2);
};



#endif //COLLISIONMANAGER_H
