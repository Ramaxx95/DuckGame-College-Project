#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define FLOOR 0
#define ROCKS 16
#define LEFT_EDGE 32
#define RIGHT_EDGE 48

class Background {
private:
    SDL2pp::Renderer& renderer;
    SDL2pp::Texture bkgr;
    SDL2pp::Texture behind_bkgr;         // Para tener un fondo blanco atras de la imagen
    std::vector<std::vector<char>> map;  // De 40x30 para Tiles de 16x16
    SDL2pp::Texture tile;

public:
    Background(SDL2pp::Renderer& renderer, const std::vector<std::vector<char>>& map);
    int render();
    ~Background();
};

#endif
