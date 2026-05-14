#ifndef PYSHICSENGINE_H
#define PYSHICSENGINE_H
#include <cmath>
#include <utility>
#include <string>

#include "gametypes.h"
#include "common/gameData.h"

class PhysicsEngine {

public:
    struct PhysicsConfig {
        float gravity;
        float jump_force;
        float move_speed;
        float terminal_velocity;
        float fixed_timestep;
        float friction;
    };

    struct PhysicsState {
        float velocityX;
        float velocityY;
        bool is_grounded;
    };

    PhysicsEngine();

    void update(PhysicsState& state, Position& position) const;

    void jump(PhysicsState& state) const;

    void move(PhysicsState& state, const Direction& dir);

    void set_new_ground_state(PhysicsState& state, bool new_state) const;

    void add_force(PhysicsState& state, float force_x, float force_y);

    Player_Data get_data_state(const PhysicsState& state, const Position& position) const;

private:
    PhysicsConfig config;
    PhysicsState initial_state;

    void load_config(const std::string& config_path, PhysicsState& state,
        PhysicsConfig& config);

    void on_groud_update(PhysicsState& state, Position& position) const;
    void on_air_update(PhysicsState& state, Position& position) const;
};


#endif  // PYSHICSENGINE_H
