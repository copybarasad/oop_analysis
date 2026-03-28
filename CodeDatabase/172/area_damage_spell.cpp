#include <cstdlib>

#include "area_damage_spell.h"
#include "spell_context.h"
#include "player.h"
#include "field.h"

namespace game {
    bool AreaDamageSpell::canCast(const SpellContext& ctx) const
    {
        (void)ctx;
        return true; // always can be cast
    }

    bool AreaDamageSpell::cast(SpellContext& ctx)
    {
        if (!canCast(ctx)) return false;

        const Pos playerPos = ctx.player.getPosition();

        return ctx.field.applyAreaDamage(playerPos, radius_, baseDamage_);
    }

    void AreaDamageSpell::applyLevelUp(const GameConfig& cfg)
    {
        baseDamage_ += cfg.spellDamageUpgradeValue;
        radius_ += cfg.spellRadiusUpgradeValue;
    }
}