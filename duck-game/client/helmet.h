#ifndef HELMET_H
#define HELMET_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "sprite.h"

#define P_HELMET_PATH_1 (HELMETS_PATH "/helmet.png")
#define P_HELMET_PATH_2 (HELMETS_PATH "/caps.png")
#define P_HELMET_PATH_3 (HELMETS_PATH "/dicks.png")
#define P_HELMET_PATH_4 (HELMETS_PATH "/knightHelmetPickup.png")
#define P_HELMET_PATH_5 (HELMETS_PATH "/wahhs.png")

#define HELMET_WIDTH 32
#define HELMET_HEIGHT 32

class Helmet {
private:
    Sprite hlm_sprite;
    Sprite* helmet;
    bool is_equipped;

public:
    explicit Helmet(SDL2pp::Renderer& renderer);
    int selectHelmet(int helmet_id);
    int dropHelmet();
    int render();
    int flip();
    int setPosition(int x, int y);
    int setHelmetSprite(const std::string& img_file);

    bool isArmed();
    ~Helmet();
};

#endif  // HELMET_H
