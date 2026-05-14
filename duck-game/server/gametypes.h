#ifndef GAMETYPES_H
#define GAMETYPES_H

enum ArmorType { ARMOR_NO, ARMOR_HEAD, ARMOR_BODY, FULL_ARMOR };

enum WeaponType {
    WEAPON_NONE,
    COWBOY_PISTOL,
    MAGNUM,
    DUEL_PISTOL,
    AK47,
    SHOTGUN,
    PEW_PEW_LASERM,
    LASER_RIFLE,
    SNIPER_RIFLE,
    BANANA,
    GRANADE
};

enum Action { JUMP, PRESS_FIRE, RELEASE_FIRE, MOVE, LOOK, PICKUP, INVALID_ACTION };

enum class Direction { NONE, LEFT, RIGHT, UP, DOWN };

struct Position {
    float x = 0;
    float y = 0;
};


#endif  // GAMETYPES_H
