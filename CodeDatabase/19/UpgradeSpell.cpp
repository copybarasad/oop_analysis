#include "UpgradeSpell.h"

#include "core/Game.h"
#include "entity/Entity.h"

bool UpgradeSpell::cast(GameLogic &logic, Entity &caster, int mouse_x, int mouse_y, int level) {
    if (caster.type() != EntityType::Player) return false;
    SpellHolder &holder = logic.player().get_spell_holder();
    holder.set_spell_buff(holder.get_spell_buff() + 1);
    return true;
}

void UpgradeSpell::render_preview(IRenderer &renderer, const World &world, const Player &player, int mouse_x,
                                  int mouse_y, int level) {
}
