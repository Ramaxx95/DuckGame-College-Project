#include "player.h"

#include "playerweapon.h"

Player::Player(const int id, PhysicsEngine& physics, Socket&& skt,
               Queue<PlayerAction>& q_game_loop):
        perr(std::move(skt)),
        q_sender(1),
        q_game_loop(q_game_loop),
        receiver(perr, q_game_loop, id),
        sender(perr, q_sender),
        physics(physics),
        state({0.0, 0.0, false}),
        collider(position, COLLIDER_WIDTH, COLLIDER_HEIGHT, PLAYER_HEIGHT, COLLIDER_X_OFFSET),
        id(id),
        head_equipped(false),
        body_equipped(false),
        dead(false),
        match_wins(0),
        ragdoll(false),
        ragdoll_counter(1) {}

void Player::init() {
    sender.start();
    receiver.start();
}

void Player::send(const GameStateMessage& event) { q_sender.try_push(event); }

bool Player::is_dead() const { return receiver.is_running(); }

void Player::jump() {
    if (this->direction != Direction::DOWN) {
        physics.jump(state);
    }
}

void Player::move(const Direction dir) {
    physics.move(state, dir);
    collider.changeSize(COLLIDER_HEIGHT);
}

void Player::set_direction(const Direction new_dir) {
    direction = new_dir;
    if (direction == Direction::DOWN) {
        collider.changeSize(COLLIDER_HEIGHT_DOWN);
    }
}

void Player::update() {
    physics.update(state, position);
    collider.update_position(position);

    // TODO mover a un metodo en concreto privado
    if (ragdoll) {
        ragdoll_counter--;
        if (ragdoll_counter <= 0) {
            ragdoll = false;
        }
    }
}

void Player::push(float force_x, float force_y) { physics.add_force(state, force_x, force_y); }

int Player::get_id() const { return id; }

Player_Data Player::get_data() const {
    Player_Data data = physics.get_data_state(state, position);
    data.is_lying_down = false;
    data.is_facing_up = false;
    data.dead = dead;
    data.score = match_wins;
    data.body_armor_equipped = body_equipped;
    data.head_armor_equipped = head_equipped;
    if (weapon) {
        data.gun_equipped = weapon->get_type();
        data.ammo = weapon->get_ammo();
    }

    if (direction == Direction::DOWN && this->state.is_grounded) {
        data.is_lying_down = true;
    }
    if (direction == Direction::UP) {
        data.is_facing_up = true;
    }

    return data;
}

const Position& Player::get_position() const { return position; }

const RectangleCollider& Player::get_collider() const { return collider; }


void Player::set_position(const Position new_position) {
    position = new_position;
    collider.update_position(position);
}

void Player::set_groud_state(const bool new_value) {
    physics.set_new_ground_state(state, new_value);
}

void Player::set_dead(bool value) {
    dead = value;
    this->drop_weapon();
    this->body_equipped = false;
    this->head_equipped = false;
}

int Player::increase_match_wins() {
    match_wins++;
    return match_wins;
}

std::vector<std::unique_ptr<Bullet>> Player::press_shoot() {
    if (weapon != nullptr && this->direction != Direction::DOWN) {
        return weapon->on_press_action(position, direction);
    }
    return {};
}

void Player::release_shoot() {
    if (weapon) {
        weapon->on_release_action();
    }
}

bool Player::has_weapon_actions() {
    if (weapon) {
        return weapon->has_action();
    }
    return false;
}

bool Player::has_armor(const ArmorType& armor) const {
    if (armor == ARMOR_HEAD) {
        return this->head_equipped;
    } else if (armor == ARMOR_BODY) {
        return this->body_equipped;
    }
    return true;
}


std::vector<std::unique_ptr<Bullet>> Player::update_weapon() {
    if (weapon != nullptr && this->direction != Direction::DOWN) {
        return weapon->update(position, direction);
    }
    return {};
}

void Player::set_weapon(std::unique_ptr<IActionableItem> new_weapon) {
    weapon = std::move(new_weapon);
}

void Player::set_armor(ArmorType armor_equipped) {

    if (armor_equipped == ARMOR_HEAD && !this->head_equipped) {
        this->head_equipped = true;
    } else if (armor_equipped == ARMOR_BODY && !this->body_equipped) {
        this->body_equipped = true;
    }
}

void Player::drop_weapon() { weapon = nullptr; }

bool Player::get_dead_value() const { return dead; }

void Player::receive_damage() {
    if (this->head_equipped) {
        this->head_equipped = false;
        return;
    }
    if (this->body_equipped) {
        this->body_equipped = false;
        return;
    }
    set_dead(true);
}

void Player::set_ragdool() {
    ragdoll = true;
    direction = Direction::DOWN;
}

bool Player::cant_perform_action() const { return ragdoll; }

Player::~Player() {
    // TODO agregar un perr shutdown and close
    // para el caso de que se cierre el server sin que los clientes se desconecten
    q_sender.close();
    sender.join();
    receiver.join();
}
