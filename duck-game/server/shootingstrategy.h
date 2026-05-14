#ifndef SHOOTINGSTRATEGY_H
#define SHOOTINGSTRATEGY_H
#include <memory>
#include <utility>
#include <vector>

#include "bullet.h"
#include "gametypes.h"

struct ShootingContext {
    float min_dispersion = 0;           // Cuantos grados minimos de dispersion habra entre balas
    float dispersion_increment = 0;     // Incremento de dispersion
    float max_dispersion = 0;           // Maximo grado de dispersion
    float recoil = 0;                   // Retroceso
    int bullets_per_shot = 0;           // Número de balas por disparo
    int ammo = 0;                       // Munición máxima
    int min_range = 0;                  // Minimo alcance
    int max_range = 0;                  // Maximo alcance
    float bullet_speed = 0;             // Velocidad de la bala
    int frame_per_bullets = 0;          // Frames que tienen que pasar para disparar la prox bala
    float bullet_offset_x = 0;          // Offset de donde spawnea la bala en x
    float bullet_offset_y = 0;          // Offset de donde spawnea la bala en y
    BulletType bullet_type = NORMAL;    // Tipo de bala que dispara
};


class ShootingStrategy {
protected:
    ShootingContext context;
    float dispersion;
    int timer;
    bool can_shoot;
    bool has_actions;

    virtual std::pair<float, float> make_bullet_dir(const Direction& dir) = 0;

    void update_dispersion();

    void reset_dispersion();

    int get_bullet_life_time() const;

    void set_timer();

    void set_timer(int new_timer);

    void decrease_timer();

    Position get_position_w_offset(const Direction& direction,
                                   const Position& origin) const;

    std::unique_ptr<Bullet> make_bullet(int life_time, const Position& pos, float speed,
                                        const std::pair<float, float>& dir) const;
public:
    explicit ShootingStrategy(const ShootingContext& ctx);

    virtual std::vector<std::unique_ptr<Bullet>> shoot(const Position& origin,
                                                       const Direction& direction) = 0;

    virtual std::vector<std::unique_ptr<Bullet>> update(const Position& origin,
                                                       const Direction& direction) = 0;

    virtual void reset() = 0;

    int get_ammo() const;

    virtual ~ShootingStrategy() = default;

    bool has_actions_left() const;
};


#endif  // SHOOTINGSTRATEGY_H
