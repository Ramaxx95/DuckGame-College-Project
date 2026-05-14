#include "mainwindow.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QRect>
#include <QTimer>
#include <QVBoxLayout>
#include <iostream>
#include <string>
#include <vector>
#include <QtConcurrent/QtConcurrent>

#include "./ui_mainwindow.h"

const static int WIDTH = 640;
const static int HEIGHT = 480;
const static int DOOR_WIDTH = 160;
const static int DOOR_HEIGHT = 480;
const static int FRAME_WIDTH = 32;
const static int FRAME_HEIGHT = 32;
const static int INITIAL_DUCK_SPRITE = 2;
const static int ALLOWED_SPRITES = 6;
const static int GRAVITY = 2;
const static int JUMP_VELOCITY = 25;

MainWindow::MainWindow(LobbyProtocol& protocol, std::shared_ptr<Map> map, int& duck_id,
            MediaPlayer& audio_player, QWidget* parent):

        QMainWindow(parent),
        ui(new Ui::MainWindow),
        protocol(protocol),
        duck_id(duck_id),
        map(map),
        isJumping(false),
        currentDirection(1),
        currentVelocityY(0),
        lastDirection(1),
        is_ready(false),
        amount_of_players(0),
        selected_map_id(0),
        menuOpen(false),
        audio_player(audio_player) {
    ui->setupUi(this);
    ui->stackedWidget->setGeometry(0, 0, ui->stackedWidget->width(), ui->stackedWidget->height());

    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &MainWindow::updateDuckPosition);

    // int padding = 0; -> cppcheck: nunca se usa
    int baseY = 60;

    audio_player.backgroundSound(MediaPlayer::LOBBY);

    std::string str_duck_id = std::to_string(duck_id);

    // Load images
    QPixmap left_door_img(QString::fromStdString(IMG_PATH "/selectDoorLeft.png"));
    QPixmap right_door_img(QString::fromStdString(IMG_PATH "/selectDoorRight.png"));

    QPixmap leftBackgroundImage(QString::fromStdString(IMG_PATH "/leftRoomBackground.png"));
    QPixmap rightBackgroundImage(QString::fromStdString(IMG_PATH "/rightRoomBackground.png"));

    QPixmap leftForegroundImage(QString::fromStdString(IMG_PATH "/leftRoomForeground.png"));
    QPixmap rightForegroundImage(QString::fromStdString(IMG_PATH "/rightRoomForeground.png"));

    QPixmap readyPlayerOne(QString::fromStdString(IMG_PATH "/readyRight.png"));
    QPixmap readyPlayerTwo(QString::fromStdString(IMG_PATH "/readyLeft.png"));

    QPixmap spriteDuckRunning(
        QString::fromStdString(SPRITES_PATH "/P" + str_duck_id + "/running.png"));

    QPixmap spriteDuckJumping(
        QString::fromStdString(SPRITES_PATH "/P" + str_duck_id + "/jumping.png"));
    
    QPixmap spriteSheet_m(QString::fromStdString(SPRITES_PATH "/P2/running.png"));
    QPixmap spriteSheetConsole(QString::fromStdString(IMG_PATH "/selectConsole.png"));

    QPixmap menu_img(QString::fromStdString(IMG_PATH "/tutorialTV.png"));

    load_sprites(spriteDuckRunning, spriteDuckJumping);

    load_spritesConsole(spriteSheetConsole);
    set_doors_cfg(left_door_img, right_door_img);
    set_background_cfg(leftBackgroundImage, rightBackgroundImage);
    set_foreground_cfg(leftForegroundImage, rightForegroundImage);

    // Configuración del menú
    ui->menu->setPixmap(menu_img);
    ui->menu->setScaledContents(true);
    ui->menu->setFixedSize(WIDTH / 2, HEIGHT / 2);
    int menu_x = (WIDTH - ui->menu->width()) / 2;
    int menu_y = (HEIGHT - ui->menu->height()) / 2;
    ui->menu->setGeometry(menu_x, menu_y, ui->menu->width(), ui->menu->height());
    ui->menu->hide();

    // Crear un QVBoxLayout para el menú
    QVBoxLayout* menuLayout = new QVBoxLayout(ui->menu);

    ui->map_selected_text->setText("Mapa Nro: " + QString::number(selected_map_id));
    ui->map_selected_text->setStyleSheet("font-size: 20px; color: white; font-weight: bold;");
    ui->map_selected_text->adjustSize();
    ui->map_selected_text->move(50, 420);
    ui->map_selected_text->show();

    // Crear los QLabels y agregarlos al layout
    ui->menu_text->setText("Mapa Nro: " + QString::number(selected_map_id));
    ui->menu_text->setAlignment(Qt::AlignCenter);
    ui->menu_text->setStyleSheet("font-size: 20px; color: yellow;");
    menuLayout->addWidget(ui->menu_text, 0, Qt::AlignCenter);

    // Console config
    ui->console->setPixmap(console_imgs[1]);
    ui->console->setScaledContents(true);
    ui->console->setFixedSize(64, 64);
    ui->console->setGeometry(0, 230, 64, 64);

    // Ready player one config
    ui->ready_player_one->setPixmap(readyPlayerOne);
    ui->ready_player_one->setScaledContents(true);
    ui->ready_player_one->setGeometry(DOOR_WIDTH * 2 - 100, baseY - 45, 80, 30);
    ui->ready_player_one->hide();

    // Ready player two config
    ui->ready_player_two->setPixmap(readyPlayerTwo);
    ui->ready_player_two->setScaledContents(true);
    ui->ready_player_two->setGeometry(20 + DOOR_WIDTH * 2, baseY - 45, 80, 30);
    ui->ready_player_two->hide();

    // Duck left config
    ui->duck_left->setPixmap(duck_moves[INITIAL_DUCK_SPRITE]);
    ui->duck_left->setScaledContents(true);
    ui->duck_left->setFixedSize(60, 75);
    ui->duck_left->setGeometry(150, 300, 40, 75);

    // Duck right config
    ui->duck_right->setPixmap(
            duck_moves[INITIAL_DUCK_SPRITE].transformed(QTransform().scale(-1, 1)));
    ui->duck_right->setScaledContents(true);
    ui->duck_right->setFixedSize(60, 75);
    ui->duck_right->setGeometry(550, 300, 40, 75);

    groundY = ui->duck_left->y();

    if (duck_id == 1) {
        openDoors(ui->left_door_player_one, ui->right_door_player_one);
    } else {
        openDoors(ui->left_door_player_one, ui->right_door_player_one);
        openDoors(ui->left_door_player_two, ui->right_door_player_two);
    }

    if (duck_id == 1) {
        std::cout << "[DEBUG] Requesting map ids\n";
        map_ids = protocol.receive_map_ids();
        std::cout << "[DEBUG] Received: " << map_ids.size() << " ids\n";
    }

    ui->ready_player_one->raise();
    ui->ready_player_two->raise();
    ui->console->raise();
    ui->duck_left->raise();
    ui->duck_right->raise();
    ui->right_door_player_one->raise();
    ui->left_door_player_one->raise();
    ui->right_door_player_two->raise();
    ui->left_door_player_two->raise();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->isAutoRepeat())
        return;

    switch (event->key()) {
        case Qt::Key_Left:
            if (menuOpen) {
                if (selected_map_id == 0) {
                    selected_map_id = map_ids.size() - 1;
                } else {
                    selected_map_id--;
                }
                ui->menu_text->setText("Mapa Nro: " + QString::number(selected_map_id));
                ui->map_selected_text->setText("Mapa Nro: " + QString::number(selected_map_id));
                audio_player.playMenuSounds();
                break;
            }
            currentDirection = -1;
            moveTimer->start(30);
            break;
        case Qt::Key_Right:
            if (menuOpen) {
                if (selected_map_id >= map_ids.size() - 1) {
                    selected_map_id = 0;
                } else {
                    selected_map_id++;
                }
                ui->menu_text->setText("Mapa Nro: " + QString::number(selected_map_id));
                ui->map_selected_text->setText("Mapa Nro: " + QString::number(selected_map_id));
                audio_player.playMenuSounds();
                break;
            }
            currentDirection = 1;
            moveTimer->start(30);
            break;
        case Qt::Key_Space:
            if (!isJumping) {
                isJumping = true;
                currentVelocityY = -JUMP_VELOCITY;
                moveTimer->start(30);
                audio_player.playJumpSound();
            }
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if (ui->duck_left->x() == ui->console->x() && ui->duck_left->y() == ui->console->y() &&
                !menuOpen) {
                toggleMenu(true);
            }
            break;
        case Qt::Key_Escape:
            if (ui->menu->isVisible()) {
                toggleMenu(false);
            }
            break;
        default:
            QMainWindow::keyPressEvent(event);
            return;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    if (event->isAutoRepeat())
        return;

    if ((event->key() == Qt::Key_Left && currentDirection == -1) ||
        (event->key() == Qt::Key_Right && currentDirection == 1)) {
        currentDirection = 0;
    }
}

