#include "collisionEventManager.h"

CollisionEventManager::CollisionEventManager() = default;


bool CollisionEventManager::on_platform_collision(Player& player, const Platform& platform,
                                                  const CollisionResult& collision_result) {
    bool ground_state = false;

    if (!collision_result.collided) {
        player.set_groud_state(ground_state);
        return false;
    }

    Position newPosition = player.get_position();


    const auto player_collider = player.get_collider();
    const auto platform_collider = platform.get_collider();

    switch (collision_result.type) {
        case CollisionResult::TOP:
            newPosition.y = platform.get_position().y - 32;  // tamaño textura player
            ground_state = true;
            player.set_groud_state(ground_state);
            player.set_position(newPosition);
            return true;
        case CollisionResult::BOTTOM:
            newPosition.y = platform.get_position().y + platform_collider.get_height() -
                            9;  // offset del collider del player
            break;
        case CollisionResult::LEFT:
            newPosition.x = platform_collider.get_position().x - player_collider.get_length() - 9;
            break;
        case CollisionResult::RIGHT:
            newPosition.x = platform_collider.get_position().x + platform_collider.get_length() -
                            7;  // offset del collider del player
            break;
        default:
            break;
    }
    player.set_position(newPosition);

    return false;
}


void CollisionEventManager::on_platform_collision(Bullet& bullet,
                                                  const CollisionResult& collision_result) {

    bullet.on_platform_collision(collision_result);
}

void CollisionEventManager::out_of_map_check(Player& player, float max_x, float max_y) {
    Position new_position = player.get_position();
    float player_height = player.get_collider().get_height();
    float player_lenght = player.get_collider().get_length();

    if (new_position.y >= max_y - player_height) {
        player.set_dead(true);
    }

    new_position.x = std::clamp(new_position.x, 0.0f, max_x - player_lenght);
    new_position.y = std::clamp(new_position.y, 0.0f, max_y - player_height);

    player.set_position(new_position);
}

void CollisionEventManager::on_player_collision(Bullet& bullet, Player& player) {
    switch (bullet.player_hit()) {
        case BulletEffect::TAKE_DAMAGE:
            player.receive_damage();
            bullet.set_bullet_dead();
            break;
        case BulletEffect::PUSH_PLAYER:
            player.push(push_force_info.x, push_force_info.y);
            player.set_groud_state(false);
            player.set_ragdool();
            break;
    }
}
