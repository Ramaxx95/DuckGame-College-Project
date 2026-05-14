#ifndef WEAPONFACTORY_H
#define WEAPONFACTORY_H

#include <memory>

#include "gametypes.h"
#include "playerweapon.h"
#include "IActionableItem.h"

class WeaponFactory {

public:
    WeaponFactory() = delete;

    // Devuelve un arma del tipo pedido
    static std::unique_ptr<IActionableItem> create_weapon(WeaponType weapon);

    // Devuelve un arma aleatoria
    static std::unique_ptr<IActionableItem> create_weapon();
};


#endif  // WEAPONFACTORY_H
