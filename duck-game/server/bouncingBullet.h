#ifndef BOUNCINGBULLET_H
#define BOUNCINGBULLET_H
#include "bullet.h"


class BouncingBullet : public Bullet {

    public:
    BouncingBullet(int life_time, const Position& pos, float speed, const std::pair<float, float>& dir);

    void update() override;

    BulletType get_type() override;

    BulletEffect player_hit() override;

    bool on_platform_collision(const CollisionResult& result) override;
};



#endif //BOUNCINGBULLET_H
