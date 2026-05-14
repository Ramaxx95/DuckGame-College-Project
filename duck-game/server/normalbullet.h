#ifndef NORMALBULLET_H
#define NORMALBULLET_H

#include <utility>

#include "bullet.h"


class NormalBullet: public Bullet {

public:
    NormalBullet(int life_time, Position position, float speed, std::pair<float, float> dir);

    BulletType get_type() override;

    void update() override;

    BulletEffect player_hit() override;

    bool on_platform_collision(const CollisionResult& result) override;

    ~NormalBullet() override = default;
};


#endif  // NORMALBULLET_H
