#include "player.h"

Player::Player(int x, int y, SDL2pp::Renderer& renderer, unsigned int num):
        number(num),
        body(Sprite(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, DUCK_FRAMES, DEFAULT_SPRITE, renderer)),
        weapon(Weapon(renderer)),
        armor(Armor(renderer)),
        helmet(Helmet(renderer)),
        dead(false) {

    std::string wpn_path;

    switch (num) {
        case 1:
            this->sprite_path = P1_PATH;
            wpn_path = P1_GUNS_PATH;
            break;

        case 2:
            this->sprite_path = P2_PATH;
            wpn_path = P2_GUNS_PATH;
            break;

        case 3:
            this->sprite_path = P3_PATH;
            wpn_path = P3_GUNS_PATH;
            break;

        case 4:
            this->sprite_path = P4_PATH;
            wpn_path = P4_GUNS_PATH;
            break;

        default:
            this->sprite_path = DEFAULT_SPRITE;
            wpn_path = WEAPON_PATH "/guns.png";
            break;
    }

    std::string path = this->sprite_path + "/running.png";
    this->body.changeTexture(path);

    this->helmet.setPosition(x, y);

    this->armor.setPosition(x, y);

    this->weapon.setPosition(x, y);
    this->weapon.setWeaponSprite(wpn_path + "/guns.png");
    this->weapon.setUnarmedSprite(this->sprite_path + "/running wings.png");

    this->is_running = false;
    this->is_facing_right = true;
    this->is_jumping = false;
    this->is_falling = false;
    this->is_lying_down = false;
    this->score = 0;
}

int Player::render() {

    this->body.render();
    if (!this->is_lying_down) {
        this->helmet.render();
        this->armor.render();
        this->weapon.render();
    }

    return 0;
}

int Player::changeRunningStatus(bool action) {

    this->is_running = action;
    if (this->is_running && !this->is_falling) {
        this->is_lying_down = false;
        this->body.advanceFrame(FIRST_RUNNING_FRAME, LAST_RUNNING_FRAME);
        this->armor.advanceFrame();
        this->weapon.advanceFrame();
    } else if (!this->is_jumping && !this->is_falling) {  // Si el pato se para -> resetearlo
        this->body.resetFrame();
        this->weapon.resetWing();
    }
    return 0;
}

int Player::changePosition(int x, int y) {
    this->body.changePosition(x, y);
    this->helmet.setPosition(x, y);
    this->armor.setPosition(x, y);
    this->weapon.setPosition(x, y);
    return 0;
}

int Player::moveLeft(bool action) {

    if (this->is_facing_right == action) {
        this->is_facing_right = !action;
        this->body.flip();
        this->helmet.flip();
        this->armor.flip();
        this->weapon.flip();
    }
    return 0;
}

int Player::jump(MediaPlayer& audio_player) {

    if (!this->is_jumping && !this->is_lying_down) {
        this->body.resetFrame();
        this->weapon.resetWing();
        this->body.changeTexture(this->sprite_path + "/jumping.png");
        this->weapon.setUnarmedSprite(this->sprite_path + "/jumping wings.png");
        this->is_jumping = true;
        audio_player.playJumpSound();
    }

    return 0;
}

int Player::advanceJump() {

    if (this->is_jumping) {

        int jump_frame = this->body.advanceFrame(FIRST_RUNNING_FRAME, LAST_RUNNING_FRAME);
        this->weapon.advanceFrame();
        this->armor.advanceFrame();
        bool exit_jump = jump_frame == 5;

        if (exit_jump) {
            this->is_jumping = false;
            this->body.changeTexture(this->sprite_path + "/running.png");
            this->weapon.setUnarmedSprite(this->sprite_path + "/running wings.png");
            this->body.resetFrame();
            this->weapon.resetWing();
        }
    }
    return 0;
}

int Player::pickUpWeapon(int wpn, MediaPlayer& audio_player) {

    if (wpn == 0) {
        this->weapon.dropWeapon(audio_player);
    } else {
        this->weapon.selectWeapon(wpn, audio_player);
    }

    return 0;
}

int Player::setFalling(bool is_falling) {

    if (is_falling) {
        this->body.changeTexture(this->sprite_path + "/jumping.png");
        this->body.selectFrame(4);
        if (!this->weapon.isArmed()) {
            this->weapon.setUnarmedSprite(this->sprite_path + "/jumping wings.png");
            this->weapon.selectFrame(3);
        }
        this->is_jumping = false;
        this->is_running = false;
        this->is_falling = true;
    } else if (this->is_falling) {
        this->body.changeTexture(this->sprite_path + "/running.png");
        if (!this->weapon.isArmed()) {
            this->weapon.setUnarmedSprite(this->sprite_path + "/running wings.png");
        }
        this->body.resetFrame();
        this->weapon.resetWing();
        this->is_falling = false;
    }
    return 0;
}

void Player::setPlayerIsAlive(bool is_alive) { this->dead = is_alive; }

int Player::addScore(unsigned int new_score) {
    this->score = new_score;
    return 0;
}

int Player::updateAmmoCounter(unsigned int new_ammo, MediaPlayer& audio_player) {
    return this->weapon.setAmmo(new_ammo, audio_player);
}

void Player::lieDown() {
    this->body.selectFrame(6);
    this->is_lying_down = true;
}

void Player::lookUp(bool do_action) { this->weapon.aimUpward(do_action); }

int Player::equipArmor(bool head, bool body) {

    if (head) {
        this->helmet.selectHelmet(0);
    }
    if (body) {
        this->armor.selectArmor();
    }
    return 0;
}

int Player::removeArmor(bool head, bool body) {
    if (head) {
        this->helmet.dropHelmet();
    }
    if (body) {
        this->armor.dropArmor();
    }
    return 0;
}

void Player::getPosition(int& x, int& y) {

    x = this->body.getX();
    y = this->body.getY();
}

unsigned int Player::getScore() { return this->score; }

unsigned int Player::getAmmo() { return this->weapon.getAmmo(); }

bool Player::is_dead() { return this->dead; }

Player::~Player() {}
