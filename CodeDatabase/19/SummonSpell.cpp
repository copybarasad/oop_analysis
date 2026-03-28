#include "SummonSpell.h"

#include "core/Game.h"
#include "entity/types/Ally.h"
#include "world/World.h"

bool SummonSpell::cast(GameLogic &logic, Entity &caster, int mouse_x, int mouse_y, int level) {
    for (int i = 0; i < level; ++i) {
        const auto &pos = logic.world().pick_random_safe_pos();
        if (pos.first < 0 || pos.second < 0) continue;
        logic.add_entity(new Ally(pos.first, pos.second));
    }
    return true;
}

void SummonSpell::render_preview(IRenderer &renderer, const World &world, const Player &player, int mouse_x,
                                 int mouse_y, int level) {
}
