#ifndef SPRITE_H
#define SPRITE_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#define INIT_FRAME 0      // Por lo general, posicion estatica del Sprite
#define MAX_FRAME_PACE 1  // Cantidad de ciclos parados hasta cambiar al siguiente frame

class Sprite {
private:
    int x, y;
    int width, height;
    int max_frames;
    int current_frame;
    SDL2pp::Renderer& renderer;
    SDL2pp::Texture texture;
    SDL2pp::Rect clip;
    bool is_fliped;
    unsigned int frame_pace;  // Tiempo antes de cambiar al siguiente frame
    double angle;

public:
    // Inicializa el Sprite en una posicion dada
    // x: posicion 'x' en el mapa
    // y: posicion 'y' en el mapa
    // w: ancho del sprite
    // h: alto del sprite
    // max_frames: cantidad total de frames de animacion en la sprite sheet (el 0 es el sprite
    // estatico)
    // img_file: data path de la imagen
    // renderer: renderizador general de SDL
    Sprite(int x, int y, int w, int h, int max_frames, const std::string& img_file,
           SDL2pp::Renderer& renderer);
    int changePosition(int x, int y);
    int render();

    // first_anim: numero del primer cuadro de animacion (0, MAX_FRAMES)
    // last_anim: numero del ultimo cuadro de animacion (0, MAX_FRAMES)
    // Devuelve el frame actual
    // Devuelve -1 si NO se cambio de frame
    int advanceFrame(int first_anim, int last_anim);
    int resetFrame();

    // Cambiar a un frame especifico del Sprite
    // pos: entero entre 0 y 'max_frames'
    // Devuelve 0 en caso exitoso
    // Devuelve -1 si se mando un 'pos' erroneo
    int selectFrame(int pos);
    int flip();
    int changeTexture(const std::string& img_file);
    int getX();
    int getY();
    int rotate(double new_angle);
    ~Sprite();
};

#endif
