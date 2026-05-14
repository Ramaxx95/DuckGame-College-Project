#ifndef EXPLOSIVEBULLET_H
#define EXPLOSIVEBULLET_H
#include "bullet.h"


class ExplosiveBullet : public Bullet {
    int timer;
    int explsion_radious;

    public:
    ExplosiveBullet(int timer, const Position& pos, float speed,
                    const std::pair<float, float>& dir);

    void update() override;

    BulletType get_type() override;

    bool on_platform_collision(const CollisionResult& result) override;

    BulletEffect player_hit() override;

};



#endif //EXPLOSIVEBULLET_H
