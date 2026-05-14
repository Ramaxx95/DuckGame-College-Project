#ifndef GAME_H
#define GAME_H

#include <QApplication>
#include <memory>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/gameData.h"
#include "../common/map.h"
#include "../common/queue.h"
#include "../common/sockets.h"
#include "lobby/mainwindow.h"

#include "gameReceiver.h"
#include "gameSender.h"
#include "inputManager.h"
#include "match.h"

#define VERSION "v1.0"
#define MAX_PLAYERS 4

class Game {
private:
    Socket server;
    QApplication& app;
    std::shared_ptr<MainWindow> main_window;
    bool was_closed;

public:
    Game(const char* server_ip, const char* port, QApplication& app);
    int start();
    ~Game();
};

#endif  // GAME_H