void MainWindow::updateDuckPosition() {
    int x = ui->duck_left->x();
    int y = ui->duck_left->y();

    if (currentDirection == -1 && x > 0) {
        x -= 10;
        lastDirection = -1;
        ui->duck_left->setPixmap(duck_moves[i].transformed(QTransform().scale(-1, 1)));
    } else if (currentDirection == 1 && x < 270) {
        x += 10;
        lastDirection = 1;
        ui->duck_left->setPixmap(duck_moves[i]);
    }

    i = (i + 1) % duck_moves.size();

    if (isJumping || y < groundY) {
        y += currentVelocityY;
        currentVelocityY += GRAVITY;

        int index = 0;
        if (currentVelocityY < -15)
            index = 0;
        else if (currentVelocityY < -5)
            index = 1;
        else if (currentVelocityY < 0)
            index = 2;
        else if (currentVelocityY > 0 && currentVelocityY < 5)
            index = 3;
        else if (currentVelocityY >= 5 && currentVelocityY < 15)
            index = 4;
        else if (currentVelocityY >= 15)
            index = 5;

        QPixmap duckPixmap = duck_jumps[index];
        if (lastDirection == -1)
            duckPixmap = duckPixmap.transformed(QTransform().scale(-1, 1));
        ui->duck_left->setPixmap(duckPixmap);

        if (y >= groundY && x >= 60 && x <= 160) {
            y = groundY;
            isJumping = false;
            currentVelocityY = 0;
        } else if (y >= 230 && y < groundY) {
            if (x >= 0 && x <= 60) {
                y = 230;
                isJumping = false;
                currentVelocityY = 0;
            } else if (x >= 160 && x <= 270) {
                y = 230;
                isJumping = false;
                currentVelocityY = 0;
            }
        }
    }

    ui->duck_left->move(x, y);
    if (currentDirection == 0 && y == groundY) {
        i = 0;
        if (lastDirection == -1) {
            ui->duck_left->setPixmap(duck_moves[i].transformed(QTransform().scale(-1, 1)));
        } else {
            ui->duck_left->setPixmap(duck_moves[i]);
        }
        moveTimer->stop();
    }

    if (x == ui->console->x() && y == ui->console->y()) {
        ui->console->setPixmap(console_imgs[0]);
    } else {
        ui->console->setPixmap(console_imgs[1]);
    }

    if (x >= 170 && y == 230 && !is_ready) {
        is_ready = true;
        ui->ready_player_one->show();
        QtConcurrent::run(this, &MainWindow::handleProtocolOperations);
    }
}

