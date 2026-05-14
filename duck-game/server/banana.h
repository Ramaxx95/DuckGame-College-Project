#ifndef BANANA_H
#define BANANA_H
#include "throweableItem.h"
#include "gametypes.h"

class Banana : public ThroweableItem {
    bool peeled;
    int peel_duration;

    std::vector<std::unique_ptr<Bullet>> throw_item(const Position& pos,
                                                const Direction& dir) override;

    public:
    explicit Banana(const ThrowConfig& config, int peel_duration);

    std::vector<std::unique_ptr<Bullet>> on_press_action(const Position& origin,
                                                         const Direction& direction) override;
    void on_release_action() override;

    std::vector<std::unique_ptr<Bullet>> update(const Position& origin,
                                                const Direction& direction) override;

    unsigned char get_type() override;

    int get_ammo() override;
};



#endif //BANANA_H
