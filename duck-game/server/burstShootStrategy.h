#ifndef BURSTSHOOTSTATEGY_H
#define BURSTSHOOTSTATEGY_H

#include <memory>
#include <utility>
#include <vector>

#include "normalbullet.h"
#include "shootingstrategy.h"


class BurstShootStrategy: public ShootingStrategy {
    int bullet_counter;
    int bullets_left;
    int timer_btw_burst;

    void update_bullet_counter();


public:
    explicit BurstShootStrategy(const ShootingContext& ctx, int burst_timer);

    std::vector<std::unique_ptr<Bullet>> shoot(const Position& origin,
                                               const Direction& direction) override;

    std::pair<float, float> make_bullet_dir(const Direction& dir) override;

    void reset() override;

    std::vector<std::unique_ptr<Bullet>> update(const Position& origin,
                                                const Direction& direction) override;
};


#endif  // BURSTSHOOTSTATEGY_H
