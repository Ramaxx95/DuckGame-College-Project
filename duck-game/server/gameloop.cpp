#include "gameloop.h"

#include <chrono>
#include <iostream>

const static int AMOUNT_OF_MATCHES_TO_WIN = 10;
const static int RELOAD_MATCH_ITERATIONS = 100;
const static int MIDDLE_ANIMATION_ITERATIONS = 100;
const static int END_ANIMATION_ITERATIONS = 50;

GameLoop::GameLoop(Queue<PlayerAction>& q_game_loop, PlayerController& players, const Map& map_r):
        q_game_loop(q_game_loop),
        running(true),
        players(players),
        map(map_r),
        game_map(map, 640, 480),
        show_middle_animation(false),
        show_end_game(false),
        iterations(0) {}

void GameLoop::run() {
    try {
        game_map.set_players_pos(players.get_players());

        const std::chrono::duration<double> FRAME_DURATION(1.0 / 30.0);  // 30 FPS
        auto start_time = std::chrono::steady_clock::now();              // Tiempo base del loop
        int it = 0;
        bool found_winner = false;
        while (running) {
            if (!show_middle_animation) {
                // Mientras no estoy mostrando la middle_animation proceso comandos de los players
                // Handleo la logica de ganadores si es necesario
                PlayerAction action;
                if (q_game_loop.try_pop(action)) {
                    proccess_action(action);
                }

                found_winner = handle_winners_logic();
            } else {
                iterations++;
                if (iterations >= MIDDLE_ANIMATION_ITERATIONS) {
                    show_middle_animation = false;
                    show_end_game = found_winner;
                    iterations = 0;
                }
            }

            players.update_all_players();

            for (auto& player: players.get_players()) {
                game_map.resolve_player_position(*player);
            }

            game_map.add_bullets(players.get_players_actions());

            game_map.update_map(players.get_players());

            players.send_all_players(generate_game_state());

            if (found_winner) {
                show_middle_animation = true;
                if (show_end_game) {
                    break;
                }
            }

            // Calcular tiempo de compensación
            auto next_frame_time = start_time + (++it * FRAME_DURATION);
            auto sleep_time = next_frame_time - std::chrono::steady_clock::now();

            if (sleep_time > std::chrono::duration<double>(0)) {
                std::this_thread::sleep_for(sleep_time);
            }
        }
        running = false;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}


GameStateMessage GameLoop::generate_game_state() const {
    GameStateMessage msg{};
    msg.players = players.get_players_data();
    msg.player_count = players.get_cant_players();
    msg.map_data = game_map.get_map_data();
    msg.show_middle_animation = show_middle_animation;
    msg.show_end_game = show_end_game;
    msg.map_data_size = GameState::calculate_map_data_size(msg.map_data);

    return msg;
}

bool GameLoop::handle_winners_logic() {
    int winner_id = players.get_winner();

    if (winner_id == -1)
        return false;

    // seteo posicion inicial de los players
    game_map.set_players_pos(players.get_players());

    // revivo a todos los players
    players.revive_all_players();

    // sumo +1 al player ganador
    int match_wins = players.set_winner(winner_id);

    // chequeo si esta a la mitad del juego
    if (match_wins == AMOUNT_OF_MATCHES_TO_WIN / 2) {
        // Avisar a los clientes para la animacion de la mitad del juego
        show_middle_animation = true;
        iterations++;
    }

    return match_wins == AMOUNT_OF_MATCHES_TO_WIN;
}

void GameLoop::proccess_action(const PlayerAction& action) {
    auto player = players.get_player(action.playerId);
    if (player == nullptr || player->get_dead_value() || player->cant_perform_action()) {
        return;
    }
    switch (action.action) {
        case JUMP:
            player->jump();
            break;

        case MOVE: {
            player->move(action.direction);
            player->set_direction(action.direction);
            break;
        }
        case LOOK:
            player->set_direction(action.direction);
            break;

        case PRESS_FIRE:
            game_map.add_bullets(player->press_shoot());
            break;

        case RELEASE_FIRE:
            player->release_shoot();
            break;

        case PICKUP: {
            std::unique_ptr<IActionableItem> weapon(game_map.get_weapon_from_near_spawner(*player));
            if (weapon) {
                player->set_weapon(std::move(weapon));
            }

            ArmorType armor(game_map.get_armor_from_near_spawner(*player));
            if (armor != ARMOR_NO) {
                player->set_armor(armor);
            }
            break;
        }
        default:
            return;
    }
}

bool GameLoop::is_running() { return running; }

void GameLoop::end_game() { running = false; }
