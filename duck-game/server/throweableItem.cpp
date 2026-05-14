#include "throweableItem.h"

ThroweableItem::ThroweableItem(const ThrowConfig& config) : config(config), has_actions(false){}


std::pair<float, float> ThroweableItem::make_dir(const Direction& dir) const {
    float final_angle = 0.0f;

    auto throw_angle = static_cast<float>(config.throw_angle * M_PI / 180);

    // angulo segun la dirección
    switch (dir) {
        case Direction::LEFT:
            final_angle = M_PI + throw_angle; // 180° en radianes
        break;
        case Direction::RIGHT:
            final_angle = 0.0f - throw_angle;
        break;
        case Direction::UP:
            final_angle = M_PI_2 + throw_angle; // 90° en radianes
        break;
        case Direction::DOWN:
            final_angle = -M_PI_2 - throw_angle; // -90° en radianes
        break;
        default:
            // Sin Direccion
                return {0.0f, 0.0f};
    }

    // obtenemos la componente "x" y la componente "y" del angulo
    float x = std::cos(final_angle);
    float y = std::sin(final_angle);

    return {x, y};
}

Position ThroweableItem::make_position(const Direction& direction, const Position& origin) const {
    float pos_off_y = 0;
    float pos_off_x = 0;

    switch (direction) {
        case Direction::UP:
            // TODO hacer el/los calculos correctos
            pos_off_y = origin.y - config.offset_y;
            pos_off_x = origin.x - config.offset_x / 2;
            break;
        case Direction::LEFT:
            pos_off_y = origin.y + config.offset_y;
            pos_off_x = origin.x;
            break;
        case Direction::RIGHT:
            pos_off_y = origin.y + config.offset_y;
            pos_off_x = origin.x + config.offset_x;
            break;
        default:
            break;
    }
    return Position{pos_off_x, pos_off_y};
}

bool ThroweableItem::has_action() {
    return has_actions;
}

