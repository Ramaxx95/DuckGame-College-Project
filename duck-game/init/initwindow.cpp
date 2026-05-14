#include "initwindow.h"

#include <QKeyEvent>
#include <iostream>
#include <utility>

#include "./ui_initwindow.h"

const static int WIDTH = 640;
const static int HEIGH = 480;

InitWindow::InitWindow(LobbyProtocol& protocol, std::shared_ptr<Map>& map, int& duck_id,
            MediaPlayer& audio_player, QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::InitWindow),
        protocol(protocol),
        map(map),
        duck_id(duck_id),
        audio_player(audio_player),
        width(WIDTH),
        height(HEIGH),
        currentMenuIndex(0) {

    ui->setupUi(this);
    audio_player.backgroundSound(MediaPlayer::INIT);
    audio_player.play();

    QPixmap welcome_img(QString::fromStdString(IMG_PATH "/duckGameTitle.png"));
    QPixmap background_img(QString::fromStdString(IMG_PATH "/cryoBackground.png"));
    QPixmap menu_img(QString::fromStdString(IMG_PATH "/tutorialTV.png"));

    // Configuración del fondo
    ui->background->setPixmap(background_img);
    ui->background->setScaledContents(true);
    ui->background->setFixedSize(width, height);
    ui->background->setGeometry(0, 0, width, height);

    // Efecto de opacidad
    opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.9);
    ui->background->setGraphicsEffect(opacityEffect);

    // Configuración del menú
    ui->menu->setPixmap(menu_img);
    ui->menu->setScaledContents(true);
    ui->menu->setFixedSize(width / 2, height / 2);
    int menu_x = (width - ui->menu->width()) / 2;
    int menu_y = (height - ui->menu->height()) / 2;
    ui->menu->setGeometry(menu_x, menu_y, ui->menu->width(), ui->menu->height());
    ui->menu->hide();

    // Crear un QVBoxLayout para el menú
    menuLayout = new QVBoxLayout(ui->menu);

    // Crear los QLabels y agregarlos al layout
    menu_init_game = new QLabel("Iniciar Partida", ui->menu);
    menu_init_game->setAlignment(Qt::AlignCenter);
    menu_init_game->setStyleSheet("font-size: 20px; color: yellow;");
    menuLayout->addWidget(menu_init_game, 0, Qt::AlignCenter);

    menu_join_game = new QLabel("Unirse a Partida", ui->menu);
    menu_join_game->setAlignment(Qt::AlignCenter);
    menu_join_game->setStyleSheet("font-size: 20px; color: white;");
    menuLayout->addWidget(menu_join_game, 0, Qt::AlignCenter);

    menu_exit = new QLabel("Salir", ui->menu);
    menu_exit->setAlignment(Qt::AlignCenter);
    menu_exit->setStyleSheet("font-size: 20px; color: white;");
    menuLayout->addWidget(menu_exit, 0, Qt::AlignCenter);

    ui->menu->setLayout(menuLayout);

    // Configuración de welcome_msg
    ui->welcome_msg->setPixmap(welcome_img);
    ui->welcome_msg->setScaledContents(true);
    ui->welcome_msg->setFixedSize(width / 2, height / 2);

    int welcome_msg_x = (width - ui->welcome_msg->width()) / 2;
    int welcome_msg_y = (height - ui->welcome_msg->height()) / 2;

    ui->welcome_msg->setGeometry(welcome_msg_x, welcome_msg_y, width / 2, height / 2);

    ui->background->raise();
    ui->welcome_msg->raise();
    ui->menu->raise();
    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::StrongFocus);
}

void InitWindow::mousePressEvent(QMouseEvent* event) { ui->menu->show(); }

void InitWindow::keyPressEvent(QKeyEvent* event) {
    Map wrapper = Map();
    if (event->key() == Qt::Key_Up && ui->menu->isVisible()) {
        if (currentMenuIndex == 0) {
            currentMenuIndex = ui->menu->layout()->count() - 1;
        } else {
            currentMenuIndex--;
        }
        audio_player.playMenuSounds();
        colored_menu();
    } else if (event->key() == Qt::Key_Down && ui->menu->isVisible()) {
        if (currentMenuIndex == ui->menu->layout()->count() - 1) {
            currentMenuIndex = 0;
        } else {
            currentMenuIndex++;
        }
        colored_menu();
        audio_player.playMenuSounds();
    } else if (event->key() == Qt::Key_Return && ui->menu->isVisible()) {
        switch (currentMenuIndex) {
            case 0:
                protocol.send_create();
                wrapper = protocol.receive_create_success(duck_id);
                *map = wrapper;
                std::cout << "Duck number: " << duck_id << std::endl;
                if (duck_id != 0) {
                    emit initWindowClosed(duck_id);
                    close();
                }
                break;
            case 1:
                protocol.request_join();
                protocol.reveive_join_success(duck_id);
                if (duck_id != 0) {
                    emit initWindowClosed(duck_id);
                    close();
                }
                break;
            case 2:
                emit initWindowClosed(-1);
                close();
                break;
        }
    }
}

void InitWindow::colored_menu() {
    for (int i = 0; i < ui->menu->layout()->count(); i++) {
        ui->menu->layout()->itemAt(i)->widget()->setStyleSheet("font-size: 20px; color: white;");
    }
    ui->menu->layout()
            ->itemAt(currentMenuIndex)
            ->widget()
            ->setStyleSheet("font-size: 20px; color: yellow;");
}

InitWindow::~InitWindow() { 
    if (menu_exit) delete menu_exit;
    if (menu_join_game) delete menu_join_game;
    if (menu_init_game) delete menu_init_game;
    if (menuLayout) delete menuLayout;
    if (opacityEffect) delete opacityEffect;
    delete ui; 
}
