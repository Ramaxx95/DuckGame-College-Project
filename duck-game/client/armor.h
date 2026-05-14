#ifndef ARMOR_H
#define ARMOR_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "sprite.h"

#define UNARMED_SPRITE (SPRITE_PATH "/P1/running wings.png")
#define P_ARMOR_PATH (ARMORS_PATH "/chestPlateAnim.png")

#define ARMOR_WIDTH 32
#define ARMOR_HEIGHT 32
#define ARMOR_FRAMES 6

class Armor {
private:
    Sprite arm_sprite;
    Sprite* armor;
    bool is_equipped;

public:
    explicit Armor(SDL2pp::Renderer& renderer);
    int selectArmor();
    int dropArmor();
    int render();
    int advanceFrame();
    int flip();
    int setPosition(int x, int y);
    int setArmorSprite(const std::string& img_file);

    // Elegir el frame del arma/ala que este usando en este momento
    // Devuelve 0 en caso exitoso
    // Devuelve -1 si el 'frame' dado es erroneo
    int selectFrame(int frame);
    bool isArmed();
    ~Armor();
};

#endif  // ARMOR_H
