#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "armor.h"
#include "helmet.h"
#include "sprite.h"
#include "weapon.h"
#include "mediaPlayer.h"

#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32
#define DUCK_FRAMES 7
#define FIRST_RUNNING_FRAME 1
#define LAST_RUNNING_FRAME 5

#define DEFAULT_SPRITE (SPRITE_PATH "/Grey Duck.png")
#define P1_PATH (SPRITE_PATH "/P1")
#define P2_PATH (SPRITE_PATH "/P2")
#define P3_PATH (SPRITE_PATH "/P3")
#define P4_PATH (SPRITE_PATH "/P4")

#define P1_GUNS_PATH (WEAPON_PATH "/P1")
#define P2_GUNS_PATH (WEAPON_PATH "/P2")
#define P3_GUNS_PATH (WEAPON_PATH "/P3")
#define P4_GUNS_PATH (WEAPON_PATH "/P4")

class Player {
private:
    unsigned int number;
    std::string sprite_path;
    Sprite body;
    Weapon weapon;
    bool is_running;
    bool is_facing_right;
    bool is_jumping;
    bool is_falling;
    Armor armor;
    Helmet helmet;
    bool dead;
    bool is_lying_down;
    unsigned int score;

public:
    Player(int x, int y, SDL2pp::Renderer& renderer, unsigned int num);
    int render();
    int changeRunningStatus(bool action);
    int changePosition(int x, int y);

    // Comando para indicar a las Sprites del Player que orientacion
    // deben tomar
    //
    // action: true  -> voltea las Sprites para que miren hacia la izquierda
    //         false -> voltea las Sprites para que miren hacia la derecha
    int moveLeft(bool action);
    int jump(MediaPlayer& audio_player);
    int advanceJump();
    int pickUpWeapon(int wpn, MediaPlayer& audio_player);
    int setFalling(bool is_falling);
    void setPlayerIsAlive(bool is_alive);
    int addScore(unsigned int new_score);
    int updateAmmoCounter(unsigned int new_ammo, MediaPlayer& audio_player);
    void lieDown();
    void lookUp(bool do_action);

    int equipArmor(bool head, bool body);
    int removeArmor(bool head, bool body);
    bool is_dead();

    // Guarda la posicion actual del Player en las variables 'x' e 'y'
    void getPosition(int& x, int& y);
    unsigned int getScore();
    unsigned int getAmmo();
    ~Player();
};

#endif
