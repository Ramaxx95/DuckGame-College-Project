#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <utility>
#include <vector>

#include "common/gameData.h"
#include "common/map.h"
#include "common/sockets.h"

#include "IActionableItem.h"
#include "bullet.h"
#include "gamestate.h"
#include "gametypes.h"
#include "physicsengine.h"
#include "playeraction.h"
#include "receiver.h"
#include "rectanglecollider.h"
#include "sender.h"

#define COLLIDER_WIDTH 16
#define COLLIDER_HEIGHT 23
#define COLLIDER_HEIGHT_DOWN 12
#define PLAYER_HEIGHT 32
#define COLLIDER_X_OFFSET 7
#define RAGDOLL_TIMER 30
class Player {
    Socket perr;
    Queue<GameStateMessage> q_sender;
    Queue<PlayerAction>& q_game_loop;
    Receiver receiver;
    Sender sender;
    // Fisicas
    PhysicsEngine& physics;
    PhysicsEngine::PhysicsState state;
    Position position;
    // Colisiones
    RectangleCollider collider;
    // PlayerInfo
    const int id;
    // Disparar
    std::unique_ptr<IActionableItem> weapon;
    Direction direction = Direction::NONE;
    bool head_equipped;
    bool body_equipped;
    bool dead;
    int match_wins;
    bool ragdoll;
    int ragdoll_counter;

public:
    Player(int id, PhysicsEngine& physics, Socket&& skt, Queue<PlayerAction>& q_game_loop);

    void update();

    void jump();

    std::vector<std::unique_ptr<Bullet>> press_shoot();

    std::vector<std::unique_ptr<Bullet>> update_weapon();

    bool has_weapon_actions();

    bool has_armor(const ArmorType& armor) const;

    void release_shoot();

    void set_weapon(std::unique_ptr<IActionableItem> new_weapon);

    void set_armor(ArmorType armor_equipped);

    void set_direction(Direction new_dir);

    void move(Direction dir);

    bool is_dead() const;

    void init();

    void send(const GameStateMessage& event);

    int get_id() const;

    const Position& get_position() const;

    void set_position(Position new_position);

    void set_groud_state(bool new_value);

    Player_Data get_data() const;

    const RectangleCollider& get_collider() const;

    void set_dead(bool value);

    bool get_dead_value() const;

    int increase_match_wins();

    void drop_weapon();

    void push(float force_x, float force_y);

    void receive_damage();

    void set_ragdool();

    bool cant_perform_action() const;

    ~Player();
};


#endif  // PLAYER_H
