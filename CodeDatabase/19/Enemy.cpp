#include "Enemy.h"

#include "core/Game.h"
#include "world/World.h"

void Enemy::render(const IRenderer & renderer, const World &world, int center_x, int center_y) {
    render_model(renderer, "enemy", center_x, center_y, 6, 24);
    render_health(renderer, world, center_x, center_y - GUI_SCALE * 25);
}

void Enemy::tick(GameLogic &logic) {
    Player& player = logic.player();

    int intelligence = get_attribute(Attribute::Intelligence);
    int roll = rand() % 100;

    auto pos = roll < intelligence
                   ? logic.world().get_path_pos(x_, y_, player.x(), player.y())
                   : logic.world().pick_random_adjacent_pos(x_, y_);

    if (!logic.move_entity(this, pos.first, pos.second)) {
        if (player.x() == pos.first && player.y() == pos.second) {
            logic.damage_entity(&player, get_attribute(Attribute::Damage));
        }
    }
}

Enemy::Enemy(const int x, const int y) : Entity(EntityType::Enemy, x, y) {
    this->set_attribute(Attribute::Health, 7);
    this->set_attribute(Attribute::MaxHealth, 7);
    this->set_attribute(Attribute::Damage, 1);
    this->set_attribute(Attribute::Intelligence, 20);
    this->set_attribute(Attribute::Gold, 5);
}
