#include "game.h"

#include "../common/map.h"
#include "../common/queue.h"
#include "init/initwindow.h"
#include "lobby/mainwindow.h"

#include "mediaPlayer.h"

const static int GAME_STATE_MAX_SIZE = 100;  // TODO: Validar este valor

Game::Game(const char* server_ip, const char* port, QApplication& app):
        server(server_ip, port), app(app), was_closed(false) {}

int Game::start() {
    try {
        bool lobby_was_closed = false;
        int duck_id = 0;
        int amount_of_players = 0;
        std::shared_ptr<Map> map = std::make_shared<Map>();

        // Empezar en el Lobby
        std::cout << "Starting Lobby\n";
        MediaPlayer audio_player;
        LobbyProtocol protocol(server);
        InitWindow init_window(protocol, map, duck_id, audio_player);

        QObject::connect(&init_window, &InitWindow::initWindowClosed, [&](int new_duck_id) {
            if (new_duck_id == -1) {
                init_window.close();
                lobby_was_closed = true;
                return;
            }

            duck_id = new_duck_id;
            main_window = std::make_shared<MainWindow>(protocol, map, duck_id, audio_player);

            QObject::connect(main_window.get(), &MainWindow::lobbyClosedSuccessfully,
                             [&](int size) {
                                 amount_of_players = size;
                                 std::cout << "Lobby closed successfully, quitting app\n";
                                 app.quit();
                             });

            QObject::connect(main_window.get(), &MainWindow::lobbyClosedError, [&]() {
                std::cout << "Lobby closed with error, quitting app\n";
                lobby_was_closed = true;
                app.quit();
                return 0;
            });

            main_window->show();
            init_window.close();
        });

        init_window.show();
        app.exec();

        if (lobby_was_closed) {
            return 0;
        }

        std::cout << "Starting Game\n";

        std::string game_name = "DuckGame ";
        game_name.append(VERSION);
        game_name.append(" - Player " + std::to_string(duck_id));

        Queue<uint8_t> player_input = Queue<uint8_t>(1);

        Queue<GameStateAnimations> game_state = Queue<GameStateAnimations>(GAME_STATE_MAX_SIZE);
        Queue<Player_Data> players_data = Queue<Player_Data>(amount_of_players);
        Queue<MapData> map_data = Queue<MapData>(2);

        bool game_running = true;

        // Dejamos que duerma por un rato asi puede cargar sin problemas
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        GameSender sender = GameSender(player_input, this->server, game_running);
        GameReceiver receiver =
                GameReceiver(players_data, map_data, game_state, this->server, game_running);
        InputManager input = InputManager(player_input, game_running);

        Match game_match(game_name, map->get_matrix(), players_data, map_data, game_state,
                         game_running, amount_of_players, audio_player);

        input.start();
        sender.start();
        receiver.start();

        game_match.run();

        if (!game_running) {
            player_input.close();
            players_data.close();

            this->server.shutdown(2);
            this->server.close();
            this->was_closed = true;
            std::cout << "[DEBUG] Conexion con el Server cerrada\n";
        }

        std::cout << "[DEBUG] Cerrando threads...\n";

        input.join();
        sender.join();
        receiver.join();

        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

Game::~Game() {
    this->app.quit();
    if (!this->was_closed) {
        this->server.shutdown(2);
        this->server.close();
    }
}
