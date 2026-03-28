#include "SpellTrap.h"
#include "Player.h"
#include "GameField.h"

bool SpellTrap::Cast(Player& player, GameField& field, const SpellContext& ctx) {
    auto [x,y] = ctx.targetCell.value_or(field.PlayerCell());
    int dmg = baseDamage_ + player.Enh().trapDamageBonus;
    field.PlaceTrap(x,y,dmg);
    player.ConsumeEnhancement();
    return true;
}