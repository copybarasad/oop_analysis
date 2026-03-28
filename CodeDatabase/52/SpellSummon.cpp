#include "SpellSummon.h"
#include "Player.h"
#include "GameField.h"

bool SpellSummon::Cast(Player& player, GameField& field, const SpellContext& ctx) {
    auto [cx,cy] = ctx.targetCell.value_or(field.PlayerCell());
    int total = count_ + player.Enh().summonBonus;
    field.SpawnAlliesAround(cx,cy,total);
    player.ConsumeEnhancement();
    return true;
}