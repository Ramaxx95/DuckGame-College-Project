#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "player.h"

#define WINDOW_W 640
#define WINDOW_H 480
#define PLAYER_SPRITE_W 32
#define PLAYER_SPRITE_H 32
#define MIN_FRAME_W 256
#define MIN_FRAME_H 192
#define FRAME_GROWTH_RATE 1.006

class Camera {
private:
    SDL2pp::Rect frame;
    SDL2pp::Renderer& renderer;

    int updateFrame(std::vector<Player*>* players);

    // Busca la posicion de aquel jugador que se encuentra mas cerca del borde izquierdo
    // y la guarda en 'x' e 'y'
    void getLowestPlayerPos(int& x, int& y, std::vector<Player*>* players);

    bool allPlayersInsideFrame(int x, int y, int w, int h, std::vector<Player*>* players);

    void expandFrame(int& frame_w, int& frame_h);

public:
    explicit Camera(SDL2pp::Renderer& renderer);
    int render(std::vector<Player*>* players);
    ~Camera();
};

#endif
