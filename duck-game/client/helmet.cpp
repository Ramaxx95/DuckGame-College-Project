#include "helmet.h"

Helmet::Helmet(SDL2pp::Renderer& renderer):
        hlm_sprite(Sprite(320, 240, HELMET_WIDTH, HELMET_HEIGHT, 1, P_HELMET_PATH_1, renderer)),
        is_equipped(false) {
    this->helmet = &this->hlm_sprite;
}

int Helmet::selectHelmet(int wpn) {
    if (!this->is_equipped) {
        this->is_equipped = true;
    }

    switch (wpn) {
        case 0:
            setHelmetSprite(P_HELMET_PATH_1);
            break;
        case 1:
            setHelmetSprite(P_HELMET_PATH_2);
            break;
        case 2:
            setHelmetSprite(P_HELMET_PATH_3);
            break;
        case 3:
            setHelmetSprite(P_HELMET_PATH_4);
            break;
        case 4:
            setHelmetSprite(P_HELMET_PATH_5);
            break;

        default:
            break;
    }
    return 0;
}

int Helmet::dropHelmet() {

    if (this->is_equipped) {
        this->is_equipped = false;
    }
    return 0;
}

int Helmet::render() {
    if (this->is_equipped) {
        this->helmet->render();
    }

    return 0;
}

int Helmet::flip() {

    this->hlm_sprite.flip();
    return 0;
}

int Helmet::setPosition(int x, int y) {

    this->hlm_sprite.changePosition(x, y);
    return 0;
}

int Helmet::setHelmetSprite(const std::string& img_file) {

    this->hlm_sprite.changeTexture(img_file);
    return 0;
}

bool Helmet::isArmed() { return this->is_equipped; }

Helmet::~Helmet() {}
