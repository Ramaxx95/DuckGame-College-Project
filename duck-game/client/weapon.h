#ifndef WEAPON_H
#define WEAPON_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "sprite.h"
#include "mediaPlayer.h"

#define WEAPON_SPRITE (WEAPON_PATH "/guns.png")
#define UNARMED_SPRITE (SPRITE_PATH "/P1/running wings.png")

#define WING_W 32
#define WING_H 32
#define WING_FRAMES 6
#define FIRST_WING_FRAMES 1
#define LAST_WING_FRAMES 5
#define WPN_W 38
#define WPN_H 32
#define WPN_FRAMES 8

class Weapon {
private:
    Sprite wpn_sprite;
    Sprite unarmed_sprite;
    Sprite* weapon;
    bool is_armed;
    bool is_flipped;
    unsigned int ammo;

public:
    explicit Weapon(SDL2pp::Renderer& renderer);
    int selectWeapon(int wpn, MediaPlayer& audio_player);
    int dropWeapon(MediaPlayer& audio_player);
    int render();
    int advanceFrame();
    int resetWing();
    int flip();
    int setPosition(int x, int y);
    int setWeaponSprite(const std::string& img_file);
    int setUnarmedSprite(const std::string& img_file);
    int setAmmo(unsigned int new_ammo, MediaPlayer& audio_player);

    // Elegir el frame del ala que este usando en este momento
    // Devuelve 0 en caso exitoso
    // Devuelve -1 si el 'frame' dado es erroneo
    int selectFrame(int frame);
    bool isArmed();
    unsigned int getAmmo();
    void aimUpward(bool do_action);
    ~Weapon();
};

#endif
