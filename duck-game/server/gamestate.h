#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <cstdint>

#include <array>
#include <vector>

#include "../common/gameData.h"


struct GameStateMessage {
    uint8_t player_count;
    std::array<Player_Data, 4> players;  // Máximo 4 jugadores

    uint32_t map_data_size;  // Tamaño en bytes de MapData
    MapData map_data;
    uint8_t show_middle_animation;
    uint8_t show_end_game;
};

class GameState {
public:
    static uint32_t calculate_map_data_size(const MapData& map_data);

    static std::vector<uint8_t> serialize_game_state(const GameStateMessage& msg);

    GameState() = delete;  // Clase estática, no se puede instanciar
};


#endif  // GAMESTATE_H
