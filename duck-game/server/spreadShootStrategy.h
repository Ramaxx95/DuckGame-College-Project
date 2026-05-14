#ifndef SPREADSHOOTSTRATEGY_H
#define SPREADSHOOTSTRATEGY_H

#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "normalbullet.h"
#include "shootingstrategy.h"

class SpreadShootStrategy: public ShootingStrategy {
    int bullet_counter;

    void update_buller_counter();

    std::pair<float, float> make_bullet_dir(const Direction& dir) override;

public:
    explicit SpreadShootStrategy(const ShootingContext& ctx);

    std::vector<std::unique_ptr<Bullet>> shoot(const Position& origin,
                                               const Direction& direction) override;

    void reset() override;

    std::vector<std::unique_ptr<Bullet>> update(const Position& origin,
                                                const Direction& direction) override;
};


#endif  // SPREADSHOOTSTRATEGY_H
