#include "Trap.h"

#include "world/World.h"

void Trap::render(const IRenderer &renderer, const World &world, int center_x, int center_y) {
    render_model(renderer, "trap", center_x, center_y, 6, 24);
}

bool Trap::is_hostile() const {
    return hostile_;
}

Trap::Trap(const int x, const int y, bool hostile, int damage) : Entity(EntityType::Trap, x, y), hostile_(hostile) {
    set_attribute(Attribute::Damage, damage);
}
