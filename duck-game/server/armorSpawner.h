#ifndef ARMOR_SPAWNER_H
#define ARMOR_SPAWNER_H

#include "gametypes.h"
#include "itemSpawner.h"

class ArmorSpawner: public ItemSpawner {

    ArmorType current_armor;

    ArmorType generate_new_armor();

public:
    ArmorSpawner(const Position& position, int timer);

    ArmorType pick_armor();

    // Resta 1 el tiempo del spawner
    void down_timer() override;

    // Retorna el tipo de arma actualmente en el spawner
    ArmorType get_current_armor() const;
};


#endif  // ARMOR_SPAWNER_H
