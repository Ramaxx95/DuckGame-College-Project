#include "weapon.h"

Weapon::Weapon(SDL2pp::Renderer& renderer):
        wpn_sprite(Sprite(320, 240, WPN_W, WPN_H, WPN_FRAMES, WEAPON_SPRITE, renderer)),
        unarmed_sprite(Sprite(320, 240, WING_W, WING_H, WING_FRAMES, UNARMED_SPRITE, renderer)) {
    this->weapon = &this->unarmed_sprite;
    this->is_armed = false;
    this->is_flipped = false;
    this->ammo = 0;
}

int Weapon::selectWeapon(int wpn, MediaPlayer& audio_player) {
    if (!this->is_armed) {
        this->weapon = &this->wpn_sprite;
        this->is_armed = true;
        audio_player.playeGameSound(MediaPlayer::EQUIP);
    }
    this->weapon->selectFrame(wpn - 1);
    return 0;
}

int Weapon::dropWeapon(MediaPlayer& audio_player) {

    if (this->is_armed) {
        this->weapon = &this->unarmed_sprite;
        this->is_armed = false;
        audio_player.playeGameSound(MediaPlayer::REMOVE);
    }
    return 0;
}

int Weapon::render() {
    this->weapon->render();
    return 0;
}

int Weapon::advanceFrame() {

    this->unarmed_sprite.advanceFrame(FIRST_WING_FRAMES, LAST_WING_FRAMES);
    return 0;
}

int Weapon::resetWing() {

    this->unarmed_sprite.resetFrame();
    return 0;
}

int Weapon::flip() {

    this->unarmed_sprite.flip();
    this->wpn_sprite.flip();
    this->is_flipped = !this->is_flipped;
    return 0;
}

int Weapon::setPosition(int x, int y) {

    this->wpn_sprite.changePosition(x, y);
    this->unarmed_sprite.changePosition(x, y);
    return 0;
}

int Weapon::setWeaponSprite(const std::string& img_file) {

    this->wpn_sprite.changeTexture(img_file);
    return 0;
}

int Weapon::setUnarmedSprite(const std::string& img_file) {

    this->unarmed_sprite.changeTexture(img_file);
    return 0;
}

int Weapon::setAmmo(unsigned int new_ammo, MediaPlayer& audio_player) {
    if (this->ammo > new_ammo) {
        audio_player.playeGameSound(MediaPlayer::SHOOT);
    }
    this->ammo = new_ammo;
    return 0;
}

int Weapon::selectFrame(int frame) { return this->unarmed_sprite.selectFrame(frame); }

bool Weapon::isArmed() { return this->is_armed; }

unsigned int Weapon::getAmmo() { return this->ammo; }

void Weapon::aimUpward(bool do_action) {
    if (do_action) {
        this->wpn_sprite.rotate(270.0);
        if (this->is_flipped) {
            this->wpn_sprite.rotate(90.0);
        }
    } else {
        this->wpn_sprite.rotate(0.0);
    }
}

Weapon::~Weapon() {}
