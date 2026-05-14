#include "background.h"

Background::Background(SDL2pp::Renderer& renderer, const std::vector<std::vector<char>>& map):
        renderer(renderer),
        bkgr(SDL2pp::Texture(renderer, BKGR_PATH "/forest.png")),
        behind_bkgr(SDL2pp::Texture(this->renderer, SDL_PIXELFORMAT_BGR888,
                                    SDL_TEXTUREACCESS_STATIC, 1, 1)),
        tile(SDL2pp::Texture(renderer, BKGR_PATH "/ground_tiles.png")) {

    this->map = map;

    // Creamos un fondo completamente blanco
    unsigned char bkg[256 * 3];
    std::generate(bkg, bkg + sizeof(bkg), [&]() { return 255; });
    this->behind_bkgr.Update(SDL2pp::NullOpt, bkg, 3);
    this->behind_bkgr.SetColorMod(255, 255, 255);

    // Seteamos que la imagen de fondo sea un tanto transparente
    this->bkgr.SetBlendMode(SDL_BLENDMODE_BLEND);
    this->bkgr.SetAlphaMod(210);
}

int Background::render() {

    this->renderer.Copy(this->behind_bkgr);
    this->renderer.Copy(this->bkgr);

    for (size_t i = 0; i < this->map.size(); i++) {
        for (size_t j = 0; j < this->map[i].size(); j++) {
            if (this->map[i][j] == '#') {
                SDL2pp::Rect pos(j * TILE_WIDTH, i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
                SDL2pp::Rect tile(FLOOR, 0, TILE_WIDTH, TILE_HEIGHT);

                size_t map_limit = this->map[i].size();

                if (i > 0 && map[i - 1][j] == '#') {
                    tile = SDL2pp::Rect(ROCKS, 0, TILE_WIDTH, TILE_HEIGHT);
                } else if (j > 0 && map[i][j - 1] == '.') {
                    tile = SDL2pp::Rect(LEFT_EDGE, 0, TILE_WIDTH, TILE_HEIGHT);
                } else if ((j < map_limit - 1) && map[i][j + 1] == '.') {
                    tile = SDL2pp::Rect(RIGHT_EDGE, 0, TILE_WIDTH, TILE_HEIGHT);
                }
                this->renderer.Copy(this->tile, tile, pos);
            }
        }
    }
    return 0;
}

Background::~Background() {}
