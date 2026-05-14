#include "mediaPlayer.h"

#include <iostream>
#include <SDL2/SDL.h>

MediaPlayer::MediaPlayer(): muted(false) {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        std::cerr << "Error de inicialización de SDL: " << SDL_GetError() << std::endl;
    }

    if (Mix_Init(MIX_INIT_OGG) == 0) {
        std::cerr << "Error de inicialización de SDL_mixer: " << Mix_GetError() << std::endl;
        SDL_Quit();
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        std::cerr << "Error de apertura de audio: " << Mix_GetError() << std::endl;
        SDL_Quit();
    }

    music = Mix_LoadMUS(INIT_BACKGROUND_SOUND); //default
    if (!music) {
        std::cerr << "Error al cargar el archivo OGG: " << Mix_GetError() << std::endl;
    }
}

void MediaPlayer::backgroundSound(WINDOW window) {
    if (muted) return;

    Mix_FreeMusic(music);
    switch (window) {
    case INIT:
        music = Mix_LoadMUS(INIT_BACKGROUND_SOUND);
        break;
    case LOBBY:
        music = Mix_LoadMUS(LOBBY_BACKGROUND_SOUND);
        break;
    case GAME:
        music = Mix_LoadMUS(GAME_BACKGROUND_SOUND);
        break;
    default:
        break;
    }
    play();
}

void MediaPlayer::play(){ 
    Mix_PlayMusic(music, -1);
}

void MediaPlayer::playMenuSounds() {
    soundEffect = Mix_LoadWAV(MENU_SOUND);

    if (!soundEffect) {
        std::cerr << "Error al cargar música de fondo: " << Mix_GetError() << std::endl;
    }

    if (Mix_PlayChannel(-1, soundEffect, 0) == -1) {
        std::cerr << "Error al reproducir sonido: " << Mix_GetError() << std::endl;
    }
}

void MediaPlayer::playJumpSound() {
    soundEffect = Mix_LoadWAV(JUMP_SOUND);

    if (!soundEffect) {
        std::cerr << "Error al cargar música de fondo: " << Mix_GetError() << std::endl;
    }

    if (Mix_PlayChannel(-1, soundEffect, 0) == -1) {
        std::cerr << "Error al reproducir sonido: " << Mix_GetError() << std::endl;
    }
}

void MediaPlayer::playeGameSound(GAME_SOUND sound) {
    switch (sound){
    case EQUIP:
        soundEffect = Mix_LoadWAV(EQUIP_SOUND);
        break;
    case REMOVE:
        soundEffect = Mix_LoadWAV(REMOVE_SOUND);
        break;
    default:
        break;
    }

    if (!soundEffect) {
        std::cerr << "Error al cargar música de fondo: " << Mix_GetError() << std::endl;
    }

    if (Mix_PlayChannel(-1, soundEffect, 0) == -1) {
        std::cerr << "Error al reproducir sonido: " << Mix_GetError() << std::endl;
    }
}

void MediaPlayer::setMute(bool mute) {
    if (mute) {
        // Mix_PauseMusic() // si quiero pausar la musica
        Mix_VolumeMusic(0);
    } else {
        // Mix_ResumeMusic() // si quiero reanudar la musica
        Mix_VolumeMusic(MIX_MAX_VOLUME);
    }
}

MediaPlayer::~MediaPlayer() {
    if (soundEffect != nullptr) Mix_FreeChunk(soundEffect);
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}
