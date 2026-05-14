#ifndef BANANAPEEL_H
#define BANANAPEEL_H
#include "bullet.h"


class BananaPeel : public Bullet {

    public:
    BananaPeel(int life_time, const Position& pos, float speed,
                const std::pair<float, float>& dir);

    void update() override;

    BulletType get_type() override;

    bool on_platform_collision(const CollisionResult& result) override;

    BulletEffect player_hit() override;
};



#endif //BANANAPEEL_H