void MainWindow::handleProtocolOperations() {
    try {
        std::cout << "[DEBUG] Sending ready signal...\n";
        protocol.send_ready();

        std::cout << "[DEBUG] Waiting for all clients to be ready...\n";
        protocol.receive_all_clients_ready();
        std::cout << "[DEBUG] All clients are ready!\n";

        protocol.init_game(selected_map_id);
        Map wrapper = protocol.receive_init_game(amount_of_players);
        *map = wrapper;
        map.get()->print();

        emit lobbyClosedSuccessfully(amount_of_players);

        // Esto asegura que el cierre se realice correctamente
        // DOC: https://doc.qt.io/qt-6/qtconcurrent.html
        QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Exception in protocol operations: " << e.what() << "\n";
        emit lobbyClosedError();
    }
}

void MainWindow::setupConnections() {
    connect(this, &MainWindow::lobbyClosedSuccessfully, this, &MainWindow::onLobbyClosed);
}

void MainWindow::onLobbyClosed(int amount_of_players) {
    std::cout << "[DEBUG] Lobby closed successfully with " << amount_of_players << " players.\n";
    close();
}

void MainWindow::toggleMenu(bool open) {
    menuOpen = open;
    ui->menu->setVisible(open);
    ui->menu->raise();

    if (menuOpen) {
        moveTimer->stop();
    } else {
        moveTimer->start();
    }
}


