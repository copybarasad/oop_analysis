#include "EnemyTower.h"


#include "core/GameLogic.h"
#include "spell/types/DamageSpell.h"
#include "world/World.h"

void EnemyTower::render(const IRenderer & renderer, const World &world, int center_x, int center_y) {
    render_model(renderer, "enemy_tower", center_x, center_y, 20, 32);
    render_health(renderer, world, center_x, center_y - GUI_SCALE * 35);
}

void EnemyTower::tick(GameLogic &logic) {
    shoot_tick_++;
    if (shoot_tick_ < shoot_rate_) {
        return;
    }
    int radius = 1;
    for (int x = -radius; x <= radius; ++x) {
        for (int y = -radius; y <= radius; ++y) {
            int tx = x + x_;
            int ty = y + y_;

            if (!logic.world().is_valid_tile(tx, ty)) continue;
            Entity *entity = logic.world().get_tile(tx, ty).entity();
            if (entity != nullptr && !entity->is_ally(*this)) {
                DamageSpell damage_spell;
                damage_spell.cast(logic, *this, logic.world().get_screen_x(tx, ty), logic.world().get_screen_y(tx, ty),
                                  radius);
                shoot_tick_ = 0;
            }
        }
    }
}

EnemyTower::EnemyTower(const int x, const int y, const int spawn_rate) : Entity(EntityType::EnemyTower, x, y),
                                                                         shoot_rate_(std::max(1, spawn_rate)) {
    this->set_attribute(Attribute::Health, 25);
    this->set_attribute(Attribute::MaxHealth, 25);
    this->set_attribute(Attribute::Gold, 20);
}
