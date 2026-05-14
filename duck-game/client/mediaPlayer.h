#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <SDL2/SDL_mixer.h>

#define INIT_BACKGROUND_SOUND SOUNDS_PATH "/init/Title.ogg"
#define LOBBY_BACKGROUND_SOUND SOUNDS_PATH "/lobby/jazzroom.ogg"
#define GAME_BACKGROUND_SOUND SOUNDS_PATH "/game/MarchOfDuck.ogg"
#define GUN_SOUND SOUNDS_PATH "/game/gun.ogg"
#define MENU_SOUND SOUNDS_PATH "/menu/menu_select.wav"
#define JUMP_SOUND SOUNDS_PATH "/game/jump.wav"
#define EQUIP_SOUND SOUNDS_PATH "/game/equip.wav"
#define REMOVE_SOUND SOUNDS_PATH "/game/disarm.wav"
#define LOAD_SOUND SOUNDS_PATH "/game/snubbyLoad.wav"
#define SHOOT_SOUND SOUNDS_PATH "/game/FunnyGunShoot.wav"

class MediaPlayer {
private:
    Mix_Music* music;
    Mix_Chunk* soundEffect;
    bool muted;

public:

    enum GAME_SOUND {
        EQUIP,
        JUMP,
        SHOOT,
        REMOVE,
        LOAD
    };

    enum WINDOW {
        INIT,
        LOBBY,
        GAME
    };

    enum WEAPON {
        GUN,
    };

    MediaPlayer();
    void backgroundSound(WINDOW window);
    void gunSound(WEAPON weapon);
    void play();
    void setMute(bool mute);
    void playMenuSounds();
    void playJumpSound();
    void playeGameSound(GAME_SOUND sound);

    ~MediaPlayer();
};

#endif // MEDIA_PLAYER_H
