#include "middleAnimation.h"
#include <string>
#include <SDL2pp/Surface.hh>

MiddleAnimation::MiddleAnimation(SDL2pp::Renderer& renderer) : 
    renderer(renderer),
    bkgr(SDL2pp::Texture(renderer, MIDDLE_PATH "/scoreBoard.png")),
    font(nullptr) 
{
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        // throw std::runtime_error(std::string("TTF_Init failed: ") + TTF_GetError());
    }

    // Cargar la fuente
    font = TTF_OpenFont(FONTS_PATH "/Doto_Rounded-Black.ttf", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
        // throw std::runtime_error(
        //     std::string("TTF_OpenFont failed: ") + TTF_GetError());
    }
}

int MiddleAnimation::render(std::vector<Player_Data> p_data) {
    if (p_data.size() == 0) {
        return 0;
    }

    renderer.Clear();
    renderer.Copy(bkgr, SDL2pp::NullOpt, SDL2pp::NullOpt);

    SDL_Color textColor = {255, 255, 255, 255}; // Blanco

    // Posición y tamaño del texto inicial
    int x = 50;
    int y = 50;
    int lineHeight = 50; // Altura del texto + espacio entre líneas

    bool has_a_winner = false;
    std::string winner = "No hay ganador";

    for (int i = 0; i < (int)p_data.size(); ++i) {
        // Crear superficie para cada mensaje
        std::string scoreStr = std::to_string(p_data[i].score);
        if (p_data[i].score >= 10) {
            has_a_winner = true;
            winner = "Player " + std::to_string(i + 1) + " wins!";
        }
        std::string playerStr = "Player " + std::to_string(i + 1) + " " + scoreStr;
        SDL_Surface* msgSurface = TTF_RenderText_Solid(font, playerStr.c_str(), textColor);

        if (!msgSurface) {
            std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
            continue;
        }

        SDL2pp::Texture msgTexture(renderer, SDL2pp::Surface(msgSurface));

        SDL_Rect msgRect = {x, y + (lineHeight * (i + 1)), 200, lineHeight};
        renderer.Copy(msgTexture, SDL2pp::NullOpt, msgRect);
    }

    if (has_a_winner) {
        SDL_Surface* msgSurface = TTF_RenderText_Solid(font, winner.c_str(), textColor);

        if (!msgSurface) {
            std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
            return 1;
        }

        SDL2pp::Texture msgTexture(renderer, SDL2pp::Surface(msgSurface));

        int msgY = y + (lineHeight * (static_cast<int>(p_data.size()) + 1));
            SDL_Rect msgRect = {x, msgY, 200, lineHeight};
        renderer.Copy(msgTexture, SDL2pp::NullOpt, msgRect);
    }

    renderer.Present();
    return 0;
}

MiddleAnimation::~MiddleAnimation() {
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
}
