#include "DamageSpell.h"

DamageSpell::DamageSpell(std::string name, int damage, int range)
    : name_(std::move(name))
    , damage_(damage)
    , range_(range) 
{}

std::string DamageSpell::GetName() const { 
    return name_; 
}

void DamageSpell::ApplyBuffs(BuffState const& state) 
{
    range_ += state.damageRangeBonus;
}

// bool DamageSpell::Apply(Player& caster, Game& game, Position target) 
// {
//     Position casterPos = caster.GetPosition();

//     if (!game.IsInBounds(target)) return false;
//     if (!game.IsInRange(casterPos, target, range_)) return false;

//     bool hit = false;

//     hit = game.DamageEnemyOrTowerAt(target, damage_);

//     if (!hit)
//         hit = game.DamagePlayerAt(target, damage_);

//     return hit;
// }

bool DamageSpell::Apply(Player& caster, Game& game, Position target)
{
    if (!game.IsInBounds(target)) return false;

    bool hit = false;

    hit = game.DamageEnemyOrTowerAt(target, damage_);
    if (!hit)
        hit = game.DamagePlayerAt(target, damage_);

    return hit;
}