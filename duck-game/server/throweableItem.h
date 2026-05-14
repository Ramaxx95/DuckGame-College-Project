#ifndef THROWEABLE_ITEM_H
#define THROWEABLE_ITEM_H
#include <memory>
#include <vector>

#include "IActionableItem.h"
#include "gametypes.h"

class ThroweableItem : public IActionableItem {
    protected:
    struct ThrowConfig {
        float speed = 200.0f;           // Velocidad
        float throw_angle = 45.0f;      // Se lanza a 45°
        float offset_x = 0.0f;          // offset x de donde se lanzara la granada
        float offset_y = 0.0f;          // offset y de donde se lanzara la granada
    };

    ThrowConfig config;
    bool has_actions;

    Position make_position(const Direction& direction, const Position& origin) const;

    std::pair<float, float> make_dir(const Direction& dir) const;

    public:
    explicit ThroweableItem(const ThrowConfig& config);


    virtual std::vector<std::unique_ptr<Bullet>> throw_item(const Position& pos,
                                                            const Direction& dir) = 0;

    // Metodos de la Interfaz
    std::vector<std::unique_ptr<Bullet>> on_press_action(const Position& origin,
                                                         const Direction& direction) override = 0;
    void on_release_action() override = 0;

    std::vector<std::unique_ptr<Bullet>> update(const Position& origin,
                                                const Direction& direction) override = 0;
    unsigned char get_type() override = 0;

    int get_ammo() override = 0;

    bool has_action() override = 0;
};



#endif //THROWEABLE_ITEM_H
