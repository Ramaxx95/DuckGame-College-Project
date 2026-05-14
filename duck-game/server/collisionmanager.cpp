#include "collisionmanager.h"

#include <algorithm>
#include <iostream>

CollisionManager::CollisionManager() = default;

CollisionResult CollisionManager::get_collision_info(const RectangleCollider& player_collider, const RectangleCollider& platform_collider) {
    CollisionResult collision{};

    Position player_pos = player_collider.get_position();
    Position platform_pos = platform_collider.get_position();

    // ancho como el largo de cada collider
    float player_length = player_collider.get_length();
    float player_height = player_collider.get_height();
    float platform_length = platform_collider.get_length();
    float platform_height = platform_collider.get_height();

    // posiciones del collider del jugador
    float player_left = player_pos.x;
    float player_right = player_pos.x + player_length;
    float player_top = player_pos.y;
    float player_bottom = player_pos.y + player_height;


    const float OFFSET = 0.5f;
    float platform_left = platform_pos.x;
    float platform_right = platform_pos.x + platform_length;
    float platform_top = platform_pos.y - OFFSET;
    float platform_bottom = platform_pos.y + platform_height;

    // Verifico si hay colision
    bool has_horizontal_overlap = player_right >= platform_left && player_left <= platform_right;
    bool has_vertical_overlap = player_bottom >= platform_top && player_top <= platform_bottom;

    if (has_horizontal_overlap && has_vertical_overlap) {
        collision.collided = true;

        // Calculo las penetraciones de cada lado
        float overlap_from_left = player_right - platform_left;
        float overlap_from_right = platform_right - player_left;
        float overlap_from_top = player_bottom - platform_top;
        float overlap_from_bottom = platform_bottom - player_top;

        // Se busca la penetracion minima para saber de que lado colisiono
        float min_overlap = std::min({
            overlap_from_top,
            overlap_from_left,
            overlap_from_right,
            overlap_from_bottom
        });

        // se busca que lado fue
        if (min_overlap == overlap_from_top) {
            collision.type = CollisionResult::TOP;
        } else if (min_overlap == overlap_from_bottom) {
            collision.type = CollisionResult::BOTTOM;
        } else if (min_overlap == overlap_from_left) {
            collision.type = CollisionResult::LEFT;
        } else {
            collision.type = CollisionResult::RIGHT;
        }
    }

    return collision;
}


bool CollisionManager::verify_collision(const RectangleCollider& object1, const RectangleCollider& object2) {
    Position collider_a = object1.get_position();
    Position collider_b = object2.get_position();

    // ancho como el largo de cada collider
    float collider_a_length = object1.get_length();
    float collider_a_width = object1.get_height();
    float collider_b_length = object2.get_length();
    float collider_b_width = object2.get_height();


    float collider_a_left = collider_a.x;
    float collider_a_right = collider_a.x + collider_a_length;
    float collider_a_top = collider_a.y;
    float collider_a_bottom = collider_a.y + collider_a_width;

    float collider_b_left = collider_b.x;
    float collider_b_right = collider_b.x + collider_b_length;
    float collider_b_top = collider_b.y;
    float collider_b_bottom = collider_b.y + collider_b_width;

    // Verifico si hay colision
    bool has_horizontal_overlap = collider_a_right > collider_b_left && collider_a_left < collider_b_right;
    bool has_vertical_overlap = collider_a_bottom > collider_b_top && collider_a_top < collider_b_bottom;

    return  has_horizontal_overlap && has_vertical_overlap;
}

