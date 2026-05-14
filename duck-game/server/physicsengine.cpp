#include "physicsengine.h"

#include <algorithm>
#include <yaml-cpp/yaml.h>
#include <iostream>

PhysicsEngine::PhysicsEngine() {
    load_config(CONFIG_PATH "/config.yaml", initial_state, config);
}

void PhysicsEngine::update(PhysicsState& state, Position& position) const {
    if (state.is_grounded) {
        on_groud_update(state,position);
    } else {
        on_air_update(state, position);
    }

}
void PhysicsEngine::on_groud_update(PhysicsState& state, Position& position) const {
    state.velocityX = std::clamp(state.velocityX,
                             -config.terminal_velocity,
                             config.terminal_velocity);
    // Actualizar posiciones
    position.x += state.velocityX * config.fixed_timestep;

    // Disminuir la velocidad
    if (state.velocityX > 0) {
        state.velocityX -= config.friction;
        if (state.velocityX < 0)
            state.velocityX = 0;
    } else if (state.velocityX < 0) {
        state.velocityX += config.friction;
        if (state.velocityX > 0)
            state.velocityX = 0;
    }
}

void PhysicsEngine::on_air_update(PhysicsState& state, Position& position) const {
    state.velocityY += config.gravity * config.fixed_timestep;

    // Limitar la velocidad de caída
    if (state.velocityY > config.terminal_velocity) {
        state.velocityY = config.terminal_velocity;
    }

    position.y += state.velocityY * config.fixed_timestep;
    position.y += pow(config.fixed_timestep, 2) * config.gravity;

    // En el aire tiene el movimiento reducido y no hay friccion
    position.x += state.velocityX * config.fixed_timestep * 0.75;
}




void PhysicsEngine::jump(PhysicsState& state) const {
    if (state.is_grounded) {
        state.velocityY = config.jump_force;
        state.is_grounded = false;
    }
}

void PhysicsEngine::move(PhysicsState& state, const Direction& dir) {
    switch (dir) {
        case Direction::RIGHT:
            state.velocityX = config.move_speed;
            break;
        case Direction::LEFT:
            state.velocityX = -config.move_speed;
            break;
        default:
            state.velocityX = 0;
            break;
    }
}

void PhysicsEngine::add_force(PhysicsState& state, const float force_x, const float force_y) {
    state.velocityX = force_x;
    state.velocityY = -force_y;
}


Player_Data PhysicsEngine::get_data_state(const PhysicsState& state,
                                         const Position& position) const {

    Player_Data data{};
    data.x = static_cast<int>(position.x);
    data.y = static_cast<int>(position.y);
    data.is_jumping = state.velocityY < 0;
    data.is_running = state.velocityX != 0;

    return data;
}

void PhysicsEngine::set_new_ground_state(PhysicsState& state, bool new_state) const {
    // Si toco el suelo se elimina toda velocidad en el eje y
    if (new_state) {
        state.velocityY = 0;
    }
    state.is_grounded = new_state;
}

void PhysicsEngine::load_config(const std::string& config_path, 
    PhysicsState& state, PhysicsConfig& config) {

    YAML::Node file = YAML::LoadFile(config_path);

    // Leer physicsState
    YAML::Node physicsState = file["physicsState"];
    state.velocityX = physicsState["velocity_x"].as<float>();
    state.velocityY = physicsState["velocity_y"].as<float>();
    state.is_grounded = false;

    // Leer physicsConfig
    YAML::Node physicsConfig = file["physicsConfig"];
    config.gravity = physicsConfig["gravity"].as<float>();
    config.jump_force = physicsConfig["jump_force"].as<float>();
    config.move_speed = physicsConfig["move_speed"].as<float>();
    config.terminal_velocity = physicsConfig["terminal_velocity"].as<float>();
    config.fixed_timestep = physicsConfig["fixed_timestamp"].as<float>();
    config.friction = physicsConfig["fraction"].as<float>();
}
