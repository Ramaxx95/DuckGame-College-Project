#include "userInterface.h"

UserInterface::UserInterface(SDL2pp::Renderer& renderer):
        ttf(SDL2pp::SDLTTF()), font(SDL2pp::Font(FONT_PATH "/stocky.ttf", 25)), renderer(renderer) {
    this->player_score_pos = {{0, 0}, {500, 0}, {0, 430}, {500, 430}};
    this->player_color = {SDL_Color{255, 255, 255, 0},  // P1
                          SDL_Color{141, 136, 136, 0},  // P2
                          SDL_Color{242, 242, 20, 0},   // P3
                          SDL_Color{242, 20, 20, 0}};   // P4
}

int UserInterface::render(std::vector<Player*>* players) {

    for (size_t i = 0; i < players->size(); i++) {

        std::string p_score = "P" + std::to_string((int)i + 1) + ": " +
                              std::to_string(players->at(i)->getScore());
        std::string p_ammo = "Ammo: " + std::to_string(players->at(i)->getAmmo());

        SDL2pp::Texture black_sq(this->renderer, SDL_PIXELFORMAT_BGR888, SDL_TEXTUREACCESS_STATIC,
                                 150, 50);
        black_sq.SetBlendMode(SDL_BLENDMODE_BLEND);
        black_sq.SetColorAndAlphaMod(SDL_Color{0, 0, 0, 170});
        SDL2pp::Texture text_sprite_score(
                this->renderer, this->font.RenderText_Solid(p_score, this->player_color[i]));
        SDL2pp::Texture text_sprite_ammo(
                this->renderer, this->font.RenderText_Solid(p_ammo, this->player_color[i]));

        this->renderer.Copy(black_sq, SDL2pp::NullOpt,
                            SDL2pp::Rect(std::get<0>(this->player_score_pos.at(i)),
                                         std::get<1>(this->player_score_pos.at(i)),
                                         black_sq.GetWidth(), black_sq.GetHeight()));
        this->renderer.Copy(
                text_sprite_score, SDL2pp::NullOpt,
                SDL2pp::Rect(std::get<0>(this->player_score_pos.at(i)),
                             std::get<1>(this->player_score_pos.at(i)),
                             text_sprite_score.GetWidth(), text_sprite_score.GetHeight()));
        this->renderer.Copy(
                text_sprite_ammo, SDL2pp::NullOpt,
                SDL2pp::Rect(std::get<0>(this->player_score_pos.at(i)),
                             std::get<1>(this->player_score_pos.at(i)) + 25,
                             text_sprite_ammo.GetWidth(), text_sprite_ammo.GetHeight()));
    }
    return 0;
}

UserInterface::~UserInterface() {}
