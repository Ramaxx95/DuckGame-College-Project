#ifndef MATCH_H
#define MATCH_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/gameData.h"
#include "../common/queue.h"

#include "background.h"
#include "camera.h"
#include "middleAnimation.h"
#include "player.h"
#include "playerBullet.h"
#include "userInterface.h"
#include "weaponSpawn.h"
#include "mediaPlayer.h"

class Match {
private:
    SDL2pp::SDL sdl;
    SDL2pp::Window game_window;
    SDL2pp::Renderer renderer;
    std::vector<Player*> players;
    Background bkgr;
    MiddleAnimation middleAnimation;
    Queue<GameStateAnimations>& game_state;
    Queue<Player_Data>& players_data;
    Queue<MapData>& map_data;
    UserInterface ui;
    Camera cam;
    bool& game_running;
    int player_cnt;
    std::vector<Prop*> props;
    MediaPlayer& audio_player;

    // Actualiza el estado de cada jugador en la partida
    //
    // player_data: puntero con structs PlayerData que manda el servidor
    int updatePlayers(std::vector<Player_Data>* player_data);

    int addProps();
    void clearProps();
    int render();

    // Guarda en 'data_vector' los PlayerData enviados por el servidor
    void getPlayerData(std::vector<Player_Data>& data_vector);

    void getGameState(GameStateAnimations& game_state);

public:
    Match(const std::string& game_name, const std::vector<std::vector<char>>& map,
          Queue<Player_Data>& players_data, Queue<MapData>& map_data,
          Queue<GameStateAnimations>& game_state, bool& game_running, 
          int player_cnt, MediaPlayer& audio_player);

    void run();
    void stop();
    ~Match();
};

#endif
