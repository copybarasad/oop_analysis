#include "Ally.h"

#include "core/Game.h"
#include "world/World.h"

void Ally::render(const IRenderer & renderer, const World &world, int center_x, int center_y) {
    render_model(renderer, "ally", center_x, center_y, 6, 20);
    render_health(renderer, world, center_x, center_y - GUI_SCALE * 23);
}

void Ally::tick(GameLogic &logic) {

    int dist = 100000;
    Entity* target = nullptr;
    for (auto e : logic.world().entities()) {
        if (!e->is_ally(*this)) {
            int d = e->manh_distance(x_, y_);
            if(d < dist) {
                target = e;
                dist = d;
            }
        }
    }
    int intelligence = get_attribute(Attribute::Intelligence);
    int roll = rand() % 100;

    auto pos = (intelligence > roll && target != nullptr)
        ? logic.world().get_path_pos(x_, y_, target->x(), target->y())
        : logic.world().pick_random_adjacent_pos(x_, y_);

    auto &tile = logic.world().get_tile(pos.first, pos.second);
    if (!logic.move_entity(this, pos.first, pos.second)) {
        Entity *entity = tile.entity();
        if (entity != nullptr && !entity->is_ally(*this)) {
            logic.damage_entity(entity, get_attribute(Attribute::Damage));
        }
    }
}

Ally::Ally(const int x, const int y) : Entity(EntityType::Ally, x, y) {
    this->set_attribute(Attribute::Health, 7);
    this->set_attribute(Attribute::MaxHealth, 7);
    this->set_attribute(Attribute::Damage, 2);
    this->set_attribute(Attribute::Intelligence, 80);
}
