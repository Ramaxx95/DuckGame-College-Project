#include "gamemap.h"

#include <algorithm>
#include <complex>
#include <memory>

GameMap::GameMap(const Map& map, float width, float height):
        map_width(width), map_height(height), map(map) {
    loadMap();
}

void GameMap::resolve_player_position(Player& player) {
    for (auto& platform: platforms) {
        CollisionResult result = collision_manager.get_collision_info(player.get_collider(),
                                                                      platform.get_collider());

        bool collision_found =
                collision_event_manager.on_platform_collision(player, platform, result);
        if (collision_found) {
            break;
        }
    }

    collision_event_manager.out_of_map_check(player, map_width, map_height);
}

void GameMap::set_players_pos(const std::list<std::unique_ptr<Player>>& players) {
    // Reseteo toda bala que todavia este viva
    for (auto& bullet: active_bullets) {
        bullet->set_bullet_dead();
    }
    for (auto& player: players) {
        player->set_position(initial_positions[player->get_id()]);
    }
}

void GameMap::loadMap() {
    std::vector<std::vector<char>> matrix = map.get_matrix();

    for (int i = 0; i < map.get_rows(); i++) {
        int j = 0;
        while (j < map.get_columns()) {
            char current_char = matrix[i][j];

            if (current_char == '#') {
                int start_x = j;

                while (j < map.get_columns() && matrix[i][j] == '#') {
                    j++;
                }

                float platform_width = static_cast<float>(j - start_x) * 16;

                Position pos;
                pos.x = static_cast<float>(start_x) * 16;
                pos.y = static_cast<float>(i) * 16;

                platforms.emplace_back(pos, platform_width, 16);

            } else if (current_char == 'W') {  // Agrego weapon spawns
                Position spawnPos;
                spawnPos.x = static_cast<float>(j) * 16;
                spawnPos.y = static_cast<float>(i) * 16;

                int timer_respawn = 100;
                spawners.emplace_back(spawnPos, timer_respawn);
            } else if (current_char == 'A') {  // Agrego armor spawns
                Position spawnPos;
                spawnPos.x = static_cast<float>(j) * 16;
                spawnPos.y = static_cast<float>(i) * 16;

                int timer_respawn = 100;
                armor_spawners.emplace_back(spawnPos, timer_respawn);
            } else if (current_char >= '1' && current_char <= '4') {
                Position playerPos;
                playerPos.x = static_cast<float>(j) * 16 - 16;
                playerPos.y = static_cast<float>(i) * 16 - 16;

                int playerNumber = current_char - '0';
                initial_positions[playerNumber] = playerPos;
            }

            j++;  // avanzar al siguiente carácter
        }
    }
}


void GameMap::add_bullets(std::vector<std::unique_ptr<Bullet>> bullets) {
    if (bullets.empty()) {
        return;
    }
    for (auto& bullet: bullets) {
        active_bullets.push_back(std::move(bullet));
    }
}

std::unique_ptr<IActionableItem> GameMap::get_weapon_from_near_spawner(const Player& player) {
    for (auto& spawner: spawners) {
        if (collision_manager.verify_collision(player.get_collider(), spawner.get_collider())) {
            return spawner.pick_item();
        }
    }
    return nullptr;
}

ArmorType GameMap::get_armor_from_near_spawner(const Player& player) {
    for (auto& spawner: armor_spawners) {
        if (collision_manager.verify_collision(player.get_collider(), spawner.get_collider())) {
            if (!player.has_armor(spawner.get_current_armor())) {
                return spawner.pick_armor();
            }
        }
    }
    return ARMOR_NO;
}


MapData GameMap::get_map_data() const {
    MapData map_data;
    for (auto& spawner: spawners) {
        // Posicion del weapon spawner
        auto [x, y] = spawner.get_position();
        map_data.weapons.emplace_back(x, y, spawner.get_char_item());
    }
    for (auto& spawner: armor_spawners) {
        // Posicion del armor spawner
        auto [x, y] = spawner.get_position();
        map_data.armors.emplace_back(x, y, static_cast<unsigned char>(spawner.get_current_armor()));
    }
    for (auto& bullet: active_bullets) {
        map_data.bullets_fired.push_back(bullet->get_data());
    }
    return map_data;
}

void GameMap::update_spawners() {
    for (auto& spawner: spawners) {
        spawner.down_timer();
    }
    for (auto& spawner: armor_spawners) {
        spawner.down_timer();
    }
}


void GameMap::remove_dead_bullets() {
    int active_bullets_idx = 0;
    for (const auto& bullet: active_bullets) {
        if (!bullet->get_life_status()) {
            auto it = active_bullets.begin();
            it += active_bullets_idx;
            active_bullets.erase(it);
            break;
        }
        active_bullets_idx++;
    }
}

void GameMap::update_map(const std::list<std::unique_ptr<Player>>& players) {

    // Prioriza choque con plataformas primero
    for (auto& bullet: active_bullets) {
        bullet->update();
        for (auto& platform: platforms) {
            auto result = collision_manager.get_collision_info(bullet->get_collider(),
                                                               platform.get_collider());
            if (result.collided) {
                collision_event_manager.on_platform_collision(*bullet, result);
            }
        }
    }

    // Borramos balas que chocaron con plataformas
    remove_dead_bullets();

    for (auto& bullet: active_bullets) {
        for (auto& player: players) {
            if (collision_manager.verify_collision(bullet->get_collider(),
                                                   player->get_collider())) {

                collision_event_manager.on_player_collision(*bullet, *player);
            }
        }
    }

    update_spawners();
}
