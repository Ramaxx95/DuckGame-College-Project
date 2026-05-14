#ifndef BULLET_H
#define BULLET_H
#include <cmath>  // Para std::atan2 y M_PI
#include <utility>

#include <common/gameData.h>

#include "collisionmanager.h"
#include "gametypes.h"
#include "rectanglecollider.h"

enum BulletType {
    NORMAL,
    EXPLOSIVE,
    ELASTIC,
    SPECIAL,
};

enum BulletEffect {
    TAKE_DAMAGE,
    PUSH_PLAYER
};

class Bullet {
protected:
    const float FIXED_TIMESTEP = 1.0f / 30.0f;
    int life_time;
    Position position;
    Position origin;
    float speed;
    std::pair<float, float> vector_dir;
    RectangleCollider collider;
    bool is_live;

    void update_collider();

    Bullet(int life_time, const Position& pos, float speed, const std::pair<float, float>& dir);

public:
    virtual void update() = 0;

    virtual ~Bullet() = default;

    virtual BulletType get_type() = 0;

    virtual BulletEffect player_hit() = 0;

    virtual bool on_platform_collision(const CollisionResult& result) = 0;

    BulletData get_data() const;

    bool get_life_status() const;

    void set_bullet_dead();

    RectangleCollider get_collider() const;
};


#endif  // BULLET_H