// derecha abajo max: x = 160, y = 300
// izquierda abajo max: x = 60, y = 300
// bloque izquierda: y = 252
// izquierda max = x = 0, y = 252
// derecha max = x = 270, y = 252
void MainWindow::openDoors(QLabel* leftDoor, QLabel* rightDoor) {
    QPropertyAnimation* leftDoorAnimation = new QPropertyAnimation(leftDoor, "geometry");
    leftDoorAnimation->setDuration(1000);
    leftDoorAnimation->setStartValue(leftDoor->geometry());
    leftDoorAnimation->setEndValue(
            QRect(leftDoor->x() - 150, leftDoor->y(), leftDoor->width(), leftDoor->height()));

    connect(leftDoorAnimation, &QPropertyAnimation::finished, [this, leftDoor]() {
        leftDoor->setEnabled(false);
        leftDoor->hide();
    });

    QPropertyAnimation* rightDoorAnimation = new QPropertyAnimation(rightDoor, "geometry");
    rightDoorAnimation->setDuration(1000);
    rightDoorAnimation->setStartValue(rightDoor->geometry());
    rightDoorAnimation->setEndValue(
            QRect(rightDoor->x() + 150, rightDoor->y(), rightDoor->width(), rightDoor->height()));

    connect(leftDoorAnimation, &QPropertyAnimation::finished, [this, rightDoor]() {
        rightDoor->setEnabled(false);
        rightDoor->hide();
    });

    leftDoorAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    rightDoorAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::load_sprites(QPixmap running, QPixmap jumping) {
    for (int i = 0; i < ALLOWED_SPRITES; i++) {
        duck_moves.push_back(running.copy(i * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
    }

    for (int i = 0; i < ALLOWED_SPRITES; i++) {
        duck_jumps.push_back(
                jumping.copy(i * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
    }
}

void MainWindow::set_doors_cfg(QPixmap left_door_img, QPixmap right_door_img) {
    // Left door player one config
    ui->left_door_player_one->setPixmap(left_door_img);
    ui->left_door_player_one->setScaledContents(true);
    ui->left_door_player_one->setFixedSize(DOOR_WIDTH, DOOR_HEIGHT);
    ui->left_door_player_one->setGeometry(0, 0, DOOR_WIDTH, DOOR_HEIGHT);

    // Left door player two config
    ui->left_door_player_two->setPixmap(left_door_img);
    ui->left_door_player_two->setScaledContents(true);
    ui->left_door_player_two->setFixedSize(DOOR_WIDTH + 30, DOOR_HEIGHT);
    ui->left_door_player_two->setGeometry(DOOR_WIDTH * 2, 0, DOOR_WIDTH, DOOR_HEIGHT);

    // Right door player one config
    ui->right_door_player_one->setPixmap(right_door_img);
    ui->right_door_player_one->setScaledContents(true);
    ui->right_door_player_one->setFixedSize(DOOR_WIDTH, DOOR_HEIGHT);
    ui->right_door_player_one->setGeometry(DOOR_WIDTH, 0, DOOR_WIDTH, DOOR_HEIGHT);

    // Right door player two config
    ui->right_door_player_two->setPixmap(right_door_img);
    ui->right_door_player_two->setScaledContents(true);
    ui->right_door_player_two->setFixedSize(DOOR_WIDTH, DOOR_HEIGHT);
    ui->right_door_player_two->setGeometry(DOOR_WIDTH * 3, 0, DOOR_WIDTH, DOOR_HEIGHT);
}

void MainWindow::set_background_cfg(QPixmap leftBackgroundImage, QPixmap rightBackgroundImage) {
    // Left Background config
    ui->leftBackground->setPixmap(leftBackgroundImage);
    ui->leftBackground->setScaledContents(true);
    ui->leftBackground->setFixedSize(DOOR_WIDTH * 2, DOOR_HEIGHT);
    ui->leftBackground->setGeometry(0, 0, DOOR_WIDTH, DOOR_HEIGHT);

    // Right Background config
    ui->rightBackground->setPixmap(rightBackgroundImage);
    ui->rightBackground->setScaledContents(true);
    ui->rightBackground->setFixedSize(DOOR_WIDTH * 2, DOOR_HEIGHT);
    ui->rightBackground->setGeometry(DOOR_WIDTH * 2, 0, DOOR_WIDTH * 2, DOOR_HEIGHT);
}

void MainWindow::set_foreground_cfg(QPixmap leftForegroundImage, QPixmap rightForegroundImage) {
    // Left Foreground config
    ui->leftForeground->setPixmap(leftForegroundImage);
    ui->leftForeground->setScaledContents(true);
    ui->leftForeground->setFixedSize(DOOR_WIDTH * 2, DOOR_HEIGHT);
    ui->leftForeground->setGeometry(0, 0, DOOR_WIDTH * 2, DOOR_HEIGHT);

    // Right Foreground config
    ui->rightForeground->setPixmap(rightForegroundImage);
    ui->rightForeground->setScaledContents(true);
    ui->rightForeground->setFixedSize(DOOR_WIDTH * 2, DOOR_HEIGHT);
    ui->rightForeground->setGeometry(DOOR_WIDTH * 2, 0, DOOR_WIDTH * 2, DOOR_HEIGHT);
}

void MainWindow::load_spritesConsole(QPixmap spriteSheet) {
    for (int i = 0; i < 2; i++) {
        console_imgs.push_back(spriteSheet.copy(i * 20, 0, 20, 20));
    }
}

MainWindow::~MainWindow() { 
    moveTimer->stop();
    delete moveTimer;
    delete ui; 
}
