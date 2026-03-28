#include "SpellAreaDamage.h"
#include "Player.h"
#include "GameField.h"
#include "Enemy.h"

bool SpellAreaDamage::Cast(Player& player, GameField& field, const SpellContext& ctx) {
    if (!ctx.targetCell) return true;
    auto [tx, ty] = *ctx.targetCell;
    auto [px, py] = field.PlayerCell();
    int r = baseRadius_ + player.Enh().radiusBonus;
    int dx = tx - px, dy = ty - py;
    if (dx*dx + dy*dy > r*r) return true;

    int size = baseSize_ + player.Enh().areaBonus;
    for (int oy=0; oy<size; ++oy)
        for (int ox=0; ox<size; ++ox) {
            int x = tx + ox;
            int y = ty + oy;
            if (auto* e = field.EnemyAt(x,y))
                e->TakeDamage(baseDamage_);
        }
    player.ConsumeEnhancement();
    field.KillDead();
    return true;
}