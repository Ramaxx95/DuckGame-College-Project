#include "gamestate.h"

#include <cstring>
#include <iostream>


uint32_t GameState::calculate_map_data_size(const MapData& map_data) {
    uint32_t map_data_size = 0;

    // |wpn_size|armor_size|bullet_size|
    map_data_size = sizeof(size_t) + map_data.weapons.size() * sizeof(WeaponCache) +
                    sizeof(size_t) + map_data.armors.size() * sizeof(ArmorCache) + sizeof(size_t) +
                    map_data.bullets_fired.size() * sizeof(BulletData);
    return map_data_size;
}


std::vector<uint8_t> GameState::serialize_game_state(const GameStateMessage& msg) {
    // Calcular el tamaño requerido
    uint32_t required_size = sizeof(uint8_t) +  // playerCount
                             msg.player_count * sizeof(Player_Data) + msg.map_data_size;

    required_size += sizeof(uint8_t) * 2;  // show_middle_animation, show_end_game

    // Crear un buffer del tamaño requerido
    std::vector<uint8_t> buffer(required_size);
    uint8_t* ptr = buffer.data();  // Obtener un puntero al buffer

    // Escribir número de jugadores
    *ptr = msg.player_count;
    ptr += sizeof(uint8_t);

    // Escribir datos de jugadores
    for (int i = 0; i < msg.player_count; i++) {

        int x = msg.players[i].x;
        int y = msg.players[i].y;
        uint8_t running = msg.players[i].is_running;
        uint8_t jumping = msg.players[i].is_jumping;
        uint8_t lying_down = msg.players[i].is_lying_down;
        uint8_t facing_up = msg.players[i].is_facing_up;
        unsigned char weapon = msg.players[i].gun_equipped;
        uint8_t head_armor = msg.players[i].head_armor_equipped;
        uint8_t body_armor = msg.players[i].body_armor_equipped;
        int ammo = msg.players[i].ammo;
        uint8_t dead = msg.players[i].dead;
        uint8_t score = msg.players[i].score;

        memcpy(ptr, &x, sizeof(x));
        ptr += sizeof(x);
        memcpy(ptr, &y, sizeof(y));
        ptr += sizeof(y);
        memcpy(ptr, &running, sizeof(running));
        ptr += sizeof(running);
        memcpy(ptr, &jumping, sizeof(jumping));
        ptr += sizeof(jumping);
        memcpy(ptr, &lying_down, sizeof(lying_down));
        ptr += sizeof(lying_down);
        memcpy(ptr, &facing_up, sizeof(facing_up));
        ptr += sizeof(facing_up);
        memcpy(ptr, &weapon, sizeof(weapon));
        ptr += sizeof(weapon);
        memcpy(ptr, &head_armor, sizeof(head_armor));
        ptr += sizeof(head_armor);
        memcpy(ptr, &body_armor, sizeof(body_armor));
        ptr += sizeof(body_armor);
        memcpy(ptr, &ammo, sizeof(ammo));
        ptr += sizeof(ammo);
        memcpy(ptr, &dead, sizeof(dead));
        ptr += sizeof(dead);
        memcpy(ptr, &score, sizeof(score));
        ptr += sizeof(score);
    }

    // Guradamos data de WeaponSpawns
    size_t wpn_size = msg.map_data.weapons.size();
    memcpy(ptr, &wpn_size, sizeof(wpn_size));
    ptr += sizeof(wpn_size);
    for (size_t i = 0; i < wpn_size; i++) {
        int x = msg.map_data.weapons[i].x;
        int y = msg.map_data.weapons[i].y;
        unsigned char model = msg.map_data.weapons[i].model;

        memcpy(ptr, &x, sizeof(x));
        ptr += sizeof(x);
        memcpy(ptr, &y, sizeof(y));
        ptr += sizeof(y);
        memcpy(ptr, &model, sizeof(model));
        ptr += sizeof(model);
    }

    // Guradamos data de ArmorSpawns
    size_t armor_size = msg.map_data.armors.size();
    memcpy(ptr, &armor_size, sizeof(armor_size));
    ptr += sizeof(armor_size);
    for (size_t i = 0; i < armor_size; i++) {
        int x = msg.map_data.armors[i].x;
        int y = msg.map_data.armors[i].y;
        unsigned char type = msg.map_data.armors[i].type;

        memcpy(ptr, &x, sizeof(x));
        ptr += sizeof(x);
        memcpy(ptr, &y, sizeof(y));
        ptr += sizeof(y);
        memcpy(ptr, &type, sizeof(type));
        ptr += sizeof(type);
    }

    // Guradamos data de Bullets
    size_t bullet_size = msg.map_data.bullets_fired.size();
    memcpy(ptr, &bullet_size, sizeof(bullet_size));
    ptr += sizeof(bullet_size);
    for (size_t i = 0; i < bullet_size; i++) {
        int x = msg.map_data.bullets_fired[i].x;
        int y = msg.map_data.bullets_fired[i].y;
        int rotation = msg.map_data.bullets_fired[i].rotation;

        memcpy(ptr, &x, sizeof(x));
        ptr += sizeof(x);
        memcpy(ptr, &y, sizeof(y));
        ptr += sizeof(y);
        memcpy(ptr, &rotation, sizeof(rotation));
        ptr += sizeof(rotation);
    }

    // Guradamos data de Boxes

    // Guradamos data de Explosions

    // Escribir show_middle_animation
    memcpy(ptr, &msg.show_middle_animation, sizeof(uint8_t));
    ptr += sizeof(uint8_t);

    // Escribir show_end_game
    memcpy(ptr, &msg.show_end_game, sizeof(uint8_t));
    ptr += sizeof(uint8_t);

    return buffer;  // Devolver el buffer serializado
}
