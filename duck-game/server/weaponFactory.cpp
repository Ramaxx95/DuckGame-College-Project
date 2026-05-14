#include "weaponFactory.h"

#include <random>

#include "burstShootStrategy.h"
#include "continuosShootStrategy.h"
#include "singleshotstrat.h"
#include "spreadShootStrategy.h"

std::unique_ptr<IActionableItem> WeaponFactory::create_weapon(const WeaponType weapon) {
    ShootingContext context{};

    // se configura el contexto segun el tipo de arma
    switch (weapon) {
        case COWBOY_PISTOL:
            context = {
                    .min_dispersion = 0.f,        // no dispersion
                    .dispersion_increment = 0.f,  // no dispersion
                    .max_dispersion = 0.f,        // no dispersion
                    .recoil = 0.f,                // no retroceso
                    .bullets_per_shot = 1,        // 1 disparo
                    .ammo = 6,                    // 6 balas
                    .min_range = 20,              // 20 Tiles Min
                    .max_range = 20,              // 20 Tiles Max
                    .bullet_speed = 350.0f,       // Velocidad
                    .bullet_offset_x = 37,
                    .bullet_offset_y = 14,
            };
            return std::make_unique<PlayerWeapon>(
                    std::make_unique<SingleShotStrat>(context),  // Sin tiempo de recarga
                    COWBOY_PISTOL);

        case MAGNUM:
            context = {
                    .min_dispersion = 0.0f,        // 10° de min_dispersion
                    .dispersion_increment = 0.0f,  // Dispersion constante
                    .max_dispersion = 10.0f,       // Dispersion constante
                    .recoil = 4.0f,                // Fuerza de retroceso
                    .bullets_per_shot = 1,         // 1 disparo
                    .ammo = 6,                     // 6 balas
                    .min_range = 20,               // 20 tiles Min
                    .max_range = 20,               // 20 tiles Max
                    .bullet_speed = 350.0f,        // Velocidad
                    .frame_per_bullets = 2,
                    .bullet_offset_x = 26,
                    .bullet_offset_y = 14,
            };
            return std::make_unique<PlayerWeapon>(
                    std::make_unique<SingleShotStrat>(context),  // Sin tiempo de recarga
                    MAGNUM);

        case DUEL_PISTOL:
            context = {
                    .min_dispersion = 20.f,        // 20° de min_dispersion
                    .dispersion_increment = 0.0f,  // Dispersion constante
                    .max_dispersion = 20.f,        // Dispersion constante
                    .recoil = 0.f,                 // Sin retroceso
                    .bullets_per_shot = 1,         // 1 Disparo
                    .ammo = 1,                     // 1 bala
                    .min_range = 5,                // 5 tiles Min
                    .max_range = 5,                // 5 tiles Max
                    .bullet_speed = 350.0f,        // Velocidad de la bala
                    .bullet_offset_x = 34,
                    .bullet_offset_y = 14,
            };
            return std::make_unique<PlayerWeapon>(
                    std::make_unique<SingleShotStrat>(context),  // Sin tiempo de carga
                    DUEL_PISTOL);

        case AK47:
            context = {
                    .min_dispersion = 2.0f,        // 10° de min_dispersion
                    .dispersion_increment = 1.0f,  // Incrementa de a 3°
                    .max_dispersion = 20.0f,       // Aumenta hasta 20°
                    .recoil = 10.0f,               // Fuerza de retroceso
                    .bullets_per_shot = 1,         // 1 Disparo
                    .ammo = 30,                    // 30 balas
                    .min_range = 13,               // 13 tiles Min
                    .max_range = 13,               // 13 tiles Max
                    .bullet_speed = 400.0f,        // Velocidad de la bala
                    .frame_per_bullets = 2,
                    .bullet_offset_x = 36,
                    .bullet_offset_y = 16,
            };
            return std::make_unique<PlayerWeapon>(
                    std::make_unique<ContinuosShootStrategy>(context, true), AK47);

        case SHOTGUN:
            context = {
                    .min_dispersion = 10.0f,       // 20° de min_dispersion
                    .dispersion_increment = 0.0f,  // Dispersion Constante
                    .max_dispersion = 20.0f,       // Dispersion Constante
                    .recoil = 10.0f,               // retroceso
                    .bullets_per_shot = 6,         // 6 perdigones
                    .ammo = 2,                     // 2 disparos
                    .min_range = 7,                // 7 tiles Min
                    .max_range = 9,                // 9 tiles Max
                    .bullet_speed = 350.0f,        // velocidad de la bala
                    .bullet_offset_x = 31,
                    .bullet_offset_y = 15,
            };
            return std::make_unique<PlayerWeapon>(
                    std::make_unique<SpreadShootStrategy>(context),  // 1 segundos de recarga
                    SHOTGUN);

        case PEW_PEW_LASERM:
            context = {
                    .min_dispersion = 2.0f,        // 2° de min_dispersion
                    .dispersion_increment = 0.0f,  // Dispersion constante
                    .max_dispersion = 2.0f,        // Dispersion constante
                    .recoil = 0.0f,                // No retroceso
                    .bullets_per_shot = 3,         // 3 diparos
                    .ammo = 12,                    // 12 balas
                    .min_range = 35,               // 35 tiles Min
                    .max_range = 38,               // 38 tiles Max
                    .bullet_speed = 450.0f,        // velocidad de la bala
                    .frame_per_bullets = 2,
                    .bullet_offset_x = 35,
                    .bullet_offset_y = 15,
            };
            return std::make_unique<PlayerWeapon>(std::make_unique<BurstShootStrategy>(context, 3),
                                                  PEW_PEW_LASERM);

        case LASER_RIFLE:
            context = {.min_dispersion = 45.0f,       // 45° de dispesion
                       .dispersion_increment = 0.0f,  // Dispersion constante
                       .max_dispersion = 45.0f,       // Dispersion constante
                       .recoil = .0f,                 // No retroceso
                       .bullets_per_shot = 3,         // 3 disparos
                       .ammo = 10,                    // 10 balas
                       .min_range = 30,               // 30 tiles Min
                       .max_range = 30,               // 30 tiles Max
                       .bullet_speed = 450.0f,        // velocidad de la bala
                       .frame_per_bullets = 2,
                       .bullet_offset_x = 30,
                       .bullet_offset_y = 13,
                       .bullet_type = ELASTIC};
            return std::make_unique<PlayerWeapon>(
                    std::make_unique<ContinuosShootStrategy>(context, false), LASER_RIFLE);

        case SNIPER_RIFLE:
            context = {
                    .min_dispersion = 0.0f,        // No dispersion
                    .dispersion_increment = 0.0f,  // No dispersion
                    .max_dispersion = 0.0f,        // No dispersion
                    .recoil = 0.f,                 // No retroceso
                    .bullets_per_shot = 1,         // 1 disparo
                    .ammo = 3,                     // 3 balas
                    .min_range = 64,               // 64 tiles
                    .max_range = 64,               // 64 tiles Max
                    .bullet_speed = 600.0f,        // Velocidad de la bala
                    .bullet_offset_x = 34,
                    .bullet_offset_y = 14,
            };
            return std::make_unique<PlayerWeapon>(
                    std::make_unique<SingleShotStrat>(context),  // 2 segundos de recarga
                    SNIPER_RIFLE);

        case WEAPON_NONE:
        default:
            throw std::invalid_argument("Weapon type not supported or invalid");
    }
}


std::unique_ptr<IActionableItem> WeaponFactory::create_weapon() {
    // generador de números aleatorios
    static std::random_device rd;
    static std::mt19937 gen(rd());


    std::uniform_int_distribution<> dis(1, SNIPER_RIFLE);

    return create_weapon(static_cast<WeaponType>(dis(gen)));
}
