#ifndef IACTIONABLEITEM_H
#define IACTIONABLEITEM_H
#include <memory>
#include <vector>

#include "bullet.h"


class IActionableItem {
public:
    virtual ~IActionableItem() = default;

    // Se llama cuando el jugador presiona la tecla
    virtual std::vector<std::unique_ptr<Bullet>> on_press_action(const Position& origin,
                                                  const Direction& direction) = 0;

    // Se llama cuando el jugador suelta la tecla
    virtual void on_release_action() = 0;

    // Se llama siempre que el jugador no suelte la tecla
    virtual std::vector<std::unique_ptr<Bullet>> update(const Position& origin,
                                                 const Direction& direction) = 0;

    virtual unsigned char get_type() = 0;

    virtual int get_ammo() = 0;

    virtual bool has_action() = 0;
};


#endif //IACTIONABLEITEM_H
