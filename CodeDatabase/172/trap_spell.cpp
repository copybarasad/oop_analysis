#include "trap_spell.h"
#include "spell_context.h"
#include "player.h"
#include "field.h"

namespace game {
    bool TrapSpell::canCast(const SpellContext& ctx) const {
        const Pos playerPos = ctx.player.getPosition();
        const Cell& playerCell = ctx.field.getCell(playerPos.y, playerPos.x);
        if (!playerCell.isWalkable() && playerCell.isSlime())
            return false;

        return true;
    }

    bool TrapSpell::cast(SpellContext& ctx) {
        if (!canCast(ctx)) return false;

        const Pos playerPos = ctx.player.getPosition();
        ctx.field.placeTrap(playerPos, dmg_);
        return true;
    }

    void TrapSpell::applyLevelUp(const GameConfig& cfg) 
    {
        dmg_ += cfg.spellDamageUpgradeValue;
    }
}