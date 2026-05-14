#ifndef SINGLESHOTSTRAT_H
#define SINGLESHOTSTRAT_H
#include <memory>
#include <utility>
#include <vector>

#include "shootingstrategy.h"


class SingleShotStrat: public ShootingStrategy {

    public:
    explicit SingleShotStrat(const ShootingContext& ctx);

    std::vector<std::unique_ptr<Bullet>> shoot(const Position& origin, const Direction& direction) override;

    std::pair<float, float> make_bullet_dir(const Direction& dir) override;

    void reset() override;

    std::vector<std::unique_ptr<Bullet>> update(const Position& origin,
                                                const Direction& direction) override;
};


#endif  // SINGLESHOTSTRAT_H
