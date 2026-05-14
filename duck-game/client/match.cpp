#include "match.h"

Match::Match(const std::string& game_name, const std::vector<std::vector<char>>& map,
             Queue<Player_Data>& players_data, Queue<MapData>& map_data,
             Queue<GameStateAnimations>& game_state, bool& game_running, int player_cnt,
             MediaPlayer& audio_player):

        sdl(SDL2pp::SDL(SDL_INIT_VIDEO)),
        game_window(SDL2pp::Window(game_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,
                                   480, 0)),
        renderer(SDL2pp::Renderer(game_window, -1, SDL_RENDERER_ACCELERATED)),
        bkgr(Background(this->renderer, map)),
        middleAnimation(MiddleAnimation(this->renderer)),
        game_state(game_state),
        players_data(players_data),
        map_data(map_data),
        ui(UserInterface(this->renderer)),
        cam(this->renderer),
        game_running(game_running),
        player_cnt(player_cnt),
        audio_player(audio_player) {}

int Match::updatePlayers(std::vector<Player_Data>* player_data) {

    int p_current_x = 0;
    int p_current_y = 0;

    for (size_t i = 0; i < this->players.size(); i++) {

        this->players[i]->getPosition(p_current_x, p_current_y);

        if (i < player_data->size()) {

            this->players[i]->setPlayerIsAlive(player_data->at(i).dead);

            if (player_data->at(i).is_facing_up) {
                this->players[i]->lookUp(true);
            } else {
                this->players[i]->lookUp(false);
            }
            if (player_data->at(i).x < p_current_x) {
                this->players[i]->moveLeft(true);
            } else if (player_data->at(i).x > p_current_x) {
                this->players[i]->moveLeft(false);
            }

            if (p_current_y < player_data->at(i).y) {
                this->players[i]->setFalling(true);
            } else if (p_current_y > player_data->at(i).y) {
                this->players[i]->advanceJump();
            } else {
                this->players[i]->setFalling(false);
            }
            this->players[i]->changePosition(player_data->at(i).x, player_data->at(i).y);
            this->players[i]->changeRunningStatus(player_data->at(i).is_running);

            if (player_data->at(i).is_jumping) {
                this->players[i]->jump(audio_player);
            }
            if (player_data->at(i).is_lying_down || player_data->at(i).dead) {
                this->players[i]->lieDown();
            }

            if (player_data->at(i).head_armor_equipped || player_data->at(i).body_armor_equipped) {
                this->players[i]->equipArmor(player_data->at(i).head_armor_equipped,
                                             player_data->at(i).body_armor_equipped);
            }
            if (!player_data->at(i).head_armor_equipped ||
                !player_data->at(i).body_armor_equipped) {
                this->players[i]->removeArmor(!player_data->at(i).head_armor_equipped,
                                              !player_data->at(i).body_armor_equipped);
            }

            this->players[i]->pickUpWeapon((int)player_data->at(i).gun_equipped, audio_player);
            this->players[i]->updateAmmoCounter(player_data->at(i).ammo, audio_player);

            this->players[i]->addScore(player_data->at(i).score);
        }
    }
    return 0;
}

int Match::addProps() {

    MapData m_data;
    if (this->map_data.try_pop(m_data)) {
        // Limpio la lista de props del ciclo anterior
        clearProps();

        // WeaponSpawns
        for (size_t i = 0; i < m_data.weapons.size(); i++) {
            if (m_data.weapons[i].model != 0x00) {
                std::string wpn_path = WEAPON_PATH "/weapon_cache.png";
                std::unique_ptr<Sprite> wpn_sprite(
                        std::make_unique<Sprite>(m_data.weapons[i].x, m_data.weapons[i].y, 16, 16,
                                                 8, wpn_path, this->renderer));
                WeaponSpawn* wpn = new WeaponSpawn(std::move(wpn_sprite), m_data.weapons[i].model);
                this->props.push_back(wpn);
            }
        }

        // ArmorSpawns
        for (size_t i = 0; i < m_data.armors.size(); i++) {
            if (m_data.armors[i].type != 0x00) {
                std::string armor_path = WEAPON_PATH "/armor.png";
                std::unique_ptr<Sprite> armor_sprite(
                        std::make_unique<Sprite>(m_data.armors[i].x, m_data.armors[i].y, 16, 16, 2,
                                                 armor_path, this->renderer));
                WeaponSpawn* armor =
                        new WeaponSpawn(std::move(armor_sprite), m_data.armors[i].type);
                this->props.push_back(armor);
            }
        }

        // PlayerBullets
        for (size_t j = 0; j < m_data.bullets_fired.size(); j++) {
            std::string bullet_path = WEAPON_PATH "/bullet.png";
            std::unique_ptr<Sprite> wpn_sprite(
                    std::make_unique<Sprite>(m_data.bullets_fired[j].x, m_data.bullets_fired[j].y,
                                             8, 3, 2, bullet_path, this->renderer));

            PlayerBullet* bullet =
                    new PlayerBullet(std::move(wpn_sprite), m_data.bullets_fired[j].rotation, 0);
            this->props.push_back(bullet);
        }
    }
    return 0;
}

void Match::clearProps() {
    for (size_t i = 0; i < this->props.size(); i++) {
        delete (this->props[i]);
    }
    this->props.clear();
}

int Match::render() {

    this->bkgr.render();
    for (size_t i = 0; i < this->players.size(); i++) {
        this->players[i]->render();
    }

    for (size_t j = 0; j < this->props.size(); j++) {
        this->props[j]->render();
    }

    this->cam.render(&this->players);
    this->ui.render(&this->players);

    return 0;
}

void Match::getPlayerData(std::vector<Player_Data>& data_vector) {

    for (size_t i = 0; i < this->players.size(); i++) {

        Player_Data p_data;
        bool data_retrieved = this->players_data.try_pop(p_data);
        if (data_retrieved) {
            data_vector.push_back(p_data);
        }
    }
}

void Match::getGameState(GameStateAnimations& game_state) {
    GameStateAnimations g_state;
    bool data_retrieved = this->game_state.try_pop(g_state);
    if (data_retrieved) {
        game_state = g_state;
    }
}

void Match::run() {
    try {

        // agregamos los jugadores de la partida actual
        for (int i = 0; i < this->player_cnt; i++) {
            this->players.push_back(new Player(320, 240 + i * 32, this->renderer, i + 1));
        }

        audio_player.backgroundSound(MediaPlayer::GAME);

        GameStateAnimations g_state;
        while (this->game_running) {

            // Sacamos de la Queue la data de cada jugador recibida por el servidor
            // y la actualizamos para este cliente
            std::vector<Player_Data> p_data;
            getPlayerData(p_data);

            getGameState(g_state);

            addProps();

            if (g_state.end_game) {
                std::cout << "Match terminado" << std::endl;
                this->game_running = false;
                break;
            }

            if (g_state.show_middle_animation) {
                middleAnimation.render(p_data);
                continue;
            }

            if (p_data.size() > 0) {
                updatePlayers(&p_data);
            }

            // Limpio la pantalla
            renderer.Clear();

            // Renderizo jugadores y fondo
            this->render();

            // Muestro los renderizados en pantalla
            renderer.Present();
        }
        std::cout << "Match terminado" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Match::stop() { this->game_running = false; }

Match::~Match() {
    for (size_t i = 0; i < this->players.size(); i++) {
        delete (this->players[i]);
    }
    for (size_t j = 0; j < this->props.size(); j++) {
        delete (this->props[j]);
    }
}
