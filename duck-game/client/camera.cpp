#include "camera.h"

Camera::Camera(SDL2pp::Renderer& renderer): renderer(renderer) {
    this->frame = SDL2pp::Rect(0, 0, WINDOW_W, WINDOW_H);
}

int Camera::render(std::vector<Player*>* players) {

    SDL2pp::Surface curr_frame(0, WINDOW_W, WINDOW_H, 32, 0x00ff0000, 0x0000ff00, 0x000000ff,
                               0xff000000);
    this->renderer.ReadPixels(SDL2pp::NullOpt, SDL_PIXELFORMAT_ARGB8888, curr_frame.Get()->pixels,
                              curr_frame.Get()->pitch);
    SDL2pp::Texture frame_tex(this->renderer, curr_frame);

    updateFrame(players);

    this->renderer.Copy(frame_tex, this->frame, SDL2pp::NullOpt);
    return 0;
}

int Camera::updateFrame(std::vector<Player*>* players) {

    int new_frame_x, new_frame_y, new_frame_w, new_frame_h = 0;

    int lowest_player_pos_x, lowest_player_pos_y = 0;
    getLowestPlayerPos(lowest_player_pos_x, lowest_player_pos_y, players);

    // Seteo x
    new_frame_x = lowest_player_pos_x - PLAYER_SPRITE_W;
    if (new_frame_x < 0) {
        new_frame_x = 0;
    }

    // Seteo y
    new_frame_y = lowest_player_pos_y - PLAYER_SPRITE_H;
    if (new_frame_y < 0) {
        new_frame_y = 0;
    }

    // Recalibro a 4:3
    new_frame_w = MIN_FRAME_W;
    new_frame_h = MIN_FRAME_H;

    while (!allPlayersInsideFrame(new_frame_x, new_frame_y, new_frame_w, new_frame_h, players)) {
        expandFrame(new_frame_w, new_frame_h);
    }

    // Reubico el frame si se salio de pantalla
    if (new_frame_x + new_frame_w > WINDOW_W) {
        new_frame_x = WINDOW_W - new_frame_w;
    }
    if (new_frame_y + new_frame_h > WINDOW_H) {
        new_frame_y = WINDOW_H - new_frame_h;
    }

    this->frame.SetX(new_frame_x);
    this->frame.SetY(new_frame_y);
    this->frame.SetW(new_frame_w);
    this->frame.SetH(new_frame_h);

    return 0;
}

void Camera::getLowestPlayerPos(int& x, int& y, std::vector<Player*>* players) {

    // Agarro la posicion del jugador 1 como referencia
    if (!players->at(0)->is_dead()) {
        players->at(0)->getPosition(x, y);
    }

    // Busco la posicion minima que contiene jugadores
    for (size_t i = 1; i < players->size(); i++) {
        int pos_x, pos_y = 0;
        players->at(i)->getPosition(pos_x, pos_y);

        if (!players->at(i)->is_dead()) {
            if (pos_x < x) {
                x = pos_x;
            }
            if (pos_y < y) {
                y = pos_y;
            }
        }
    }
}

bool Camera::allPlayersInsideFrame(int x, int y, int w, int h, std::vector<Player*>* players) {

    for (size_t i = 0; i < players->size(); i++) {

        // Ignora a los jugadores muertos
        if (players->at(i)->is_dead()) {
            continue;
        }

        int pl_x, pl_y, pl_w, pl_h = 0;

        players->at(i)->getPosition(pl_x, pl_y);
        pl_w = pl_x + PLAYER_SPRITE_W * 2;
        pl_h = pl_y + PLAYER_SPRITE_H * 2;

        // Caso limite
        if (pl_w > WINDOW_W) {
            pl_w = WINDOW_W;
        }
        if (pl_h > WINDOW_H) {
            pl_h = WINDOW_H;
        }

        // Si el jugador en cuestion no esta dentro del frame -> devuelve false
        if (pl_x < x || pl_y < y || pl_w > (x + w) || pl_h > (y + h)) {
            return false;
        }
    }

    return true;
}

void Camera::expandFrame(int& frame_w, int& frame_h) {

    frame_w = frame_w * FRAME_GROWTH_RATE;
    frame_h = frame_h * FRAME_GROWTH_RATE;
}

Camera::~Camera() {}
