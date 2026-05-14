#include "playerBullet.h"

PlayerBullet::PlayerBullet(std::unique_ptr<Sprite> bullet_sprite, unsigned int angle,
                           unsigned int type):
        Prop(std::move(bullet_sprite)), angle(angle), type(type) {}

void PlayerBullet::render() {

    if (this->type == 0) {
        this->prop_sprite->selectFrame(0);
    } else {
        this->prop_sprite->selectFrame(1);
    }

    this->prop_sprite->rotate(this->angle);

    this->prop_sprite->render();
}

PlayerBullet::~PlayerBullet() {}
