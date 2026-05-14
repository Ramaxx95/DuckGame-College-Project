#ifndef INITWINDOW_H
#define INITWINDOW_H

#include "../client/lobbyProtocol.h"
#include "../client/mediaPlayer.h"
#include "../common/map.h"

#include <QMainWindow>
#include <QGraphicsOpacityEffect>
#include <QVBoxLayout>
#include <QLabel>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class InitWindow;
}
QT_END_NAMESPACE

class InitWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit InitWindow(LobbyProtocol& protocol, std::shared_ptr<Map>& map, 
        int& duck_id, MediaPlayer& audio_player, QWidget* parent = nullptr);
    ~InitWindow();

private:
    Ui::InitWindow* ui;
    LobbyProtocol& protocol;
    int currentMenuIndex;
    int width;
    int height;
    std::shared_ptr<Map> map;
    int duck_id;
    MediaPlayer& audio_player;
    QGraphicsOpacityEffect* opacityEffect;
    QVBoxLayout* menuLayout;
    QLabel* menu_init_game;
    QLabel* menu_join_game;
    QLabel* menu_exit;

    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void updateIndicatorPosition();
    void colored_menu();

signals:
    void initWindowClosed(int duckId);
};
#endif  // INITWINDOW_H
