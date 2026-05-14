#include "armor.h"

Armor::Armor(SDL2pp::Renderer& renderer):
        arm_sprite(
                Sprite(320, 240, ARMOR_WIDTH, ARMOR_HEIGHT, ARMOR_FRAMES, P_ARMOR_PATH, renderer)),
        is_equipped(false) {
    this->armor = &this->arm_sprite;
}

int Armor::selectArmor() {
    if (!this->is_equipped) {
        this->is_equipped = true;
    }
    return 0;
}

int Armor::dropArmor() {
    if (this->is_equipped) {
        this->is_equipped = false;
    }
    return 0;
}

int Armor::render() {
    if (this->is_equipped) {
        this->armor->render();
    }
    return 0;
}

int Armor::advanceFrame() {
    this->arm_sprite.advanceFrame(1, 5);
    return 0;
}

int Armor::flip() {

    this->arm_sprite.flip();
    return 0;
}

int Armor::setPosition(int x, int y) {

    this->arm_sprite.changePosition(x, y);
    return 0;
}

int Armor::setArmorSprite(const std::string& img_file) {

    this->arm_sprite.changeTexture(img_file);
    return 0;
}

int Armor::selectFrame(int frame) {

    if (frame < 0 || frame >= ARMOR_FRAMES) {
        return -1;
    }
    this->armor->selectFrame(frame);
    return 0;
}

bool Armor::isArmed() { return this->is_equipped; }

Armor::~Armor() {}
