#ifndef GRANADE_H
#define GRANADE_H

#include "throweableItem.h"
#include "explosiveBullet.h"
#include "gametypes.h"

class Granade : public ThroweableItem {
    bool is_active;
    int  timer;

    std::vector<std::unique_ptr<Bullet>> throw_item(const Position& pos,
                                                const Direction& dir) override;


    std::vector<std::unique_ptr<Bullet>> update_state(const Position& pos,
                                                               const Direction& dir);

    public:
    Granade(int timer, const ThrowConfig& config);

    std::vector<std::unique_ptr<Bullet>> on_press_action(const Position& origin,
                                                         const Direction& direction) override;
    void on_release_action() override;

    std::vector<std::unique_ptr<Bullet>> update(const Position& origin,
                                                const Direction& direction) override;
    unsigned char get_type() override;

    int get_ammo() override;
};



#endif //GRANADE_H
