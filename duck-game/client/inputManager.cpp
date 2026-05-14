#include "inputManager.h"

InputManager::InputManager(Queue<uint8_t>& input_queue, bool& game_running):
        input(input_queue), game_running(game_running) {}

void InputManager::run() {
    const std::chrono::duration<double> FRAME_DURATION(1.0 / 30.0); // 30 FPS
    auto start_time = std::chrono::steady_clock::now(); // Tiempo base del loop
    int it = 0;

    // Estado persistente de teclas
    const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);

    while (this->game_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                this->game_running = false;
                break;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                this->game_running = false;
                break;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_z) {
                this->input.push(FIRE_WEAPON);
            } else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_z) {
                this->input.push(STOP_FIRE);
            }
        }

        // Determinar acción según el estado actual del teclado
        uint8_t action = 0;

        if (keyboard_state[SDL_SCANCODE_RIGHT]) {
            action = MOVE_RIGHT;
        } else if (keyboard_state[SDL_SCANCODE_LEFT]) {
            action = MOVE_LEFT;
        }

        if (keyboard_state[SDL_SCANCODE_UP]) {
            action = LOOK_UP;
        }

        if (keyboard_state[SDL_SCANCODE_SPACE]) {
            action = ACTION_JUMP;
        }

        if (keyboard_state[SDL_SCANCODE_DOWN]) {
            action = ACTION_LIE_DOWN;
        } else if (keyboard_state[SDL_SCANCODE_X]) {
            action = PICK_WEAPON;
        } else if (keyboard_state[SDL_SCANCODE_LCTRL]) {
            action = DROP_WEAPON;
        }

        if (this->game_running && action != 0) {
            this->input.push(action);
        }

        // tiempo de compensación (delay)
        auto next_frame_time = start_time + (++it * FRAME_DURATION);
        auto sleep_time = next_frame_time - std::chrono::steady_clock::now();

        // Limitar la tasa de procesamiento de inputs (30 FPS)
        if (sleep_time > std::chrono::duration<double>(0)) {
            std::this_thread::sleep_for(sleep_time);
        }
    }
}



InputManager::~InputManager() {}
