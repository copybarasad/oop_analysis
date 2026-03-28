#include "direct_damage_spell.h"
#include "helpers.h"
#include "spell_context.h"

#include "field.h"
#include "player.h"
#include "enemy.h"
#include "etower.h"

namespace game {
    bool DirectDamageSpell::canCast(const SpellContext& ctx) const
    {
        if (!ctx.targetEntityPos) return false;

        const Pos target = *ctx.targetEntityPos;
        if (helpers::manhattan(ctx.player.getPosition(), target) > radius_) return false;

        return true;
    }

    bool DirectDamageSpell::cast(SpellContext& ctx)
    {
        if (!canCast(ctx)) return false;

        const Pos target = *ctx.targetEntityPos;
        const bool applied = ctx.field.damageEnemyAt(target, baseDamage_);

        return applied;
    }

    void DirectDamageSpell::applyLevelUp(const GameConfig& cfg)
    {
        baseDamage_ += cfg.spellDamageUpgradeValue;
        radius_ += cfg.spellRadiusUpgradeValue;
    }

    bool DirectDamageSpell::castFromTower(Field& field, const ETower& tower)
    {
        const Pos playerPos = field.getPlayer().getPosition();
        const Pos towerPos = tower.getPosition();
        

        if (helpers::manhattan(playerPos, towerPos) > radius_) return false;

        field.damagePlayer(baseDamage_);
        return true;
    }
}