#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>
#include <tuple>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "player.h"

class UserInterface {
private:
    SDL2pp::SDLTTF ttf;
    SDL2pp::Font font;
    SDL2pp::Renderer& renderer;
    std::vector<std::tuple<int, int>> player_score_pos;
    std::vector<SDL_Color> player_color;

public:
    explicit UserInterface(SDL2pp::Renderer& renderer);
    int render(std::vector<Player*>* players);
    ~UserInterface();
};

#endif
