#include "sprite.h"

Sprite::Sprite(int x, int y, int w, int h, int max_frames, const std::string& img_file,
               SDL2pp::Renderer& renderer):
        x(x),
        y(y),
        width(w),
        height(h),
        max_frames(max_frames),
        renderer(renderer),
        texture(SDL2pp::Texture(renderer, img_file)) {
    this->current_frame = INIT_FRAME;
    this->clip = SDL2pp::Rect(0, 0, w, h);
    this->is_fliped = false;
    this->frame_pace = MAX_FRAME_PACE;
    this->angle = 0.0;
}

int Sprite::changePosition(int x, int y) {

    if (this->x >= 0 && this->y >= 0) {

        this->x = x;
        this->y = y;
    }

    return 0;
}

int Sprite::render() {

    this->renderer.Copy(this->texture, this->clip,
                        SDL2pp::Rect(this->x, this->y, this->width, this->height), this->angle,
                        SDL2pp::NullOpt, this->is_fliped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

    return 0;
}

int Sprite::advanceFrame(int first_anim, int last_anim) {

    if (this->frame_pace == MAX_FRAME_PACE) {
        if (this->current_frame < last_anim) {
            this->current_frame++;
        } else {
            this->current_frame = first_anim;
        }

        this->clip.SetX(this->current_frame * this->width);

        this->frame_pace = 0;
        return this->current_frame;
    }
    this->frame_pace++;

    return -1;
}

int Sprite::resetFrame() {
    this->clip.SetX(INIT_FRAME);
    this->current_frame = INIT_FRAME;
    this->frame_pace = MAX_FRAME_PACE;

    return 0;
}

int Sprite::selectFrame(int pos) {

    if (pos < 0 || pos > this->max_frames) {
        std::cerr << "[ERROR] Posicion de frame requerida es erronea. Debe proponer un numero "
                     "menor o igual a "
                  << this->max_frames << std::endl;
        return -1;
    }

    this->current_frame = pos;
    this->clip.SetX(this->current_frame * this->width);

    return 0;
}

int Sprite::flip() {

    // Alterno el valor anterior
    this->is_fliped = !this->is_fliped;
    return 0;
}

int Sprite::changeTexture(const std::string& img_file) {

    this->texture = SDL2pp::Texture(this->renderer, img_file);
    return 0;
}

int Sprite::getX() { return this->x; }

int Sprite::getY() { return this->y; }

int Sprite::rotate(double new_angle) {
    this->angle = new_angle;
    return 0;
}

Sprite::~Sprite() {}
