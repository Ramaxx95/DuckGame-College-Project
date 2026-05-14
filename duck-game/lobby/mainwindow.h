#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../common/map.h"
#include "../client/lobbyProtocol.h"
#include "../client/mediaPlayer.h"

#include <QMainWindow>
#include <QLabel>
#include <vector>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    explicit MainWindow(LobbyProtocol& protocol, 
        std::shared_ptr<Map> map, int& duck_id, MediaPlayer& audio_player,
        QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LobbyProtocol& protocol;
    int duck_id;
    std::shared_ptr<Map> map;
    std::vector<QPixmap> duck_moves;
    std::vector<QPixmap> duck_jumps;
    std::vector<QPixmap> console_imgs;
    int i = 0;
    QTimer *moveTimer;
    int currentDirection;
    int isJumping;
    int currentVelocityY;
    int groundY;
    int lastDirection;
    bool is_ready;
    int amount_of_players;
    std::vector<int> map_ids;
    int selected_map_id;
    bool menuOpen;
    MediaPlayer& audio_player;

    void openDoors(QLabel* leftDoor, QLabel* rightDoor);
    void updateDuckPosition();
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void load_sprites(QPixmap running, QPixmap jumping);
    void set_doors_cfg(QPixmap left_door_img, QPixmap right_door_img);
    void set_background_cfg(QPixmap leftBackgroundImage, QPixmap rightBackgroundImage);
    void set_foreground_cfg(QPixmap leftForegroundImage, QPixmap rightForegroundImage);
    void load_spritesConsole(QPixmap spriteSheet);
    void toggleMenu(bool open);
    void handleProtocolOperations();
    void setupConnections();
    void onLobbyClosed(int amount_of_players);

signals:
    void lobbyClosedSuccessfully(int amount_of_players);
    void lobbyClosedError();
};

#endif // MAINWINDOW_H
