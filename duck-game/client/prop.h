#ifndef PROP_H
#define PROP_H

#include <memory>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "sprite.h"

// Clase abstracta que encapsula los objetos de mapa WeaponSpawn,
// PlayerBullet, Box y Explosion
class Prop {
private:
    // no hay miembro privado

public:
    std::unique_ptr<Sprite> prop_sprite;

    explicit Prop(std::unique_ptr<Sprite> sprite);
    virtual void render() = 0;
    virtual ~Prop();
};


#endif  // PROP_H
