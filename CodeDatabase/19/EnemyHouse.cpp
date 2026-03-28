#include "EnemyHouse.h"

#include "Enemy.h"
#include "core/Game.h"
#include "world/World.h"

void EnemyHouse::render(const IRenderer & renderer, const World &world, int center_x, int center_y) {
    render_model(renderer, "enemy_house", center_x, center_y, 10, 32);
    render_health(renderer, world, center_x, center_y - GUI_SCALE * 25);
}

void EnemyHouse::tick(GameLogic &logic) {
    spawn_tick_++;
    if (spawn_tick_ % spawn_rate_ == 0) {
        auto pos = logic.world().pick_random_adjacent_pos(x_, y_);
        auto *enemy = new Enemy(pos.first, pos.second);
        if (!logic.add_entity(enemy)) {
            delete enemy;
        }
    }
}

EnemyHouse::EnemyHouse(const int x, const int y, const int spawn_rate) : Entity(EntityType::EnemyHouse, x, y),
                                                                         spawn_rate_(std::max(1, spawn_rate)) {
    this->set_attribute(Attribute::Health, 40);
    this->set_attribute(Attribute::MaxHealth, 40);
    this->set_attribute(Attribute::Gold, 15);
}
