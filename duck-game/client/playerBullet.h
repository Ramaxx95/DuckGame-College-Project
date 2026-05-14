#ifndef PLAYER_BULLET_H
#define PLAYER_BULLET_H

#include <memory>
#include <utility>

#include "prop.h"

class PlayerBullet: public Prop {
private:
    unsigned int angle;

    // Si en 0 -> bala de plomo
    // Si en 1 -> bala de plasma
    unsigned int type;

public:
    PlayerBullet(std::unique_ptr<Sprite> bullet_sprite, unsigned int angle, unsigned int type);
    virtual void render() override;
    ~PlayerBullet();
};


#endif  // PLAYER_BULLET_H
