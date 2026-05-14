#ifndef CONTINUOSSHOOTSTRATEGY_H
#define CONTINUOSSHOOTSTRATEGY_H

#include <memory>
#include <utility>
#include <vector>

#include "normalbullet.h"
#include "shootingstrategy.h"

class ContinuosShootStrategy: public ShootingStrategy {
    int bullet_counter;
    bool invert_dispersion;

public:
    explicit ContinuosShootStrategy(const ShootingContext& ctx, bool invert_dispersion);

    std::vector<std::unique_ptr<Bullet>> shoot(const Position& origin,
                                               const Direction& direction) override;

    std::pair<float, float> make_bullet_dir(const Direction& dir) override;

    void reset() override;

    std::vector<std::unique_ptr<Bullet>> update(const Position& origin,
                                                const Direction& direction) override;
};


#endif  // CONTINUOSSHOOTSTRATEGY_H
