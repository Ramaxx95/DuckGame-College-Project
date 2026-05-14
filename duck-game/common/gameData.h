#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <vector>

// Estructura que guarda la data de un jugador especifico
struct Player_Data {
    int x;
    int y;

    bool is_running;
    bool is_jumping;
    bool is_lying_down;
    bool is_facing_up;
    bool dead;

    bool head_armor_equipped;
    bool body_armor_equipped;

    unsigned char gun_equipped;
    int ammo;
    int score;
};

struct GameStateAnimations {
    bool show_middle_animation;
    bool end_game;
};

// Estructura para las armas desparramadas en el mapa
struct WeaponCache {
    int x;
    int y;
    unsigned char model;

    WeaponCache(int x, int y, unsigned char model): x(x), y(y), model(model) {}
};

// Estructura para las armaduras desparramadas en el mapa
struct ArmorCache {
    int x;
    int y;
    unsigned char type;
};

// Estructura que guarda el estado de una bala disparada
struct BulletData {
    int x;
    int y;
    // Va de 0° a 360°
    int rotation;

    BulletData(const int x, const int y, const int rotation): x(x), y(y), rotation(rotation) {}
};

// Estructura que guarda el estado de una caja del mapa
// struct Box
// {
//     int x;
//     int y;
//     bool is_smashed;
// };

// Estructura que guarda el estado de una explosion
// struct Explosion
// {

//     int x;
//     int y;
// };

// Estructura que guarda toda la data del mapa de juego
struct MapData {
    std::vector<WeaponCache> weapons;
    std::vector<ArmorCache> armors;
    std::vector<BulletData> bullets_fired;
    // std::vector<Box> boxes;
    // std::vector<Explosion> explosions;
};

#endif  // GAME_DATA_H
