#ifndef MIDDLEANIMATION_H
#define MIDDLEANIMATION_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL_ttf.h>
#include <vector>
#include "../common/gameData.h"

class MiddleAnimation {
    private:
        SDL2pp::Renderer& renderer;
        SDL2pp::Texture bkgr;
        TTF_Font* font;

    public:
        explicit MiddleAnimation(SDL2pp::Renderer& renderer);
        int render(std::vector<Player_Data> p_data);
        ~MiddleAnimation();
};

#endif //MIDDLEANIMATION_H
