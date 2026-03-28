#include "SpellEnhance.h"
#include "Player.h"

bool SpellEnhance::Cast(Player& player, GameField&, const SpellContext&) {
    auto& e = player.Enh();
    e.radiusBonus     += r_;
    e.areaBonus       += a_;
    e.trapDamageBonus += t_;
    e.summonBonus     += s_;
    ++e.stacks;
    return true;
}