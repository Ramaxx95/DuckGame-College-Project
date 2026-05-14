#include "gameReceiver.h"

#include <cstring>

GameReceiver::GameReceiver(Queue<Player_Data>& player_data, Queue<MapData>& m_data,
                           Queue<GameStateAnimations>& game_state, Socket& skt, bool& game_running):
        p_data(player_data),
        m_data(m_data),
        game_state(game_state),
        protocol(GameProtocol(skt)),
        game_running(game_running) {}

void GameReceiver::run() {

    while (this->game_running) {
        std::vector<Player_Data> data;
        std::vector<uint8_t> received_msg;
        GameStateAnimations game_data;
        MapData m_data;

        this->protocol.receiveGameUpdate(received_msg);

        if (received_msg.size() > 0) {
            if (this->deserializeData(data, m_data, game_data, &received_msg) == SUCCESS) {
                for (size_t i = 0; i < data.size(); i++) {
                    this->p_data.push(data[i]);
                }
                this->m_data.push(m_data);
                this->game_state.push(game_data);
            }
        }
    }
}

int GameReceiver::deserializeData(std::vector<Player_Data>& data, MapData& map_data,
                                  GameStateAnimations& game_data, std::vector<uint8_t>* msg) {

    //  Cantidad de jugadores
    int player_cnt = (int)msg->at(0);

    uint8_t* ptr = msg->data() + 1;

    // Escribir datos de jugadores
    for (int i = 0; i < player_cnt; i++) {
        Player_Data player;
        int x;
        int y;
        uint8_t running;
        uint8_t jumping;
        uint8_t lying_down;
        uint8_t facing_up;
        unsigned char weapon;
        uint8_t head_armor;
        uint8_t body_armor;
        int ammo;
        uint8_t dead;
        uint8_t match_wins;

        memcpy(&x, ptr, sizeof(x));
        ptr += sizeof(x);
        memcpy(&y, ptr, sizeof(y));
        ptr += sizeof(y);
        memcpy(&running, ptr, sizeof(running));
        ptr += sizeof(running);
        memcpy(&jumping, ptr, sizeof(jumping));
        ptr += sizeof(jumping);
        memcpy(&lying_down, ptr, sizeof(lying_down));
        ptr += sizeof(lying_down);
        memcpy(&facing_up, ptr, sizeof(facing_up));
        ptr += sizeof(facing_up);
        memcpy(&weapon, ptr, sizeof(weapon));
        ptr += sizeof(weapon);
        memcpy(&head_armor, ptr, sizeof(head_armor));
        ptr += sizeof(head_armor);
        memcpy(&body_armor, ptr, sizeof(body_armor));
        ptr += sizeof(body_armor);
        memcpy(&ammo, ptr, sizeof(ammo));
        ptr += sizeof(ammo);
        memcpy(&dead, ptr, sizeof(dead));
        ptr += sizeof(dead);
        memcpy(&match_wins, ptr, sizeof(match_wins));
        ptr += sizeof(match_wins);

        player.x = x;
        player.y = y;
        player.is_running = (bool)running;
        player.is_jumping = (bool)jumping;
        player.is_lying_down = (bool)lying_down;
        player.is_facing_up = (bool)facing_up;
        player.gun_equipped = weapon;
        player.head_armor_equipped = (bool)head_armor;
        player.body_armor_equipped = (bool)body_armor;
        player.ammo = ammo;
        player.dead = (bool)dead;
        player.score = (int)match_wins;

        std::cout << "[DEBUG] Player" << i + 1 << ": X=" << player.x << " Y=" << player.y
                  << " up=" << (int)player.is_facing_up << " down=" << (int)player.is_lying_down
                  << " running=" << (int)player.is_running << " jumping=" << (int)player.is_jumping
                  << " gun=" << (int)player.gun_equipped << " ammo=" << player.ammo
                  << " h_armor=" << (int)player.head_armor_equipped
                  << " b_armor=" << (int)player.body_armor_equipped << " dead=" << (int)player.dead
                  << " score=" << player.score << std::endl;

        data.push_back(player);
    }

    // Informacion del mapa

    // WeaponSpawns
    size_t wpn_size;
    memcpy(&wpn_size, ptr, sizeof(wpn_size));
    ptr += sizeof(wpn_size);
    for (size_t i = 0; i < wpn_size; i++) {
        int x, y;
        unsigned char model;
        memcpy(&x, ptr, sizeof(x));
        ptr += sizeof(x);
        memcpy(&y, ptr, sizeof(y));
        ptr += sizeof(y);
        memcpy(&model, ptr, sizeof(model));
        ptr += sizeof(model);

        WeaponCache wpn(x, y, model);
        map_data.weapons.push_back(wpn);
    }

    // ArmorSpawns
    size_t armor_size;
    memcpy(&armor_size, ptr, sizeof(armor_size));
    ptr += sizeof(armor_size);
    for (size_t i = 0; i < armor_size; i++) {
        int x, y;
        unsigned char type;
        memcpy(&x, ptr, sizeof(x));
        ptr += sizeof(x);
        memcpy(&y, ptr, sizeof(y));
        ptr += sizeof(y);
        memcpy(&type, ptr, sizeof(type));
        ptr += sizeof(type);

        ArmorCache armor = {x, y, type};
        map_data.armors.push_back(armor);
    }

    // PlayerBullets
    size_t bullet_size;
    memcpy(&bullet_size, ptr, sizeof(bullet_size));
    ptr += sizeof(bullet_size);
    for (size_t i = 0; i < bullet_size; i++) {
        int x, y, rotation;

        memcpy(&x, ptr, sizeof(x));
        ptr += sizeof(x);
        memcpy(&y, ptr, sizeof(y));
        ptr += sizeof(y);
        memcpy(&rotation, ptr, sizeof(rotation));
        ptr += sizeof(rotation);

        BulletData bullet(x, y, rotation);
        map_data.bullets_fired.push_back(bullet);

        std::cout << "[DEBUG] Bullet" << i + 1 << ": X=" << bullet.x << " Y=" << bullet.y
                  << " rotation=" << bullet.rotation << std::endl;
    }

    // Informacion middle/final de partida
    uint8_t show_middle_animation, show_end_game;

    memcpy(&show_middle_animation, ptr, sizeof(show_middle_animation));
    ptr += sizeof(show_middle_animation);

    memcpy(&show_end_game, ptr, sizeof(show_end_game));
    ptr += sizeof(show_end_game);

    std::cout << "[DEBUG] show_middle_animation = " << (int)show_middle_animation
              << ", show_end_game = " << (int)show_end_game << std::endl;

    game_data = GameStateAnimations{(bool)show_middle_animation, (bool)show_end_game};

    return SUCCESS;
}

GameReceiver::~GameReceiver() {}
