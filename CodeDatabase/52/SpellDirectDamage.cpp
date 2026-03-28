#include "SpellDirectDamage.h"
#include "Player.h"
#include "GameField.h"
#include "Enemy.h"

bool SpellDirectDamage::Cast(Player& player, GameField& field, const SpellContext& ctx) {
    if (!ctx.targetCell) return false;
    auto [tx, ty] = *ctx.targetCell;
    auto [px, py] = field.PlayerCell();
    int r = baseRadius_ + player.Enh().radiusBonus;
    int dx = tx - px, dy = ty - py;
    if (dx*dx + dy*dy > r*r) return false;

    if (auto* e = field.EnemyAt(tx,ty)) {
        e->TakeDamage(baseDamage_);
        player.ConsumeEnhancement();
        field.KillDead();
        return true;
    }
    return false;
}