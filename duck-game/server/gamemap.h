#ifndef GAMEMAP_H
#define GAMEMAP_H
#include <list>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../common/gameData.h"
#include "../common/map.h"

#include "armorSpawner.h"
#include "bullet.h"
#include "collisionEventManager.h"
#include "collisionmanager.h"
#include "itemSpawner.h"
#include "platform.h"
#include "player.h"
#include "playerweapon.h"

class GameMap {
    // Vector de plataformas
    std::vector<Platform> platforms;
    // Vector de balas activas
    std::vector<std::unique_ptr<Bullet>> active_bullets;
    // Posiciones Iniciales de los jugadores
    std::unordered_map<int, Position> initial_positions;
    // Vector de Spawners de Items
    std::vector<ItemSpawner> spawners;
    // Vector de Spawners de Armors
    std::vector<ArmorSpawner> armor_spawners;
    float map_width;
    float map_height;
    Map map;
    CollisionManager collision_manager;
    CollisionEventManager collision_event_manager;


    void update_spawners();

    void loadMap();

    void remove_dead_bullets();

public:
    GameMap(const Map& map, float width, float height);

    void add_bullets(std::vector<std::unique_ptr<Bullet>> bullets);

    void set_players_pos(const std::list<std::unique_ptr<Player>>& players);

    void update_map(const std::list<std::unique_ptr<Player>>& players);

    void resolve_player_position(Player& player);

    // Devuelve los datos del mapa
    MapData get_map_data() const;

    std::unique_ptr<IActionableItem> get_weapon_from_near_spawner(const Player& player);

    ArmorType get_armor_from_near_spawner(const Player& player);
};


#endif  // GAMEMAP_H
